#pragma once

#include <string>
#include <vector>

#include "Layer.h"
#include "util/signal/Signal.h"

class LayerManager {
    public:
        LayerManager(Core* core);
        ~LayerManager();

        Layer* GetLayerByIndex(int index);
        Layer* GetLayerByName(std::string name);
        int GetIndexByLayer(Layer* layer);

        Layer* CreateLayer();

        void SwapLayerIndexes(int index1, int index2);

        void DeleteLayer(int index);

        Core* GetCore();

        std::string ChooseInitialLayerName();

        Util::Signal<Layer*>* onLayerCreated;
        Util::Signal<Layer*, LayerUpdateType>* onLayerUpdated;
        Util::Signal<int>* onLayerDeleted;

        std::vector<Layer*> layers;

        Core* core;
};