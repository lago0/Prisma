#pragma once

#include <imgui.h>
#include <SDL3/SDL.h>
#include <memory>

namespace UI
{
    class NodeViewer;

    typedef struct RenderInfo RenderInfo;

    /**
     * @class NodeViewerController
     * @brief Event proccesor for NodeViewer
     * @param nodeViewer Pointer to the NodeViewer instance
     * @author lago0
     */
    class NodeViewerControls
    {
        public:
            NodeViewerControls(NodeViewer* nodeViewer);
            ~NodeViewerControls();

            void OnEvent(SDL_Event* event);

            NodeViewer* nodeViewer;
    };
}