#pragma once

class Buffer4;
class LayerManager;
class Compositor;

class Core
{
    public:
        Core();
        ~Core();
        void Start();

        Buffer4* GetOutput();

        Compositor* GetCompositor();

        LayerManager* layerManager;
        Compositor* compositor;
};