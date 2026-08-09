#include "Layer.h"
#include "LayerManager.h"

Layer::Layer(LayerManager* layerManager, int index, std::string name) :
    layerManager(layerManager),
    index(index),
    name(name),
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