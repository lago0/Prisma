#include <math.h>

#include "NodeViewerUtils.h"
#include "RenderInfo.h"
#include "NodeViewer.h"
#include "NodeView.h"
#include "node/Node.h"

float UI::Distance(ImVec2 p1, ImVec2 p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

bool UI::PointInBounds(ImVec2 startPos, ImVec2 endPos, ImVec2 point)
{
    return startPos.x < point.x && startPos.y < point.y
                && endPos.x > point.x && endPos.y > point.y;
}

float UI::CalculateNodeXPosition(std::unique_ptr<RenderInfo>& renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView)
{
    return renderInfo->winPos.x + (nodeView->x * renderInfo->window->zoom) + renderInfo->winSize.x / 2 - renderInfo->window->currentViewPosX * renderInfo->window->zoom;
}

float UI::CalculateNodeXPositionInverse(std::unique_ptr<RenderInfo>& renderInfo, float posX)
{
    return ((posX - renderInfo->winPos.x - (renderInfo->winSize.x / 2.0f)) / renderInfo->window->zoom) + renderInfo->window->currentViewPosX;
}

float UI::CalculateNodeYPosition(std::unique_ptr<RenderInfo>& renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView)
{
    return renderInfo->winPos.y + (nodeView->y * renderInfo->window->zoom) + renderInfo->winSize.y / 2 - renderInfo->window->currentViewPosY * renderInfo->window->zoom;
}

float UI::CalculateNodeYPositionInverse(std::unique_ptr<RenderInfo>& renderInfo, float posY)
{
    return ((posY - renderInfo->winPos.y - (renderInfo->winSize.y / 2.0f)) / renderInfo->window->zoom) + renderInfo->window->currentViewPosY;
}

float UI::CalculateNodeXSize(std::unique_ptr<RenderInfo>& renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView)
{
    return 200 * renderInfo->window->zoom;
}

float UI::CalculateNodeYSize(std::unique_ptr<RenderInfo>& renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView)
{
    return (40 + 20 * (node->nodeInputs.size() > 0 ? node->nodeInputs.size() - 1 : 0)) * renderInfo->window->zoom;
}

ImVec2 UI::CalculateNodeInputPosition(std::unique_ptr<RenderInfo>& renderInfo, const Node*, int xPos, int yPos, int index)
{
    return ImVec2(xPos, yPos + 20 * renderInfo->window->zoom + 20 * renderInfo->window->zoom * index);
}

ImVec2 UI::CalculateNodeOutputPosition(std::unique_ptr<RenderInfo>& renderInfo, const Node*, int xPos, int yPos, int xSize, int ySize)
{
    return ImVec2(xPos + xSize, yPos + ySize / 2);
}

bool UI::IsDragging(std::unique_ptr<RenderInfo>& renderInfo)
{
    return renderInfo->window->dragging || renderInfo->window->draggingNode || renderInfo->window->draggingConnection;
}

void UI::DrawNodeBezier(std::unique_ptr<RenderInfo>& renderInfo, ImDrawList* drawList, ImVec2 p1, ImVec2 p4)
{
    float dx = std::abs(p4.x - p1.x);
    float dy = std::abs(p4.y - p1.y);
    
    float minOffset = std::max(30.0f, dy * 0.3f);
    
    float handleOffset = std::max(dx * 0.5f, minOffset);

    ImVec2 p2 = ImVec2(p1.x + handleOffset, p1.y);
    ImVec2 p3 = ImVec2(p4.x - handleOffset, p4.y);

    drawList->AddBezierCubic(p1, p2, p3, p4, IM_COL32(255, 255, 255, 255), 1.0 * renderInfo->window->zoom, 20 * renderInfo->window->zoom);
}

bool UI::GetIfNodeInputBeingClicked(std::unique_ptr<RenderInfo>& renderInfo, Node* node, std::shared_ptr<NodeView> nodeView, int index)
{
    float xPos = CalculateNodeXPosition(renderInfo, node, nodeView);
    float yPos = CalculateNodeYPosition(renderInfo, node, nodeView);

    ImVec2 position = CalculateNodeInputPosition(renderInfo, node, xPos, yPos, index);
    ImVec2 mousePos = ImGui::GetMousePos();

    return Distance(position, mousePos) <= 40 * renderInfo->window->zoom;
}

bool UI::GetIfNodeOutputBeingClicked(std::unique_ptr<RenderInfo>& renderInfo, Node* node, std::shared_ptr<NodeView> nodeView)
{
    float xPos = CalculateNodeXPosition(renderInfo, node, nodeView);
    float yPos = CalculateNodeYPosition(renderInfo, node, nodeView);

    float xSize = CalculateNodeXSize(renderInfo, node, nodeView);
    float ySize = CalculateNodeYSize(renderInfo, node, nodeView);

    ImVec2 position = CalculateNodeOutputPosition(renderInfo, node, xPos, yPos, xSize, ySize);
    ImVec2 mousePos = ImGui::GetMousePos();

    return Distance(position, mousePos) <= 40 * renderInfo->window->zoom;
}