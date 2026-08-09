#pragma once

#include <vector>

#include "ui/Window.h"

class LayerManager;
class Layer;
enum class LayerUpdateType;

namespace UI
{
    class UI;
    class LayerView;
    
    class LayerWindow : public Window
    {
        public:
            LayerWindow(UI* ui);

            virtual void Update() override;
            virtual void Render() override;

            virtual void OnLayerCreatedCallback(Layer* layer);
            virtual void OnLayerUpdateCallback(Layer* layer, LayerUpdateType layerUpdateType);
            virtual void OnLayerDeletedCallback(int index);

            std::vector<LayerView*> layerViews;

            LayerManager* layerManager;
    };
}