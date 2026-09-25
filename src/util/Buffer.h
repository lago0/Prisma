#pragma once

#include <vector>
#include <stdint.h>
#include <iostream>

#include "Color.h"

/**
 * @class Buffer3
 * @brief Buffer with three colors
 * @param width Width of the buffer
 * @param height Height of the buffer
 * @author lago0
 */
class Buffer3
{
    public:
        Buffer3(size_t width, size_t height) :
            width(width),
            height(height),
            size(width * height)
        {
            pixels.resize(size);
        };

        /**
         * @brief Gets a pixel on the screen
         * @param position Number of pixel
         * @returns Color3 object with the color on the specified pixel
         * @author lago0
         */
        Color3 pixelAt(int position)
        {
            return pixels.at(position);
        };

        /**
         * @brief Gets a pixel on the screen
         * @param x X position on the screen
         * @param y Y position on the screen
         * @returns Color3 object with the color on the specified pixel
         * @author lago0
         */
        Color3 pixelAt(int x, int y)
        {
            int position = x + y * width;
            return pixelAt(position);
        };

        /**
         * @brief Converts the buffer into a uint32_t vector
         * @returns Vector with colors on uint32_t in structure RGB
         * @author lago0
         */
        std::vector<uint32_t> ToUint32Buffer()
        {
            auto buffer = std::vector<uint32_t>();
            buffer.resize(pixels.size());

            for (int i = 0; i < pixels.size(); i++)
            {
                Color3 pixel = pixels[i];
                buffer[i] = pixel.b | (pixel.g << 8) | (pixel.r << 16);
            }

            return buffer;
        };

        size_t width;
        size_t height;
        size_t size;

        std::vector<Color3> pixels;
};

/**
 * @class Buffer4
 * @brief Buffer with four colors
 * @param width Width of the buffer
 * @param height Height of the buffer
 * @author lago0
 */
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

        /**
         * @brief Gets a pixel on the screen
         * @param position Number of pixel
         * @returns Color4 object with the color on the specified pixel
         * @author lago0
         */
        Color4 PixelAt(int position)
        {
            return pixels.at(position);
        };

        /**
         * @brief Gets a pixel on the screen
         * @param x X position on the screen
         * @param y Y position on the screen
         * @returns Color4 object with the color on the specified pixel
         * @author lago0
         */
        Color4 PixelAt(int x, int y)
        {
            int position = x + y * width;
            return PixelAt(position);   
        };

        /**
         * @brief Converts the buffer into a uint32_t vector
         * @returns Vector with colors on uint32_t in structure ARGB
         * @author lago0
         */
        std::vector<uint32_t> ToUint32Buffer()
        {
            auto buffer = std::vector<uint32_t>();
            buffer.resize(size);

            for (int i = 0; i < size; i++)
            {
                Color4 pixel = pixels[i];
                buffer[i] = pixel.b | (pixel.g << 8) | (pixel.r << 16) | (pixel.a << 24);
            }

            return buffer;
        };

        int width;
        int height;
        int size;

        std::vector<Color4> pixels;
};

/**
 * @brief Composes a Buffer4 on top of another Buffer4. The buffer will be composed on top of the base buffer.
 * @param base Buffer* that is going to be the back of the image
 * @param other Buffer* that is going to be the top of the image
 * @author lago0
 */
inline void ComposeBuffer4OnBuffer4(Buffer4* base, Buffer4* other)
{
    int sizeX = base->width;
    int sizeY = base->height;

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            int i = x + y * sizeX;
            base->pixels[i] = AlphaBlendPixel(base->PixelAt(i), other->PixelAt(i));
        }
    }
}