#pragma once

class Buffer4;
class LayerManager;
class Compositor;
class ImageGallery;

/**
 * @class Core
 * @brief Main class of the system
 * @author lago0
 */
class Core
{
    public:
        Core();
        ~Core();

        /**
         * @brief Starts core work.
         * @author lago0
         */
        void Start();

        /**
         * @brief Gets the current compositor.
         * @author lago0
         */
        Compositor* GetCompositor();
        
        LayerManager* layerManager;
        Compositor* compositor;
        ImageGallery* imageGallery;
};