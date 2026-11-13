#include "ImageGallery.h"
#include "Image.h"
#include "core/Core.h"

#include <iostream>

ImageGallery::ImageGallery(Core* core) :
    core(core)
{
}

ImageGallery::~ImageGallery()
{
}

std::shared_ptr<Image> ImageGallery::ImportImage(std::string path, std::string name)
{
    std::cout << path << std::endl;

    auto image = std::make_shared<Image>(lastId, name, path);
    image->ReloadBuffer();

    images[lastId] = image;

    lastId++;

    return image;
}

std::shared_ptr<Image> ImageGallery::ImportImage(std::string path)
{
    return ImportImage(path, path);
}

std::shared_ptr<Image> ImageGallery::GetImage(std::string name)
{
    for (auto pair : images)
    {
        if (pair.second->name == name)
        {
            return pair.second;
        }
    }
    return nullptr;
}

std::shared_ptr<Image> ImageGallery::GetImage(int id)
{
    return images.at(id);
}

std::map<int, std::shared_ptr<Image>>& ImageGallery::GetImages()
{
    return images;
}
