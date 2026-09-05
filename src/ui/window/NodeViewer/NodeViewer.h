#pragma once

#include <vector>
#include <map>
#include <memory>

#include "ui/Window.h"

class Node;
class NodeView;
class Layer;
class NodeMap;

using NodeList = std::map<Node*, std::shared_ptr<NodeView>>;

namespace UI
{
    class UI;

    class NodeViewer;

    typedef struct
    {
        ImVec2 winPos;
        ImVec2 winSize;
        ImDrawList* drawList;
        NodeViewer* window;
    } RenderInfo;

    class NodeViewer : public Window
    {
        public:
            NodeViewer(UI* ui);
            ~NodeViewer();

            virtual void Update() override;
            virtual void Render() override;

            void RenderCreateNodeMenu();

            void OnEvent(SDL_Event* event);

            void OnLayerCreatedCallback(Layer*);
            void OnNodeCreatedCallback(Layer*, Node*);

            void OnLayerDeletedCallback(Layer* oldLayerPointer);
            void OnNodeDeletedCallback(Layer* layer, Node* oldNodePointer);

            void SetSelectedNodeView(std::shared_ptr<NodeView> view);
            std::shared_ptr<NodeView> GetSelectedNodeView();

            std::map<Layer*, NodeList> nodeViews;

            std::vector<NodeMap*> nodeMaps;

            bool showCreateMenu = false;

            float currentViewPosX = 0;
            float currentViewPosY = 0;

            float zoom = 1.0;

            bool dragging = false;
            bool draggingNode = false;
            bool draggingConnection = false;

            float draggingPosOffsetX = 0.0;
            float draggingPosOffsetY = 0.0;

            float draggingStartPosX = 0.0;
            float draggingStartPosY = 0.0;

            int draggingConnectionType = 0; // 0: Output, 1: Input
            void* draggingConnectionEntity = nullptr;

            RenderInfo* renderInfo;

            std::shared_ptr<NodeView> selectedNodeView = nullptr;

            bool isWindowFocused = false;
    };
}