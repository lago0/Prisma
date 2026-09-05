#include <SDL3/SDL.h>
#include <imgui.h>
#include <cmath>
#include <math.h>

#include <iostream>

#include "NodeViewer.h"
#include "NodeView.h"
#include "layer/LayerManager.h"
#include "layer/Layer.h"
#include "node/Node.h"
#include "node/NodeWorkspace.h"
#include "ui/UI.h"
#include "node/nodes/ImageOut.h"
#include "NodeMap.h"

float Distance(ImVec2 p1, ImVec2 p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

bool PointInBounds(ImVec2 startPos, ImVec2 endPos, ImVec2 point)
{
    return startPos.x < point.x && startPos.y < point.y
                && endPos.x > point.x && endPos.y > point.y;
}

float CalculateNodeXPosition(UI::RenderInfo* renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView)
{
    return renderInfo->winPos.x + (nodeView->x * renderInfo->window->zoom) + renderInfo->winSize.x / 2 - renderInfo->window->currentViewPosX * renderInfo->window->zoom;
}

float CalculateNodeXPositionInverse(UI::RenderInfo* renderInfo, float posX)
{
    return ((posX - renderInfo->winPos.x - (renderInfo->winSize.x / 2.0f)) / renderInfo->window->zoom) + renderInfo->window->currentViewPosX;
}

float CalculateNodeYPosition(UI::RenderInfo* renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView)
{
    return renderInfo->winPos.y + (nodeView->y * renderInfo->window->zoom) + renderInfo->winSize.y / 2 - renderInfo->window->currentViewPosY * renderInfo->window->zoom;
}

float CalculateNodeYPositionInverse(UI::RenderInfo* renderInfo, float posY)
{
    return ((posY - renderInfo->winPos.y - (renderInfo->winSize.y / 2.0f)) / renderInfo->window->zoom) + renderInfo->window->currentViewPosY;
}

float CalculateNodeXSize(UI::RenderInfo* renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView)
{
    return 200 * renderInfo->window->zoom;
}

float CalculateNodeYSize(UI::RenderInfo* renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView)
{
    return (40 + 20 * (node->nodeInputs.size() > 0 ? node->nodeInputs.size() - 1 : 0)) * renderInfo->window->zoom;
}

ImVec2 CalculateNodeInputPosition(UI::RenderInfo* renderInfo, const Node*, int xPos, int yPos, int index)
{
    return ImVec2(xPos, yPos + 20 * renderInfo->window->zoom + 20 * renderInfo->window->zoom * index);
}

ImVec2 CalculateNodeOutputPosition(UI::RenderInfo* renderInfo, const Node*, int xPos, int yPos, int xSize, int ySize)
{
    return ImVec2(xPos + xSize, yPos + ySize / 2);
}

bool IsDragging(UI::RenderInfo* renderInfo)
{
    return renderInfo->window->dragging || renderInfo->window->draggingNode || renderInfo->window->draggingConnection;
}

void DrawNodeBezier(UI::RenderInfo* renderInfo, ImDrawList* drawList, ImVec2 p1, ImVec2 p4)
{
    float dx = std::abs(p4.x - p1.x);
    float dy = std::abs(p4.y - p1.y);
    
    float minOffset = std::max(30.0f, dy * 0.3f);
    
    float handleOffset = std::max(dx * 0.5f, minOffset);

    ImVec2 p2 = ImVec2(p1.x + handleOffset, p1.y);
    ImVec2 p3 = ImVec2(p4.x - handleOffset, p4.y);

    drawList->AddBezierCubic(p1, p2, p3, p4, IM_COL32(255, 255, 255, 255), 1.0 * renderInfo->window->zoom, 20 * renderInfo->window->zoom);
}

bool GetIfNodeInputBeingClicked(UI::RenderInfo* renderInfo, Node* node, std::shared_ptr<NodeView> nodeView, int index)
{
    float xPos = CalculateNodeXPosition(renderInfo, node, nodeView);
    float yPos = CalculateNodeYPosition(renderInfo, node, nodeView);

    ImVec2 position = CalculateNodeInputPosition(renderInfo, node, xPos, yPos, index);
    ImVec2 mousePos = ImGui::GetMousePos();

    return Distance(position, mousePos) <= 40 * renderInfo->window->zoom;
}

bool GetIfNodeOutputBeingClicked(UI::RenderInfo* renderInfo, Node* node, std::shared_ptr<NodeView> nodeView)
{
    float xPos = CalculateNodeXPosition(renderInfo, node, nodeView);
    float yPos = CalculateNodeYPosition(renderInfo, node, nodeView);

    float xSize = CalculateNodeXSize(renderInfo, node, nodeView);
    float ySize = CalculateNodeYSize(renderInfo, node, nodeView);

    ImVec2 position = CalculateNodeOutputPosition(renderInfo, node, xPos, yPos, xSize, ySize);
    ImVec2 mousePos = ImGui::GetMousePos();

    return Distance(position, mousePos) <= 40 * renderInfo->window->zoom;
}

UI::NodeViewer::NodeViewer(UI* ui) :
    Window(ui),
    renderInfo(new RenderInfo),
    nodeMaps(mapNodes())
{
    renderInfo->window = this;

    for (Layer* layer : ui->GetCore()->layerManager->layers)
    {
        this->OnLayerCreatedCallback(layer);
    }

    ui->GetCore()->layerManager->onLayerCreated->Connect([this](Layer* layer)
    {
        this->OnLayerCreatedCallback(layer);
    });

    ui->GetCore()->layerManager->onLayerDeleted->Connect([this](Layer* layer)
    {
        this->OnLayerDeletedCallback(layer);
    });

    ui->onEvent->Connect([&](SDL_Event* event)
    {
        this->OnEvent(event);
    });
}

UI::NodeViewer::~NodeViewer()
{
    delete renderInfo;
}

void UI::NodeViewer::Update()
{
    if (IsDragging(this->renderInfo))
    {
        ImVec2 mousePos = ImGui::GetMousePos();

        if (dragging)
        {
            currentViewPosX = (draggingStartPosX - mousePos.x) / zoom + draggingPosOffsetX;
            currentViewPosY = (draggingStartPosY - mousePos.y) / zoom + draggingPosOffsetY;
        }
        else if (draggingNode)
        {
            selectedNodeView->x = (mousePos.x - draggingStartPosX) / zoom + draggingPosOffsetX;
            selectedNodeView->y = (mousePos.y - draggingStartPosY) / zoom + draggingPosOffsetY;
        }

        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
    }
}

void UI::NodeViewer::Render()
{
    ImGui::Begin("Node Viewer");
    
    renderInfo->winPos = ImGui::GetCursorScreenPos();
    renderInfo->winSize = ImGui::GetWindowSize();
    renderInfo->drawList = ImGui::GetWindowDrawList();

    isWindowFocused = ImGui::IsWindowFocused();

    const int dotSize = 10 * zoom;

    if (ui != nullptr && ui->selectedLayer != nullptr)
    {
        Layer* selectedLayer = ui->selectedLayer;

        for (const auto& [node, nodeView] : nodeViews[selectedLayer])
        {
            int xPos = CalculateNodeXPosition(renderInfo, node, nodeView);
            int yPos = CalculateNodeYPosition(renderInfo, node, nodeView);

            int xSize = CalculateNodeXSize(renderInfo, node, nodeView);
            int ySize = CalculateNodeYSize(renderInfo, node, nodeView);

            // Frame
            nodeView->realStartPos = ImVec2(xPos, yPos);
            nodeView->realEndPos = ImVec2(xPos + xSize, yPos + ySize);

            renderInfo->drawList->AddRectFilled(nodeView->realStartPos, nodeView->realEndPos, IM_COL32(60,60,60,255), 20.0 * zoom);

            if (selectedNodeView == nodeView)
            {
                // Selection
                renderInfo->drawList->AddRect(nodeView->realStartPos, nodeView->realEndPos, IM_COL32(36, 171, 224,255), 20.0 * zoom, 3.0f);
            }

            // Node name
            ImFont* font = ImGui::GetFont();
            float fontSize = 24.0f * zoom;

            const char* text = node->visualName().c_str();

            ImVec2 text_size = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, text);
            renderInfo->drawList->AddText(font, fontSize, ImVec2(xPos + xSize / 2.0 - text_size.x / 2.0, yPos + ySize / 2.0 - text_size.y / 2.0), IM_COL32(255,255,255,255), text);

            // Output dot
            if (node->outputEnabled)
            {
                ImVec2 outputDotPosition = CalculateNodeOutputPosition(renderInfo, node, xPos, yPos, xSize, ySize);

                ImVec2 outputDotPositionStart = ImVec2(outputDotPosition.x - dotSize / 2, outputDotPosition.y - dotSize / 2);
                ImVec2 outputDotPositionEnd = ImVec2(outputDotPosition.x + dotSize / 2, outputDotPosition.y + dotSize / 2);

                renderInfo->drawList->AddRectFilled(outputDotPositionStart, outputDotPositionEnd, IM_COL32(255,255,255,255), 10.0 * zoom);
            }

            // Input dots
            for (const auto& [index, input] : node->nodeInputs)
            {
                ImVec2 inputDotPosition = CalculateNodeInputPosition(renderInfo, node, xPos, yPos, index);

                ImVec2 inputDotPositionStart = ImVec2(inputDotPosition.x - dotSize / 2, inputDotPosition.y - dotSize / 2);
                ImVec2 inputDotPositionEnd = ImVec2(inputDotPosition.x + dotSize / 2, inputDotPosition.y + dotSize / 2);

                renderInfo->drawList->AddRectFilled(inputDotPositionStart, inputDotPositionEnd, IM_COL32(255,255,255,255), 10.0 * zoom);
            }
        }

        // Connections
        for (const auto& [node, nodeView] : nodeViews[selectedLayer])
        {
            if (!node->outputEnabled)
            {
                continue;
            }

            for (auto* input : node->nodeOutputs)
            {
                int xPos = CalculateNodeXPosition(renderInfo, node, nodeView);
                int yPos = CalculateNodeYPosition(renderInfo, node, nodeView);

                int xSize = CalculateNodeXSize(renderInfo, node, nodeView);
                int ySize = CalculateNodeYSize(renderInfo, node, nodeView);

                ImVec2 outputDotPosition = CalculateNodeOutputPosition(renderInfo, node, xPos, yPos, xSize, ySize);

                Node* connectedNode = input->GetNode();
                if (!connectedNode)
                {
                    continue;
                }

                int nodeInputIndex = 0;
                for (const auto& [index, inputB] : node->nodeInputs)
                {
                    if (inputB == input)
                    {
                        nodeInputIndex = index;
                    }
                }

                auto connectedNodeView = nodeViews[selectedLayer][connectedNode];
                if (!connectedNodeView)
                {
                    continue;
                }

                int xConnectedNodePos = CalculateNodeXPosition(renderInfo, connectedNode, connectedNodeView);
                int yConnectedNodePos = CalculateNodeYPosition(renderInfo, connectedNode, connectedNodeView);

                int xConnectedNodeSize = CalculateNodeXSize(renderInfo, connectedNode, connectedNodeView);
                int yConnectedNodeSize = CalculateNodeYSize(renderInfo, connectedNode, connectedNodeView);

                ImVec2 inputDotPosition = CalculateNodeInputPosition(renderInfo, connectedNode, xConnectedNodePos, yConnectedNodePos, nodeInputIndex);

                DrawNodeBezier(renderInfo, renderInfo->drawList, outputDotPosition, inputDotPosition);
            }
        }

        if (draggingConnection)
        {
            ImVec2 startRealPosition;
            ImVec2 endRealPosition = ImGui::GetMousePos();

            if (draggingConnectionType == 0)
            {
                Node* output = (Node*)draggingConnectionEntity;
                std::shared_ptr<NodeView> nodeView = nodeViews[ui->selectedLayer].at(output);

                float xPos = CalculateNodeXPosition(renderInfo, output, nodeView);
                float yPos = CalculateNodeYPosition(renderInfo, output, nodeView);

                float xSize = CalculateNodeXSize(renderInfo, output, nodeView);
                float ySize = CalculateNodeYSize(renderInfo, output, nodeView);

                startRealPosition = CalculateNodeOutputPosition(this->renderInfo, output, xPos, yPos, xSize, ySize);
            }
            else
            {
                NodeInput* input = (NodeInput*)draggingConnectionEntity;
                Node* node = input->GetNode();
                std::shared_ptr<NodeView> nodeView = nodeViews[ui->selectedLayer].at(node);

                float xPos = CalculateNodeXPosition(renderInfo, node, nodeView);
                float yPos = CalculateNodeYPosition(renderInfo, node, nodeView);

                float xSize = CalculateNodeXSize(renderInfo, node, nodeView);
                float ySize = CalculateNodeYSize(renderInfo, node, nodeView);

                int nodeInputIndex = 0;
                for (const auto& [index, inputB] : node->nodeInputs)
                {
                    if (inputB == input)
                    {
                        nodeInputIndex = index;
                    }
                }

                startRealPosition = CalculateNodeInputPosition(renderInfo, node, xPos, yPos, nodeInputIndex);
            }

            DrawNodeBezier(renderInfo, renderInfo->drawList, startRealPosition, endRealPosition);
        }
    }

    if (showCreateMenu)
    {
        ImGui::OpenPopup("CreateNodeMenu");
        showCreateMenu = false;
    }

    RenderCreateNodeMenu();

    ImGui::End();
}

void UI::NodeViewer::RenderCreateNodeMenu()
{
    if(ui->selectedLayer && ImGui::BeginPopup("CreateNodeMenu"))
    {
        for (NodeMap* nodeMap : nodeMaps)
        {
            if (ImGui::Selectable(nodeMap->nodeName.c_str()))
            {
                Node* node = nodeMap->Create(ui->selectedLayer->nodeWorkspace);

                NodeList& list = nodeViews[ui->selectedLayer];

                while (list.find(node) == list.end()) {};
                
                std::shared_ptr<NodeView> view = list.at(node);

                ImVec2 mousePos = ImGui::GetMousePos();
                ImVec2 realPos = ImVec2(mousePos.x, mousePos.y);

                view->x = CalculateNodeXPositionInverse(renderInfo, realPos.x);
                view->y = CalculateNodeYPositionInverse(renderInfo, realPos.y);
            }
        }

        ImGui::EndPopup();
    }
}

void UI::NodeViewer::OnEvent(SDL_Event* event)
{
    if (!isWindowFocused)
    {
        return;
    }

    switch(event->type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (event->button.button == SDL_BUTTON_MIDDLE && !IsDragging(renderInfo))
            {
                // Move Camera
                dragging = true;
            
                ImVec2 mousePos = ImGui::GetMousePos();

                draggingPosOffsetX = currentViewPosX;
                draggingPosOffsetY = currentViewPosY;

                draggingStartPosX = mousePos.x;
                draggingStartPosY = mousePos.y;
            }
            else if (event->button.button == SDL_BUTTON_LEFT)
            {
                if (draggingConnection)
                {
                    // Exception if it is already dragging a connection

                    for (const auto& [node, nodeView] : nodeViews[ui->selectedLayer])
                    {
                        if (draggingConnectionType == 1) 
                        {
                            draggingConnection = false;
                            
                            NodeInput* nodeInput = (NodeInput*)draggingConnectionEntity;
                            nodeInput->GetNode()->DisconnectInput(nodeInput);

                            if (GetIfNodeOutputBeingClicked(renderInfo, node, nodeView))
                            {
                                node->ConnectOutput(nodeInput);

                                draggingConnection = false;
                                return;
                            }
                        }
                        else
                        {
                            draggingConnection = false;

                            for (const auto& [index, input] : node->nodeInputs)
                            {
                                if (GetIfNodeInputBeingClicked(renderInfo, node, nodeView, index)) {
                                    input->GetNode()->DisconnectInput(input);

                                    Node* startNode = (Node*)draggingConnectionEntity;
                                    startNode->ConnectOutput(input);

                                    return;
                                }
                            }
                        }
                    }

                    return;
                }

                // Select Node
                ImVec2 clickPosition = ImGui::GetMousePos();
                std::shared_ptr<NodeView> selectingNodeView = nullptr;

                for (const auto& [node, nodeView] : nodeViews[ui->selectedLayer])
                {
                    if (PointInBounds(nodeView->realStartPos, nodeView->realEndPos, clickPosition))
                    {
                        selectingNodeView = nodeView;
                    }
                }
                
                SetSelectedNodeView(selectingNodeView);

                if (selectingNodeView)
                {
                    if (!IsDragging(renderInfo))
                    {
                        Node* node = selectedNodeView->originalNode;

                        bool ioClicked = false;

                        ImVec2 mousePos = ImGui::GetMousePos();

                        if (GetIfNodeOutputBeingClicked(renderInfo, node, selectingNodeView))
                        {
                            ioClicked = true;

                            draggingConnection = true;
                            draggingConnectionType = 0;
                            draggingConnectionEntity = (void*)node;
                        }

                        for (const auto& [index, input] : node->nodeInputs)
                        {
                            if (GetIfNodeInputBeingClicked(renderInfo, node, selectedNodeView, index)) {
                                ioClicked = true;

                                draggingConnection = true;
                                draggingConnectionType = 1;
                                draggingConnectionEntity = (void*)input;
                            }
                        }

                        // Detect if touched input or output
                        if (!ioClicked)
                        {
                            // Move Node
                            draggingNode = true;

                            ImVec2 mousePos = ImGui::GetMousePos();
                            ImVec2 realPos = ImVec2(mousePos.x, mousePos.y);

                            draggingPosOffsetX = selectingNodeView->x;
                            draggingPosOffsetY = selectingNodeView->y;

                            draggingStartPosX = realPos.x;
                            draggingStartPosY = realPos.y;
                        }
                    }
                }
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                showCreateMenu = true;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            if (event->button.button == SDL_BUTTON_MIDDLE)
            {
                // Stop Moving Camera
                dragging = false;
            }
            else if (event->button.button == SDL_BUTTON_LEFT)
            {
                // Stop Moving Node
                draggingNode = false;
            }
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: 
        {
            float scrollX = event->wheel.x;
            float scrollY = event->wheel.y;

            if (event->wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                scrollX *= -1.0f;
                scrollY *= -1.0f;
            }
            
            zoom += scrollY * 0.1 * zoom;
            break;
        }
        case SDL_EVENT_KEY_UP: 
        {
            if (event->key.key == SDLK_DELETE)
            {
                if (selectedNodeView && ui->selectedLayer)
                {
                    if (dynamic_cast<ImageOut*>(selectedNodeView->originalNode) != nullptr)
                    {
                        return;
                    }

                    selectedNodeView->originalNode->nodeWorkspace->RemoveNode(selectedNodeView->originalNode);
                    delete selectedNodeView->originalNode;
                }
            }
            break;
        }
    }
}

void UI::NodeViewer::OnLayerCreatedCallback(Layer* layer)
{
    std::cout << "Layer created. Name: " << layer->GetName() << std::endl;
    if (!nodeViews.contains(layer))
    {
        nodeViews[layer] = NodeList();
        
        for (Node* node : layer->nodeWorkspace->nodes)
        {
            this->OnNodeCreatedCallback(layer, node);
        }

        layer->nodeWorkspace->onNodeCreated->Connect([this, layer](NodeWorkspace* workspace, Node* node)
        {
            this->OnNodeCreatedCallback(layer, node);
        });

        layer->nodeWorkspace->onNodeDeleted->Connect([this, layer](NodeWorkspace* workspace, Node* node)
        {
            this->OnNodeDeletedCallback(layer, node);
        });
    }
}

void UI::NodeViewer::OnNodeCreatedCallback(Layer* layer, Node* node)
{
    std::cout << "Node created. Layer name: " << layer->GetName() << ". Node pointer: " << node << std::endl;
    if (nodeViews.contains(layer) && !nodeViews[layer].contains(node))
    {
        float xPos = -250;
        float yPos = 0;

        if (dynamic_cast<ImageOut*>(node) != nullptr)
        {
            // Special position
            xPos = 250;
        }

        nodeViews[layer][node] = std::make_shared<NodeView>();
        nodeViews[layer][node]->x = xPos;
        nodeViews[layer][node]->y = yPos;
        nodeViews[layer][node]->originalNode = node;
    }
}

void UI::NodeViewer::OnLayerDeletedCallback(Layer* oldLayerPointer)
{
    std::cout << "Layer deleted. Old pointer: " << oldLayerPointer << std::endl;
    if (nodeViews.contains(oldLayerPointer))
    {
        nodeViews.erase(oldLayerPointer);

        if (ui->selectedLayer == oldLayerPointer)
        {
            ui->selectedLayer = nullptr;
        }
    }
}

void UI::NodeViewer::OnNodeDeletedCallback(Layer* layer, Node* oldNodePointer)
{
    std::cout << "Node deleted. Layer name: " << layer->GetName() << ". Old pointer: " << oldNodePointer << std::endl;
    if (nodeViews.contains(layer) && nodeViews[layer].contains(oldNodePointer))
    {
        nodeViews[layer].erase(oldNodePointer);
    }
}

void UI::NodeViewer::SetSelectedNodeView(std::shared_ptr<NodeView> view)
{
    selectedNodeView = view;
}

std::shared_ptr<NodeView> UI::NodeViewer::GetSelectedNodeView()
{
    return selectedNodeView;
}