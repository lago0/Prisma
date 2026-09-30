#pragma once

#include <imgui.h>
#include <memory>

class Node;

namespace UI
{
    class RenderInfo;
    class NodeView;

    float Distance(ImVec2 p1, ImVec2 p2);

    bool PointInBounds(ImVec2 startPos, ImVec2 endPos, ImVec2 point);

    float CalculateNodeXPosition(std::unique_ptr<RenderInfo>& renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView);
    float CalculateNodeXPositionInverse(std::unique_ptr<RenderInfo>& renderInfo, float posX);

    float CalculateNodeYPosition(std::unique_ptr<RenderInfo>& renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView);
    float CalculateNodeYPositionInverse(std::unique_ptr<RenderInfo>& renderInfo, float posY);

    float CalculateNodeXSize(std::unique_ptr<RenderInfo>& renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView);
    float CalculateNodeYSize(std::unique_ptr<RenderInfo>& renderInfo, const Node* node, const std::shared_ptr<NodeView> nodeView);

    ImVec2 CalculateNodeInputPosition(std::unique_ptr<RenderInfo>& renderInfo, const Node*, int xPos, int yPos, int index);
    ImVec2 CalculateNodeOutputPosition(std::unique_ptr<RenderInfo>& renderInfo, const Node*, int xPos, int yPos, int xSize, int ySize);

    bool IsDragging(std::unique_ptr<RenderInfo>& renderInfo);

    void DrawNodeBezier(std::unique_ptr<RenderInfo>& renderInfo, ImDrawList* drawList, ImVec2 p1, ImVec2 p4);

    bool GetIfNodeInputBeingClicked(std::unique_ptr<RenderInfo>& renderInfo, Node* node, std::shared_ptr<NodeView> nodeView, int index);
    bool GetIfNodeOutputBeingClicked(std::unique_ptr<RenderInfo>& renderInfo, Node* node, std::shared_ptr<NodeView> nodeView);
}