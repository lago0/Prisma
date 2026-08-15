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

inline Color4 AlphaBlendPixel(Color4 one, Color4 two)
{
    float oneR = one.r / 255.0f;
    float oneG = one.g / 255.0f;
    float oneB = one.b / 255.0f;
    float oneA = one.a / 255.0f;

    float twoR = two.r / 255.0f;
    float twoG = two.g / 255.0f;
    float twoB = two.b / 255.0f;
    float twoA = two.a / 255.0f;

    float AOut = twoA + oneA * (1.0 - twoA);
    float ROut = ((twoR * twoA) + (oneR * oneA * (1.0 - twoA))) / AOut;
    float GOut = ((twoG * twoA) + (oneG * oneA * (1.0 - twoA))) / AOut;
    float BOut = ((twoB * twoA) + (oneB * oneA * (1.0 - twoA))) / AOut;

    Color4 color;
    color.r = ROut * 255.0f;
    color.g = GOut * 255.0f;
    color.b = BOut * 255.0f;
    color.a = AOut * 255.0f;

    return color;
}