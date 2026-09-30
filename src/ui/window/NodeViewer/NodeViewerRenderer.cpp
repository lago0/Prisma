#include <imgui.h>

#include "NodeViewerRenderer.h"
#include "NodeViewer.h"
#include "RenderInfo.h"
#include "NodeViewerGraph.h"
#include "NodeViewerUtils.h"
#include "ui/UI.h"
#include "NodeView.h"
#include "node/NodeInput.h"
#include "NodeMap.h"

UI::NodeViewerRenderer::NodeViewerRenderer(NodeViewer* nodeViewer) :
    nodeViewer(nodeViewer)
{
}

void UI::NodeViewerRenderer::Render()
{
    ImGui::Begin("Node Viewer");

    auto& renderInfo = this->nodeViewer->graph->renderInfo;

    float& zoom = this->nodeViewer->zoom;

    auto& nodeViews = this->nodeViewer->graph->nodeViews;
    
    auto* ui = this->nodeViewer->ui;

    auto& dragging = this->nodeViewer->dragging;
    auto& draggingConnection = this->nodeViewer->draggingConnection;
    auto& draggingNode = this->nodeViewer->draggingNode;

    auto& draggingConnectionType = this->nodeViewer->draggingConnectionType;
    auto& draggingConnectionEntity = this->nodeViewer->draggingConnectionEntity;
    
    renderInfo->winPos = ImGui::GetCursorScreenPos();
    renderInfo->winSize = ImGui::GetWindowSize();
    renderInfo->drawList = ImGui::GetWindowDrawList();

    isWindowFocused = ImGui::IsWindowFocused();

    bool& showCreateMenu = this->nodeViewer->showCreateMenu;

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

            if (ui->selectedNodeView == nodeView)
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

                startRealPosition = CalculateNodeOutputPosition(renderInfo, output, xPos, yPos, xSize, ySize);
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

void UI::NodeViewerRenderer::RenderCreateNodeMenu()
{
    auto& nodeMaps = nodeViewer->nodeMaps;
    auto& nodeViews = nodeViewer->graph->nodeViews;
    auto& ui = nodeViewer->ui;
    auto& renderInfo = nodeViewer->graph->renderInfo;

    if(ui->selectedLayer && ImGui::BeginPopup("CreateNodeMenu"))
    {
        for (std::shared_ptr<NodeMap> nodeMap : nodeMaps)
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