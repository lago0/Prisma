#pragma once

#include "util/Buffer.h"

class Core;

/** 
 * @class Compositor
 * @brief Class for compositing Layer results into one image
 * @param core A pointer to the Core that contains the layer
 * @author lago0
*/
class Compositor
{
    public:
        Compositor(Core* core);
        ~Compositor();

        /**
         * @brief Gets the cached composed viewport buffer or if it is dirty recomposes it
         * @returns Buffer4* that contains the pixels
         * @author lago0
         */
        Buffer4* GetComposedViewportBuffer();

        /**
         * @brief Sets the buffer as dirty 
         * @author lago0
         */
        void SetDirty();

        Buffer4* composedCachedBuffer = nullptr;
        bool isDirty = true;

        Core* core;
    
    private:
        /**
         * @brief Composes the viewport buffer
         * @author lago0
         */
        void ComposeViewportBuffer();
};