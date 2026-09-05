#include "Layer.h"
#include "LayerManager.h"

#include <iostream>

Layer::Layer(LayerManager* layerManager, int index, std::string name) :
    layerManager(layerManager),
    index(index),
    name(name),
    nodeWorkspace(new NodeWorkspace(layerManager->GetCore())),
    onLayerUpdated(new Util::Signal<LayerUpdateType>())
{
}

Layer::~Layer() {
    delete onLayerUpdated;
}

void Layer::SetIndex(int new_index)
{
    index = new_index;
    onLayerUpdated->Emit(LayerUpdateType::INDEX_CHANGE);
}

int Layer::GetIndex() const
{
    return index;
}

void Layer::SetName(std::string new_name)
{
    name = new_name;
    onLayerUpdated->Emit(LayerUpdateType::NAME_CHANGE);
}

std::string Layer::GetName() const
{
    return name;
}

Buffer4* Layer::GetViewportOutput()
{
    std::cout << "2\n";
    return nodeWorkspace->GetViewportOutput();
}