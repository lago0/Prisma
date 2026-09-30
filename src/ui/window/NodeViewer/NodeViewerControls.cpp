#include "NodeViewerControls.h"
#include "NodeViewer.h"
#include "RenderInfo.h"
#include "NodeViewerUtils.h"
#include "NodeViewerGraph.h"
#include "ui/UI.h"
#include "node/NodeInput.h"
#include "node/Node.h"
#include "NodeView.h"
#include "node/nodes/ImageOut.h"

UI::NodeViewerControls::NodeViewerControls(NodeViewer* nodeViewer) :
    nodeViewer(nodeViewer)
{
}

UI::NodeViewerControls::~NodeViewerControls()
{
}

void UI::NodeViewerControls::OnEvent(SDL_Event* event)
{
    auto& renderInfo = this->nodeViewer->graph->renderInfo;

    auto& dragging = this->nodeViewer->dragging;
    auto& draggingConnection = this->nodeViewer->draggingConnection;
    auto& draggingNode = this->nodeViewer->draggingNode;

    auto& draggingConnectionEntity = this->nodeViewer->draggingConnectionEntity;
    auto& draggingConnectionType = this->nodeViewer->draggingConnectionType;
    
    auto& draggingPosOffsetX = this->nodeViewer->draggingPosOffsetX;
    auto& draggingPosOffsetY = this->nodeViewer->draggingPosOffsetY;

    auto& draggingStartPosX = this->nodeViewer->draggingStartPosX;
    auto& draggingStartPosY = this->nodeViewer->draggingStartPosY;

    auto& currentViewPosX = this->nodeViewer->currentViewPosX;
    auto& currentViewPosY = this->nodeViewer->currentViewPosY;
    
    auto& zoom = this->nodeViewer->zoom;

    auto& showCreateMenu = this->nodeViewer->showCreateMenu;

    auto& nodeViews = this->nodeViewer->graph->nodeViews;
    
    auto& ui = this->nodeViewer->ui;

    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX < renderInfo->winPos.x || mouseY < renderInfo->winPos.y
        || mouseX > renderInfo->winPos.x + renderInfo->winSize.x || mouseY > renderInfo->winPos.y + renderInfo->winSize.y)
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
                
                this->nodeViewer->SetSelectedNodeView(selectingNodeView);

                if (selectingNodeView)
                {
                    if (!IsDragging(renderInfo))
                    {
                        Node* node = ui->selectedNodeView->originalNode;

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
                            if (GetIfNodeInputBeingClicked(renderInfo, node, ui->selectedNodeView, index)) {
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
                if (ui->selectedNodeView && ui->selectedLayer)
                {
                    if (dynamic_cast<ImageOut*>(ui->selectedNodeView->originalNode) != nullptr)
                    {
                        return;
                    }

                    Node* node = ui->selectedNodeView->originalNode;
                    ui->selectedNodeView->originalNode->nodeWorkspace->RemoveNode(node);
                    delete node;
                }
            }
            break;
        }
    }
}