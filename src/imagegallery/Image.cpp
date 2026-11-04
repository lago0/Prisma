#include <opencv2/opencv.hpp>

#include "Image.h"

Image::Image(int id, std::string name, std::string path) :
    id(id),
    name(name),
    path(path)
{
};

Image::~Image()
{
    if (currentBuffer)
    {
        delete currentBuffer;
        currentBuffer = nullptr;
    }
}

void Image::ReloadBuffer()
{
    if (currentBuffer)
    {
        delete currentBuffer;
    }

    cv::Mat img = cv::imread(this->path);

    if (img.empty())
    {
        return;
    }

    cv::Size imgSize = img.size();

    Buffer4* imageBuffer = new Buffer4(imgSize.width, imgSize.height);
    cv::Mat imgMat(imgSize.height, imgSize.width, CV_8UC4, imageBuffer->pixels.data());

    img.copyTo(imgMat);
    
    currentBuffer = imageBuffer;
}

Buffer4* Image::GetBuffer()
{
    return currentBuffer;
}