#pragma once

#include <vector>
#include <map>
#include <memory>
#include <SDL3/SDL.h>

#include "ui/Window.h"

class Node;
class Layer;
class NodeMap;

namespace UI
{
    class UI;
    class NodeView;
    class NodeViewer;
    typedef struct RenderInfo RenderInfo;

    class NodeViewerRenderer;
    class NodeViewerControls;
    class NodeViewerGraph;

    using NodeList = std::map<Node*, std::shared_ptr<NodeView>>;

    class NodeViewer : public Window
    {
        public:
            NodeViewer(UI* ui);
            ~NodeViewer();

            virtual void Update() override;
            virtual void Render() override;

            void OnEvent(SDL_Event* event);

            void SetSelectedNodeView(std::shared_ptr<NodeView> view);
            std::shared_ptr<NodeView> GetSelectedNodeView();

            std::vector<std::shared_ptr<NodeMap>> nodeMaps;

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

            std::unique_ptr<NodeViewerRenderer> renderer;
            std::unique_ptr<NodeViewerControls> controls;
            std::unique_ptr<NodeViewerGraph> graph;
    };
}