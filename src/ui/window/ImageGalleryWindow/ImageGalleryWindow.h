#pragma once

#include <vector>

#include "ui/Window.h"

class LayerManager;
class Layer;
class Core;
enum class LayerUpdateType;

namespace UI
{
    class UI;
    class LayerView;
    
    class ImageGalleryWindow : public Window
    {
        public:
            ImageGalleryWindow(UI* ui);

            virtual void Update() override;
            virtual void Render() override;

            Core* core;
    };
}