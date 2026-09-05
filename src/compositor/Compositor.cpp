#include <iostream>

#include "Compositor.h"
#include "core/Core.h"
#include "layer/LayerManager.h"

Compositor::Compositor(Core* core) :
    core(core)
{
}

Buffer4* Compositor::GetComposedViewportBuffer()
{
    if (isDirty || composedCachedBuffer == nullptr)
    {
        ComposeViewportBuffer();
    }
    return composedCachedBuffer;
}

void Compositor::SetDirty()
{
    std::cout << "compositor dirty\n";
    isDirty = true;
}

void Compositor::ComposeViewportBuffer()
{
    std::cout << "1\n";
    if (composedCachedBuffer != nullptr)
    {
        delete composedCachedBuffer;
    }

    Buffer4* buffer = new Buffer4(1920, 1080);

    int layerNumber = core->layerManager->layers.size();

    for (int i = layerNumber - 1; i >= 0; i--)
    {
        Buffer4* layerBuffer = core->layerManager->layers[i]->GetViewportOutput();

        if (layerBuffer != nullptr)
        {
            ComposeBuffer4OnBuffer4(buffer, layerBuffer);
        }
    }

    composedCachedBuffer = buffer;

    isDirty = false;
}