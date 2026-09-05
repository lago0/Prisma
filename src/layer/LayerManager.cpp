#include <format>
#include <iostream>

#include "LayerManager.h"

LayerManager::LayerManager(Core* core) :
    core(core),
    onLayerCreated(new Util::Signal<Layer*>()),
    onLayerUpdated(new Util::Signal<Layer*, LayerUpdateType>()),
    onLayerDeleted(new Util::Signal<Layer*>())
{
}

LayerManager::~LayerManager()
{
    for (Layer* layer : layers)
    {
        delete layer;
    }

    delete onLayerCreated, onLayerUpdated, onLayerDeleted;
}

Layer* LayerManager::GetLayerByIndex(int index)
{
    return layers.at(index);
}

Layer* LayerManager::GetLayerByName(std::string name)
{
    for (Layer* layer : layers) {
        if (layer->GetName() == name) {
            return layer;
        }
    }

    return nullptr;
}

int LayerManager::GetIndexByLayer(Layer* layer)
{
    for (int i = 0; i < layers.size(); i++)
    {
        if (layers.at(i) == layer)
        {
            return i;
        }
    }

    return -1;
}

Layer* LayerManager::CreateLayer()
{
    int index = layers.size();
    std::string name = ChooseInitialLayerName();

    Layer* layer = new Layer(this, index, name);
    layers.push_back(layer);

    layer->onLayerUpdated->Connect([layer, this](LayerUpdateType type)
    {
        this->onLayerUpdated->Emit(layer, type);
    });
    
    onLayerCreated->Emit(layer);

    layer->nodeWorkspace->SetDirty();

    return layer;
}

void LayerManager::SwapLayerIndexes(int index1, int index2)
{
    Layer* layer1 = layers.at(index1);
    Layer* layer2 = layers.at(index2);

    if (layer1 && layer2) 
    {
        layers[index2] = layer1;
        layer1->SetIndex(index2);

        layers[index1] = layer2;
        layer2->SetIndex(index1);
    }
}

void LayerManager::DeleteLayer(int index)
{
    Layer* layer = layers.at(index);
    layers.erase(layers.begin() + index);
  
    delete layer;

    for (int i = 0; i < layers.size(); i++) {
        layers[i]->SetIndex(i);
    }
    
    onLayerDeleted->Emit(layer);
}

std::string LayerManager::ChooseInitialLayerName() 
{
    int number = 1;

    while (1)
    {
        std::string current = std::format("New Layer {}", number);

        if (!GetLayerByName(current))
        {
            return current;
        }

        number++;
    }
}

Core* LayerManager::GetCore()
{
    return core;
}