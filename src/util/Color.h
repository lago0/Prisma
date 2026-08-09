#pragma once

#include <stdint.h>

class Color3
{
    public:
        uint8_t r;
        uint8_t g;
        uint8_t b;

        Color3() :
            r(0),
            g(0),
            b(0)
        {
        }

        Color3(uint8_t r, uint8_t g, uint8_t b) :
            r(r),
            g(g),
            b(b)
        {
        }
};

class Color4
{
    public:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        Color4() :
            r(0),
            g(0),
            b(0),
            a(1)
        {
        }

        Color4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
            r(r),
            g(g),
            b(b),
            a(a)
        {
        }
};