#pragma once

#include <memory>
#include <map>

class Layer;
class Node;

namespace UI 
{
    class NodeViewer;
    class NodeView;
    class RenderInfo;

    using NodeList = std::map<Node*, std::shared_ptr<NodeView>>;

    /**
     * @class NodeViewerGraph
     * @brief Graph manager for NodeViewer
     * @param nodeViewer Pointer to the NodeViewer instance
     * @author lago0
     */
    class NodeViewerGraph
    {
        public:
            NodeViewerGraph(NodeViewer* nodeViewer);
            ~NodeViewerGraph();
            
            void OnLayerCreatedCallback(Layer*);
            void OnNodeCreatedCallback(Layer*, Node*);

            void OnLayerDeletedCallback(Layer* oldLayerPointer);
            void OnNodeDeletedCallback(Layer* layer, Node* oldNodePointer);
        
            std::map<Layer*, NodeList> nodeViews;

            std::unique_ptr<RenderInfo> renderInfo;

            NodeViewer* nodeViewer;
    };
}