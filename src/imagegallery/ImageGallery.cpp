#include "ImageGallery.h"
#include "Image.h"
#include "core/Core.h"

ImageGallery::ImageGallery(Core* core) :
    core(core)
{
}

ImageGallery::~ImageGallery()
{
}

std::shared_ptr<Image> ImageGallery::ImportImage(std::string path, std::string name)
{
    int id = lastId;
    lastId++;

    auto image = std::make_shared<Image>(id, path, name);
    image->ReloadBuffer();

    images[id] = image;

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
}

std::shared_ptr<Image> ImageGallery::GetImage(int id)
{
    return images.at(id);
}
