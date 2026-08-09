#pragma once

#include "layer/LayerManager.h"

class Buffer4;

class Core
{
    public:
        Core();
        ~Core();
        void Start();

        Buffer4* GetOutput();

        LayerManager* layerManager;

        bool isViewportDirty = true;
};