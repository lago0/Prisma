#pragma once

#include <string>

#include "node/NodeWorkspace.h"
#include "util/signal/Signal.h"

class LayerManager;

enum class LayerUpdateType
{
    INDEX_CHANGE,
    NAME_CHANGE
};

class Layer {
    public:
        Layer(LayerManager* layerManager, int index, std::string name);
        ~Layer();

        void SetIndex(int new_index);
        int GetIndex() const;

        void SetName(std::string new_name);
        std::string GetName() const;

        Buffer4* GetViewportOutput();

        NodeWorkspace* nodeWorkspace;

        Util::Signal<LayerUpdateType>* onLayerUpdated;

    private:
        int index;
        std::string name;

        LayerManager* layerManager;
};