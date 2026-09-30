#include "NodeViewerGraph.h"
#include "NodeViewer.h"
#include "RenderInfo.h"
#include "layer/Layer.h"
#include "node/Node.h"
#include "ui/UI.h"
#include "node/nodes/ImageOut.h"
#include "layer/LayerManager.h"
#include "NodeView.h"

UI::NodeViewerGraph::NodeViewerGraph(NodeViewer* nodeViewer) :
    nodeViewer(nodeViewer),
    renderInfo(std::make_unique<RenderInfo>())
{
    renderInfo->window = nodeViewer;

    for (Layer* layer : nodeViewer->ui->GetCore()->layerManager->layers)
    {
        this->OnLayerCreatedCallback(layer);
    }

    nodeViewer->ui->GetCore()->layerManager->onLayerCreated->Connect([this](Layer* layer)
    {
        this->OnLayerCreatedCallback(layer);
    });

    nodeViewer->ui->GetCore()->layerManager->onLayerDeleted->Connect([this](Layer* layer)
    {
        this->OnLayerDeletedCallback(layer);
    });
}

UI::NodeViewerGraph::~NodeViewerGraph()
{
}

void UI::NodeViewerGraph::OnLayerCreatedCallback(Layer* layer)
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

void UI::NodeViewerGraph::OnNodeCreatedCallback(Layer* layer, Node* node)
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

void UI::NodeViewerGraph::OnLayerDeletedCallback(Layer* oldLayerPointer)
{
    std::cout << "Layer deleted. Old pointer: " << oldLayerPointer << std::endl;
    if (nodeViews.contains(oldLayerPointer))
    {
        nodeViews.erase(oldLayerPointer);

        if (nodeViewer->ui->selectedLayer == oldLayerPointer)
        {
            nodeViewer->ui->selectedLayer = nullptr;
        }
    }
}

void UI::NodeViewerGraph::OnNodeDeletedCallback(Layer* layer, Node* oldNodePointer)
{
    std::cout << "Node deleted. Layer name: " << layer->GetName() << ". Old pointer: " << oldNodePointer << std::endl;
    if (nodeViews.contains(layer) && nodeViews[layer].contains(oldNodePointer))
    {
        if (nodeViews[layer][oldNodePointer] == nodeViewer->GetSelectedNodeView())
        {
            nodeViewer->SetSelectedNodeView(nullptr);
        }
        nodeViews[layer].erase(oldNodePointer);
    }
}