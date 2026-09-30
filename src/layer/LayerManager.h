#pragma once

#include <string>
#include <vector>

#include "Layer.h"
#include "util/signal/Signal.h"

class LayerManager {
    public:
        LayerManager(Core* core);
        ~LayerManager();

        /**
         * @brief Gets a layer by its index.
         * @param index Index of the queried layer
         * @returns Layer* of the layer.
         * @author lago0
         */
        Layer* GetLayerByIndex(int index);

        /**
         * @brief Gets a layer by its name.
         * @param name Name of the queried layer
         * @returns Layer* of the layer.
         * @author lago0
         */
        Layer* GetLayerByName(std::string name);

        /**
         * @brief Gets the index of a layer.
         * @param layer The layer that you want to get its index
         * @returns int
         * @author lago0
         */
        int GetIndexByLayer(Layer* layer);

        /**
         * @brief Starts core work.
         * @returns Layer* with the created layer.
         * @author lago0
         */
        Layer* CreateLayer();

        /**
         * @brief Swaps two layer indexes.
         * @param index1 The layer that will be swapped with index1
         * @param index2 The layer that will be swapped with index2
         * @author lago0
         */
        void SwapLayerIndexes(int index1, int index2);

        /**
         * @brief Deletes a layer.
         * @param index The index of the layer
         * @author lago0
         */
        void DeleteLayer(int index);

        /**
         * @brief Gets core instance.
         * @returns Core* with the core's current instance
         * @author lago0
         */
        Core* GetCore();

        /**
         * @brief Chooses an initial layer name.
         * @returns std::string with the name
         * @author lago0
         */
        std::string ChooseInitialLayerName();

        Util::Signal<Layer*>* onLayerCreated;
        Util::Signal<Layer*, LayerUpdateType>* onLayerUpdated;
        Util::Signal<Layer*>* onLayerDeleted;

        std::vector<Layer*> layers;

        Core* core;
};