///////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2013/6/8

#include "imagealgth.h"

__BEGIN_DECLS
///////////////////////////////////////////////////////////////////////////////
// Implementation of the Android_grayBitmap
//

STDCEXPORT void Android_grayBitmap(void* pixels, uint32_t width, uint32_t height)
{
    assert(pixels);
    __NS::handleBitmap((__NS::Color*)pixels, width, height, [](__NS::Color& color) { color.red = color.green = color.blue = (uint8_t)(0.299f * color.red + 0.587f * color.green + 0.114f * color.blue); });
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the Android_inverseBitmap
//

STDCEXPORT void Android_inverseBitmap(void* pixels, uint32_t width, uint32_t height)
{
    assert(pixels);
    __NS::handleBitmap((__NS::Color*)pixels, width, height, [](__NS::Color& color) { color.setRGB(255 - color.red, 255 - color.green, 255 - color.blue); });
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the Android_blurBitmap
//

STDCEXPORT void Android_blurBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t radius)
{
    assert(pixels);

    if (radius > 1)
    {
        static const double SQRT2PI = ::sqrt(M_PI * 2.0);
        const double sigma  = (double)radius / 3.0;
        const double sigma2 = 2.0 * sigma * sigma;
        const double sigmap = sigma * SQRT2PI;

        // Builds the gaussian matrix from radius.
        float matrix[radius * 2 + 1];
        for (int32_t i = 0, j = -(int32_t)radius; j <= (int32_t)radius; ++j, ++i)
            matrix[i] = ::exp(-(double)(j * j) / sigma2) / sigmap;

        stdutil::heap_buffer<__NS::ColorF> buffer;
        const uint32_t size = sizeof(__NS::ColorF) * width * height;
        if (__NS::ColorF* colorsF = buffer.allocateBytes(size))
        {
            ::memset(colorsF, 0, size);
            __NS::blurBitmap((__NS::Color*)pixels, colorsF, (int32_t)width, (int32_t)height, radius, matrix);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the Android_binaryBitmap
//

STDCEXPORT void Android_binaryBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t gray)
{
    assert(pixels);

    if (!gray)
        Android_grayBitmap(pixels, width, height);

    const __NS::Color* colors = (__NS::Color*)pixels;
    const uint32_t count = width * height;

    // Calculate histogram data.
    uint32_t histData[256] = { 0 };
    for (uint32_t i = 0; i < count; ++i)
        histData[colors[i].green]++;

    // Calculate threshold.
    const uint8_t threshold = __NS::computeThreshold(histData, count);

    // Calculate binary colors
    __NS::handleBitmap((__NS::Color*)pixels, width, height, [threshold](__NS::Color& color) { color = (color.green >= threshold ? __NS::Color::WHITE : __NS::Color::BLACK); });
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the Android_mirrorBitmap
//

STDCEXPORT void Android_mirrorBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t horizontal)
{
    __NS::Color* colors = (__NS::Color*)pixels;
    assert(colors);

    if (horizontal)
    {
        // Mirror horizontally
        for (uint32_t i = 0; i < height; ++i)
        {
            for (uint32_t start = 0, end = width - 1; start < end; ++start, --end)
                colors[start].swap(colors[end]);

            // Move to next line.
            colors += width;
        }
    }
    else
    {
        // Mirror vertically
        for (uint32_t start = 0, end = height - 1, startOffset = 0, endOffset = 0; start < end; ++start, --end)
        {
            startOffset = start * width, endOffset = end * width;
            for (uint32_t i = 0; i < width; ++i)
                colors[i + startOffset].swap(colors[i + endOffset]);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the Android_spreadBitmap
//

STDCEXPORT void Android_spreadBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t spreadSize)
{
    __NS::Color* colors = (__NS::Color*)pixels;
    assert(colors);

    if (spreadSize > 0)
    {
        ::srand(::time(NULL));
        spreadSize = ::__Min(spreadSize, ::__Min(width, height));

        for (uint32_t i = 0, row = 0, col = 0; i < height; ++i)
        {
            for (uint32_t j = 0; j < width; ++j)
            {
                do
                {
                    // Randoms row, col around spread size.
                    row = i + (::rand() % spreadSize);
                    col = j + (::rand() % spreadSize);
                } while (row >= height || col >= width);

                colors[i * width + j] = colors[row * width + col];
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the Android_mosaicBitmap
//

STDCEXPORT void Android_mosaicBitmap(void* pixels, uint32_t width, uint32_t height, uint32_t mosaicSize)
{
    __NS::Color* colors = (__NS::Color*)pixels;
    assert(colors);

    if (mosaicSize > 1)
    {
        mosaicSize = ::__Min(mosaicSize, ::__Min(width, height));
        for (uint32_t i = 0; i < height; i += mosaicSize)
        {
            for (uint32_t j = 0; j < width; j += mosaicSize)
                __NS::mosaicBitmap(colors, i, j, width, ::__Min(width  - j, mosaicSize), ::__Min(height - i, mosaicSize));
        }
    }
}
__END_DECLS