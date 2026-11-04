#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <map>

#include "util/Buffer.h"

class Image;
class Core;

/**
 * @class ImageGallery
 * @brief Class for external image handling.
 * @param core A pointer to the main Core instance.
 * @author lago0
 */
class ImageGallery
{
    public:
        ImageGallery(Core* core);
        ~ImageGallery();

        /**
         * @brief Imports an image on a path and sets its name.
         * @param path A string to the external path.
         * @param name The name assigned to the imported name.
         * @author lago0
         */
        std::shared_ptr<Image> ImportImage(std::string path, std::string name);

        /**
         * @brief Imports an image on a path.
         * @param path A string to the external path.
         * @author lago0
         */
        std::shared_ptr<Image> ImportImage(std::string path);

        /**
         * @brief Gets image data by the imported name.
         * @param name Name of the Image.
         * @author lago0
         */
        std::shared_ptr<Image> GetImage(std::string name);

        /**
         * @brief Gets image data by id.
         * @param id Id of the image.
         * @author lago0
         */
        std::shared_ptr<Image> GetImage(int id);

        std::map<int, std::shared_ptr<Image>> images;

        int lastId = 0;

        Core* core;
};