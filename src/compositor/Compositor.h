#pragma once

#include "util/Buffer.h"

class Core;

class Compositor
{
    public:
        Compositor(Core* core);

        Buffer4* GetComposedViewportBuffer();

        void SetDirty();

        Buffer4* composedCachedBuffer = nullptr;
        bool isDirty = true;

        Core* core;
    
    private:
        void ComposeViewportBuffer();
};