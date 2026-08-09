#pragma once

#include <vector>
#include <stdint.h>
#include <iostream>

#include "Color.h"

class Buffer3
{
    public:
        Buffer3(int width, int height) :
            width(width),
            height(height),
            size(width * height)
        {
            pixels.reserve(size);
        };

        Color3 pixelAt(int position)
        {
            return pixels.at(position);
        };

        Color3 pixelAt(int x, int y)
        {
            int position = x + y * width;
            return pixelAt(position);
        };

        int width;
        int height;
        int size;

        std::vector<Color3> pixels;
};

class Buffer4
{
    public:
        Buffer4(int width, int height) :
            width(width),
            height(height),
            size(width * height)
        {
            pixels.resize(size);
        };

        Color4 PixelAt(int position)
        {
            return pixels.at(position);
        };

        Color4 PixelAt(int x, int y)
        {
            int position = x + y * width;
            return PixelAt(position);
        };

        std::vector<uint32_t> ToUint32Buffer()
        {
            auto buffer = std::vector<uint32_t>();

            for (Color4 pixel : pixels)
            {
                buffer.push_back(
                    pixel.b |
                    (pixel.g << 8) |
                    (pixel.r << 16) |
                    (pixel.a << 24)
                );
            }

            return buffer;
        };

        int width;
        int height;
        int size;

        std::vector<Color4> pixels;
};