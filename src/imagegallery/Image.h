#pragma once

#include <string>

#include "util/Buffer.h"

/**
 * @class Image
 * @brief Singular external image handling and decompression.
 * @param id Id assigned to the image (SHOULD BE DECIDED BY IMAGEGALLERY).
 * @param name Name assigned to the image.
 * @param path Path of the external image.
 * @author lago0
 */
class Image
{
    public:
        Image(int id, std::string name, std::string path);
        ~Image();

        /**
         * @brief Reloads image data (when external image changes).
         */
        void ReloadBuffer();

        /**
         * @brief Gets the current external buffer in memory.
         * @returns Buffer4* with the data.
         */
        Buffer4* GetBuffer();

        int id;
        std::string name;
        std::string path;

        Buffer4* currentBuffer = nullptr;
};