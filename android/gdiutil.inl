///////////////////////////////////////////////////////////////////////////////
// gdiutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2013/11/5

#ifndef __GDIUTIL_INL__
#define __GDIUTIL_INL__

#ifndef __GDIUTIL_H__
    #error gdiutil.inl requires gdiutil.h to be included first
#endif

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Implementation of the Point class
//

__INLINE__ Point::Point(int32_t X/* = 0*/, int32_t Y/* = 0*/)
    : x(X), y(Y)
{
}

__INLINE__ Point::Point(const Point& that)
    : x(that.x), y(that.y)
{
}

__INLINE__ Point::Point(const Point* point)
    : x(point->x), y(point->y)
{
}

__INLINE__ void Point::set(const Point& point)
{
    x = point.x;
    y = point.y;
}

__INLINE__ void Point::set(int32_t X, int32_t Y)
{
    x = X;
    y = Y;
}

__INLINE__ void Point::negate()
{
    x = -x;
    y = -y;
}

__INLINE__ void Point::offset(int dx, int dy)
{
    x += dx;
    y += dy;
}

__INLINE__ Point& Point::operator=(const Point& that)
{
    x = that.x;
    y = that.y;
    return *this;
}

__INLINE__ bool Point::operator==(const Point& point) const
{
    return (x == point.x && y == point.y);
}

__INLINE__ bool Point::operator!=(const Point& point) const
{
    return (x != point.x || y != point.y);
}

#ifndef NDEBUG
__INLINE__ void Point::dump() const
{
    LOGI("Point [ x = %d, y = %d ]\n", x, y);
}
#endif  // NDEBUG


#ifdef ANDROID_RECT_H
///////////////////////////////////////////////////////////////////////////////
// Implementation of the Rect class
//

__INLINE__ Rect::Rect(int32_t l/* = 0*/, int32_t t/* = 0*/, int32_t r/* = 0*/, int32_t b/* = 0*/)
{
    set(l, t, r, b);
}

__INLINE__ Rect::Rect(const ARect& that)
{
    set(that.left, that.top, that.right, that.bottom);
}

__INLINE__ Rect::Rect(const ARect* rect)
{
    assert(rect);
    set(rect->left, rect->top, rect->right, rect->bottom);
}

__INLINE__ Rect::Rect(const Point& topLeft, const Point& bottomRight)
{
    set(topLeft, bottomRight);
}

__INLINE__ Rect::operator ARect*()
{
    return this;
}

__INLINE__ Rect::operator const ARect*() const
{
    return this;
}

__INLINE__ Rect& Rect::operator=(const ARect& that)
{
    set(that.left, that.top, that.right, that.bottom);
    return *this;
}

__INLINE__ bool Rect::operator==(const ARect& rect) const
{
    return (left == rect.left && top == rect.top && right == rect.right && bottom == rect.bottom);
}

__INLINE__ bool Rect::operator!=(const ARect& rect) const
{
    return (left != rect.left || top != rect.top || right != rect.right || bottom != rect.bottom);
}

__INLINE__ void Rect::setEmpty()
{
    left = top = right = bottom = 0;
}

__INLINE__ void Rect::set(int32_t l, int32_t t, int32_t r, int32_t b)
{
    left   = l;
    top    = t;
    right  = r;
    bottom = b;
}

__INLINE__ void Rect::set(const Point& topLeft, const Point& bottomRight)
{
    left   = topLeft.x;
    top    = topLeft.y;
    right  = bottomRight.x;
    bottom = bottomRight.y;
}

__INLINE__ void Rect::setRect(int32_t x, int32_t y, int32_t width, int32_t height)
{
    left   = x;
    top    = y;
    right  = x + width;
    bottom = y + height;
}

__INLINE__ void Rect::offset(const Point& point)
{
    offset(point.x, point.y);
}

__INLINE__ void Rect::offset(int32_t dx, int32_t dy)
{
    left   += dx;
    top    += dy;
    right  += dx;
    bottom += dy;
}

__INLINE__ void Rect::offsetTo(const Point& point)
{
    offsetTo(point.x, point.y);
}

__INLINE__ void Rect::offsetTo(int32_t newLeft, int32_t newTop)
{
    right  += newLeft - left;
    bottom += newTop  - top;
    left    = newLeft;
    top     = newTop;
}

__INLINE__ void Rect::inset(const Point& point)
{
    inset(point.x, point.y, point.x, point.y);
}

__INLINE__ void Rect::inset(int32_t dx, int32_t dy)
{
    inset(dx, dy, dx, dy);
}

__INLINE__ void Rect::inset(int32_t l, int32_t t, int32_t r, int32_t b)
{
    left   += l;
    top    += t;
    right  -= r;
    bottom -= b;
}

__INLINE__ bool Rect::contains(const ARect& rect) const
{
    return contains(rect.left, rect.top, rect.right, rect.bottom);
}

__INLINE__ bool Rect::contains(const Point& point) const
{
    return contains(point.x, point.y);
}

__INLINE__ bool Rect::contains(int32_t x, int32_t y) const
{
    return (left < right && top < bottom && x >= left && x < right && y >= top && y < bottom);
}

__INLINE__ bool Rect::contains(int32_t l, int32_t t, int32_t r, int32_t b) const
{
    return (left < right && top < bottom && left <= l && top <= t && right >= r && bottom >= b);
}

#ifndef NDEBUG
__INLINE__ void Rect::dump() const
{
    LOGI("Rect [ left = %d, top = %d, right = %d, bottom = %d, width = %d, height = %d ]\n", left, top, right, bottom, width(), height());
}
#endif  // NDEBUG

__INLINE__ bool Rect::isEmpty() const
{
    return (left >= right || top >= bottom);
}

__INLINE__ int32_t Rect::width() const
{
    return (right - left);
}

__INLINE__ int32_t Rect::height() const
{
    return (bottom - top);
}

__INLINE__ Point Rect::center() const
{
    return Point(centerX(), centerY());
}

__INLINE__ int32_t Rect::centerX() const
{
    return (left + right) >> 1;
}

__INLINE__ int32_t Rect::centerY() const
{
    return (top + bottom) >> 1;
}

__INLINE__ const Point& Rect::topLeft() const
{
    return *((Point*)this);
}

__INLINE__ const Point& Rect::bottomRight() const
{
    return *((Point*)this + 1);
}
#endif  // ANDROID_RECT_H


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Color class
//

__INLINE__ Color::Color(uint32_t color/* = TRANSPARENT*/)
    : rgba(color)
{
}

__INLINE__ Color::Color(const Color& that)
    : rgba(that.rgba)
{
}

__INLINE__ Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a/* = 0xFF*/)
    : red(r), green(g), blue(b), alpha(a)
{
}

__INLINE__ Color::operator uint32_t() const
{
    return rgba;
}

__INLINE__ Color& Color::operator=(uint32_t color)
{
    rgba = color;
    return *this;
}

__INLINE__ Color& Color::operator=(const Color& that)
{
    rgba = that.rgba;
    return *this;
}

__INLINE__ uint16_t Color::toRGB565() const
{
    return toRGB565(red, green, blue);
}

__INLINE__ void Color::swap(Color& color)
{
    uint32_t c = rgba;
    rgba = color.rgba;
    color.rgba = c;
}

__INLINE__ void Color::setRGB(uint8_t r, uint8_t g, uint8_t b)
{
    red   = r;
    blue  = b;
    green = g;
}

__INLINE__ void Color::set(uint8_t r, uint8_t g, uint8_t b, uint8_t a/* = 0xFF*/)
{
    red   = r;
    blue  = b;
    green = g;
    alpha = a;
}

__INLINE__ Color Color::fromARGB(uint32_t argb)
{
    Color color(argb);
    __swap(color.red, color.blue);
    return color;
}

__INLINE__ Color Color::fromRGB565(uint16_t color)
{
    return Color(((color & 0xF800) >> 11) << 3, ((color & 0x7E0) >> 5) << 2, (color & 0x1F) << 3);
}

__INLINE__ uint16_t Color::toRGB565(uint32_t r, uint32_t g, uint32_t b)
{
    return (uint16_t)(((r << 8) & 0xF800) | ((g << 3) & 0x7E0) | ((b >> 3) & 0x1F));
}

#ifndef NDEBUG
__INLINE__ void Color::dump() const
{
    LOGI("Color = %u(0x%08x) [ red = %u(0x%02x), green = %u(0x%02x), blue = %u(0x%02x), alpha = %u(0x%02x) ]\n", rgba, rgba, red, red, green, green, blue, blue, alpha, alpha);
}
#endif  // NDEBUG


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Bitmap class
//

__INLINE__ Bitmap::Bitmap(JNIEnv* env, jobject bitmap)
    : mEnv(env), mBitmap(bitmap), mLockResult(ANDROID_BITMAP_RESULT_BAD_PARAMETER)
{
    assert(mEnv);
    assert(mBitmap);
}

__INLINE__ Bitmap::~Bitmap()
{
    if (mLockResult == ANDROID_BITMAP_RESULT_SUCCESS)
    {
    #ifndef NDEBUG
        const int result = ::AndroidBitmap_unlockPixels(mEnv, mBitmap);
        __check_error(result != ANDROID_BITMAP_RESULT_SUCCESS, "Couldn't unlock pixels, error = %d\n", result);
    #else
        ::AndroidBitmap_unlockPixels(mEnv, mBitmap);
    #endif  // NDEBUG
    }
}

__INLINE__ int Bitmap::lockPixels(void*& addrPtr)
{
    assert(mEnv);
    assert(mBitmap);

    mLockResult = ::AndroidBitmap_lockPixels(mEnv, mBitmap, &addrPtr);
    __check_error(mLockResult != ANDROID_BITMAP_RESULT_SUCCESS, "Couldn't lock pixels, error = %d\n", mLockResult);
    return mLockResult;
}

__INLINE__ int Bitmap::getBitmapInfo(AndroidBitmapInfo& info) const
{
    assert(mEnv);
    assert(mBitmap);

    const int result = ::AndroidBitmap_getInfo(mEnv, mBitmap, &info);
    __check_error(result != ANDROID_BITMAP_RESULT_SUCCESS, "Couldn't get bitmap info, error = %d\n", result);
    return result;
}


#ifdef _GIF_LIB_H_
///////////////////////////////////////////////////////////////////////////////
// Implementation of the GIFImage class
//

__INLINE__ GIFImage::GIFImage(GifFileType* GIF)
    : mGIF(GIF)
{
    assert(GIF);
    assert(GIF->SWidth > 0);
    assert(GIF->SHeight > 0);
    assert(GIF->ImageCount >= 1);
}

__INLINE__ GIFImage::~GIFImage()
{
#ifndef NDEBUG
    int32_t error = D_GIF_SUCCEEDED;
    if (::DGifCloseFile(mGIF, &error) == GIF_OK)
        LOGI("The GIF resource was released (mGIF = %p)\n", mGIF);
    else
        LOGE("Couldn't release GIF resource, error = %d, error message = %s\n", error, ::GifErrorString(error));
#else
    ::DGifCloseFile(mGIF, NULL);
#endif  // NDEBUG
}

__INLINE__ void GIFImage::draw(uint32_t* canvas, int32_t frameIndex)
{
    assert(mGIF);
    assert(canvas);
    assert(frameIndex >= 0 && frameIndex < getFrameCount());

    GraphicsControlBlock gcb;
    ::DGifSavedExtensionToGCB(mGIF, frameIndex, &gcb);

    const uint32_t bgColor = getBackgroundColor();
    if (frameIndex == 0)
        ::memset(canvas, bgColor, mGIF->SWidth * mGIF->SHeight * sizeof(uint32_t));
    else
        disposeFrame(canvas, frameIndex - 1, gcb, bgColor);

    drawFrame(canvas, mGIF->SavedImages[frameIndex], gcb.TransparentColor);
}

#ifndef NDEBUG
__INLINE__ void GIFImage::dump() const
{
    assert(mGIF);

    char colorMap[MAX_PATH];
    LOGI("GIF Info [ mGIF = %p ]\n{\n  Base Info [ SWidth = %d, SHeight = %d, ImageCount = %d, SBackGroundColor = %d(0x%08x), SColorResolution = %d, SColorMap = %s, SavedImages = %p ]\n", mGIF, mGIF->SWidth, mGIF->SHeight, mGIF->ImageCount, mGIF->SBackGroundColor, getBackgroundColor(), mGIF->SColorResolution, formatColorMap(mGIF->SColorMap, colorMap), mGIF->SavedImages);
    for (int32_t i = 0; i < mGIF->ImageCount; ++i)
    {
        GraphicsControlBlock gcb;
        ::DGifSavedExtensionToGCB(mGIF, i, &gcb);

        const SavedImage& frame = mGIF->SavedImages[i];
        LOGI("  Frame %d [ Left = %d, Top = %d, Width = %d, Height = %d, DisposalMode = %d, DelayTime = %d, TransparentColor = %d, ColorMap = %s ]\n", i, frame.ImageDesc.Left, frame.ImageDesc.Top, frame.ImageDesc.Width, frame.ImageDesc.Height, gcb.DisposalMode, gcb.DelayTime * 10, gcb.TransparentColor, formatColorMap(frame.ImageDesc.ColorMap, colorMap));
    }
    LOGI("}\n");
}
#endif  // NDEBUG

__INLINE__ int32_t GIFImage::getWidth() const
{
    assert(mGIF);
    return mGIF->SWidth;
}

__INLINE__ int32_t GIFImage::getHeight() const
{
    assert(mGIF);
    return mGIF->SHeight;
}

__INLINE__ uint32_t GIFImage::getBackgroundColor() const
{
    assert(mGIF);

    Color color;
    if (mGIF->SColorMap != NULL)
    {
        const GifColorType& c = mGIF->SColorMap->Colors[mGIF->SBackGroundColor];
        color.set(c.Red, c.Green, c.Blue);
    }

    return color.rgba;
}

__INLINE__ int32_t GIFImage::getFrameCount() const
{
    assert(mGIF);
    return mGIF->ImageCount;
}

__INLINE__ int32_t GIFImage::getFrameDelay(int32_t frameIndex) const
{
    assert(mGIF);
    assert(frameIndex >= 0 && frameIndex < getFrameCount());

    GraphicsControlBlock gcb;
    ::DGifSavedExtensionToGCB(mGIF, frameIndex, &gcb);
    return (gcb.DelayTime != 0 ? gcb.DelayTime * 10 : 100);
}

__INLINE__ void GIFImage::drawFrame(uint32_t* canvas, const SavedImage& frame, int32_t transparentColor)
{
    assert(mGIF);
    assert(canvas);

    canvas += frame.ImageDesc.Top * mGIF->SWidth + frame.ImageDesc.Left;
    const GifByteType* rasterBits  = frame.RasterBits;
    const ColorMapObject* colorMap = (frame.ImageDesc.ColorMap != NULL ? frame.ImageDesc.ColorMap : mGIF->SColorMap);

    for (int32_t y = frame.ImageDesc.Height; y > 0; --y)
    {
        drawLine(canvas, rasterBits, colorMap, frame.ImageDesc.Width, transparentColor);
        canvas += mGIF->SWidth;
        rasterBits += frame.ImageDesc.Width;
    }
}

__INLINE__ void GIFImage::disposeFrame(uint32_t* canvas, int32_t prevIndex, const GraphicsControlBlock& /*cur*/, uint32_t bgColor)
{
    assert(mGIF);
    assert(canvas);
    assert(prevIndex >= 0 && prevIndex < getFrameCount());

    GraphicsControlBlock prev;
    ::DGifSavedExtensionToGCB(mGIF, prevIndex, &prev);

    switch (prev.DisposalMode)
    {
    // Restore to background color.
    case DISPOSE_BACKGROUND:
        eraseBackground(canvas, mGIF->SavedImages[prevIndex].ImageDesc, mGIF->SWidth, mGIF->SHeight, bgColor);
        break;

    // Restore to previous frame.
    case DISPOSE_PREVIOUS:
        //if (mBackup != NULL) ::memcpy(canvas, mBackup, mGIF->SWidth * mGIF->SHeight * sizeof(uint32_t));
        LOGI("disposeFrame - DISPOSE_PREVIOUS\n");
        break;
    }
/*
    // Save the current image if the disposal mode is DISPOSE_PREVIOUS.
    if (cur.DisposalMode == DISPOSE_PREVIOUS)
    {
        const size_t size = mGIF->SWidth * mGIF->SHeight * sizeof(uint32_t);
        if (mBackup == NULL)
            mBackup = (uint32_t*)::malloc(size);

        ::memcpy(mBackup, canvas, size);
    }
*/
}

#ifndef NDEBUG
__INLINE__ const char* GIFImage::formatColorMap(const ColorMapObject* colorMap, char (&result)[MAX_PATH])
{
    if (colorMap != NULL)
        ::snprintf(result, _countof(result), "[ ColorCount = %d, BitsPerPixel = %d, SortFlag = %s, Colors = %p ]", colorMap->ColorCount, colorMap->BitsPerPixel, (colorMap->SortFlag ? "true" : "false"), colorMap->Colors);
    else
        ::snprintf(result, _countof(result), "%p", colorMap);

    return result;
}
#endif  // NDEBUG

__INLINE__ void GIFImage::eraseBackground(uint32_t* canvas, const GifImageDesc& desc, int32_t width, int32_t height, uint32_t bgColor)
{
    assert(canvas);

    if (desc.Left == 0 && desc.Top == 0 && desc.Width == width && desc.Height == height)
    {
        ::memset(canvas, bgColor, width * height * sizeof(uint32_t));
    }
    else
    {
        canvas += desc.Top * width + desc.Left;
        for (int32_t y = desc.Height; y > 0; --y)
        {
            ::memset(canvas, bgColor, desc.Width * sizeof(uint32_t));
            canvas += width;
        }
    }
}

__INLINE__ void GIFImage::drawLine(uint32_t* canvas, const GifByteType* src, const ColorMapObject* colorMap, int32_t width, int32_t transparentColor)
{
    assert(src);
    assert(canvas);
    assert(colorMap);

    for (; width > 0; --width, ++src, ++canvas)
    {
        if (*src != transparentColor)
        {
            const GifColorType& c = colorMap->Colors[*src];
            ((Color*)canvas)->set(c.Red, c.Green, c.Blue);
        }
    }
}
#endif  // _GIF_LIB_H_

__END_NAMESPACE

#endif  // __GDIUTIL_INL__