///////////////////////////////////////////////////////////////////////////////
// imagealgth.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2013/6/8

#ifndef __IMAGEALGTH_H__
#define __IMAGEALGTH_H__

#include <math.h>
#include "gdiutil.h"
#include "memutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// ColorF
//
// Global functions in this file:
//
// blurBitmap()
// mosaicBitmap()
// handleBitmap<THandler>()

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Interface of the ColorF class
//

class ColorF
{
// Constructors
public:
    ColorF(float r, float g, float b);

// Operations
public:
    template <typename _Ty>
    void calculate(const _Ty& color, float matrix);

// Data members
public:
    float red;
    float blue;
    float green;
};


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

__STATIC_INLINE__ int32_t clamp(int32_t current, int32_t offset, int32_t length)
{
    const int32_t index = current + offset;
    return (index < 0 ? -current : (index >= length ? length - current - 1 : offset));
}

__STATIC_INLINE__ uint8_t computeThreshold(uint32_t (&histData)[256], uint32_t count)
{
    // Otsu Threshold algorithm - http://www.labbookpages.co.uk
    float sum = 0;
    for (uint32_t i = 0; i < _countof(histData); ++i)
        sum += i * histData[i];

    float sumB = 0, maxBetween = 0;
    uint32_t threshold = 0, wB = 0, wF = 0;

    for (uint32_t i = 0; i < _countof(histData); ++i)
    {
        wB += histData[i];              // Weight Background
        if (wB == 0) continue;

        wF = count - wB;                // Weight Foreground
        if (wF == 0) break;

        sumB += i * histData[i];
        const float mB = sumB / wB;             // Mean Background
        const float mF = (sum - sumB) / wF;     // Mean Foreground

        // Calculate Between Class Variance
        const float between = (float)wB * (float)wF * (mB - mF) * (mB - mF);

        // Check if new maximum found
        if (between > maxBetween)
        {
            threshold  = i;
            maxBetween = between;
        }
    }

    return threshold;
}

template <typename THandler>
__STATIC_INLINE__ void handleBitmap(Color* colors, uint32_t count, THandler handler)
{
    assert(colors);

    for (uint32_t i = 0; i < count; ++i)
        handler(colors[i]);
}

__STATIC_INLINE__ void blurBitmap(Color* colors, ColorF* colorsF, int32_t width, int32_t height, int32_t radius, float* matrix)
{
    assert(colors);
    assert(colorsF);

    for (int32_t y = 0, index = 0; y < height; ++y)
    {
        for (int32_t x = 0; x < width; ++x, ++index)
        {
            for (int32_t i = 0, j = -radius; j <= radius; ++i, ++j)
                colorsF[index].calculate(colors[index + clamp(x, j, width)], matrix[i]);
        }
    }

    for (int32_t x = 0, index = 0; x < width; ++x)
    {
        for (int32_t y = 0; y < height; ++y)
        {
            index = y * width + x;
            ColorF color(0.0, 0.0, 0.0);
            for (int32_t i = 0, j = -radius; j <= radius; ++i, ++j)
                color.calculate(colorsF[index + clamp(y, j, height) * width], matrix[i]);

            colors[index].setRGB((uint8_t)color.red, (uint8_t)color.green, (uint8_t)color.blue);
        }
    }
}

__STATIC_INLINE__ void mosaicBitmap(Color* colors, uint32_t row, uint32_t col, uint32_t width, uint32_t mosaicWidth, uint32_t mosaicHeight)
{
    assert(colors);

    uint32_t red = 0, green = 0, blue = 0, count = mosaicWidth * mosaicHeight;
    for (uint32_t i = 0; i < mosaicHeight; ++i)
    {
        for (uint32_t j = 0; j < mosaicWidth; ++j)
        {
            const Color& color = colors[(row + i) * width + col + j];
            red   += color.red;
            green += color.green;
            blue  += color.blue;
        }
    }

    red /= count, green /= count, blue /= count;
    for (uint32_t i = 0; i < mosaicHeight; ++i)
    {
        for (uint32_t j = 0; j < mosaicWidth; ++j)
            colors[(row + i) * width + col + j].setRGB((uint8_t)red, (uint8_t)green, (uint8_t)blue);
    }
}

__END_NAMESPACE

#include "imagealgth.inl"

#endif  // __IMAGEALGTH_H__
