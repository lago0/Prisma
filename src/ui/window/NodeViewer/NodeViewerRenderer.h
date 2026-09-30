#pragma once

namespace UI
{
    class NodeViewer;

    /**
     * @class NodeViewerRenderer
     * @brief Renders the NodeViewer graph
     * @param nodeViewer Pointer to the NodeViewer instance
     * @author lago0
     */
    class NodeViewerRenderer
    {
        public:
            NodeViewerRenderer(NodeViewer* nodeViewer);

            void Render();
            void RenderCreateNodeMenu();

            NodeViewer* nodeViewer;

            bool isWindowFocused = false;
    };
}