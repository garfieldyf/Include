///////////////////////////////////////////////////////////////////////////////
// gdiutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2013/11/5

#ifndef __GDIUTIL_H__
#define __GDIUTIL_H__

#include "platform.h"
#include <android/bitmap.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// Point
// Rect
// Color
// Bitmap
// GIFImage

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Interface of the Point class
//

class Point
{
// Constructors
public:
    explicit Point(int32_t X = 0, int32_t Y = 0);
    Point(const Point& that);
    Point(const Point* point);

// Operations
public:
    void set(const Point& point);
    void set(int32_t X, int32_t Y);

    void negate();
    void offset(int dx, int dy);

    Point& operator=(const Point& that);
    bool operator==(const Point& point) const;
    bool operator!=(const Point& point) const;

#ifndef NDEBUG
    void dump() const;
#endif

// Data members
public:
    int32_t x;
    int32_t y;
};


#ifdef ANDROID_RECT_H
///////////////////////////////////////////////////////////////////////////////
// Interface of the Rect class
//

class Rect : public ARect
{
// Constructors
public:
    explicit Rect(int32_t l = 0, int32_t t = 0, int32_t r = 0, int32_t b = 0);
    Rect(const ARect& that);
    Rect(const ARect* rect);
    Rect(const Point& topLeft, const Point& bottomRight);

// Operations
public:
    operator ARect*();
    operator const ARect*() const;

    Rect& operator=(const ARect& that);
    bool operator==(const ARect& rect) const;
    bool operator!=(const ARect& rect) const;

    void setEmpty();
    void set(int32_t l, int32_t t, int32_t r, int32_t b);
    void set(const Point& topLeft, const Point& bottomRight);
    void setRect(int32_t x, int32_t y, int32_t width, int32_t height);

    void offset(const Point& point);
    void offset(int32_t dx, int32_t dy);
    void offsetTo(const Point& point);
    void offsetTo(int32_t newLeft, int32_t newTop);

    void inset(const Point& point);
    void inset(int32_t dx, int32_t dy);
    void inset(int32_t l, int32_t t, int32_t r, int32_t b);

    bool contains(const ARect& rect) const;
    bool contains(const Point& point) const;
    bool contains(int32_t x, int32_t y) const;
    bool contains(int32_t l, int32_t t, int32_t r, int32_t b) const;

#ifndef NDEBUG
    void dump() const;
#endif

// Attributes
public:
    bool isEmpty() const;

    int32_t width() const;
    int32_t height() const;

    Point center() const;
    int32_t centerX() const;
    int32_t centerY() const;

    const Point& topLeft() const;
    const Point& bottomRight() const;
};
#endif  // ANDROID_RECT_H


///////////////////////////////////////////////////////////////////////////////
// Interface of the Color class
//

class Color
{
public:
    // Common color constants
    enum {
        BLACK       = 0xFF000000,
        DARKGRAY    = 0xFF444444,
        GRAY        = 0xFF888888,
        LIGHTGRAY   = 0xFFCCCCCC,
        WHITE       = 0xFFFFFFFF,
        RED         = 0xFF0000FF,
        GREEN       = 0xFF00FF00,
        BLUE        = 0xFFFF0000,
        YELLOW      = 0xFF00FFFF,
        CYAN        = 0xFFFFFF00,
        MAGENTA     = 0xFFFF00FF,
        TRANSPARENT = 0x00000000,
    };

// Constructors
public:
    explicit Color(uint32_t color = TRANSPARENT);
    Color(const Color& that);
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

// Operations
public:
    operator uint32_t() const;
    Color& operator=(uint32_t color);
    Color& operator=(const Color& that);

    uint16_t toRGB565() const;
    void swap(Color& color);

    void setRGB(uint8_t r, uint8_t g, uint8_t b);
    void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

    static Color fromRGB565(uint16_t color);
    static uint16_t toRGB565(uint32_t r, uint32_t g, uint32_t b);

#ifndef NDEBUG
    void dump() const;
#endif

// Data members
public:
    union {
        uint32_t rgba;
        struct {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t alpha;
        };
    };
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the Bitmap class
//

class Bitmap
{
    DECLARE_NONCOPYABLE(Bitmap);

// Constructors/Destructor
public:
    Bitmap(JNIEnv* _env, jobject _bitmap);
    ~Bitmap();

// Operations
public:
    int lockPixels(void*& addrPtr);

// Attributes
public:
    int getBitmapInfo(AndroidBitmapInfo& info) const;

// Data members
public:
    JNIEnv* env;
    int lockRes;
    jobject bitmap;
};


#ifdef _GIF_LIB_H_
///////////////////////////////////////////////////////////////////////////////
// Interface of the GIFImage class
//

class GIFImage
{
    DECLARE_NONCOPYABLE(GIFImage);

// Constructors/Destructor
public:
    explicit GIFImage(GifFileType* GIF);
    ~GIFImage();

// Operations
public:
    void draw(uint32_t* canvas, int32_t frameIndex);

#ifndef NDEBUG
    void dump() const;
#endif

// Attributes
public:
    int32_t getWidth() const;
    int32_t getHeight() const;
    uint32_t getBackgroundColor() const;

    int32_t getFrameCount() const;
    int32_t getFrameDelay(int32_t frameIndex) const;

// Implementation
private:
    void drawFrame(uint32_t* canvas, const SavedImage& frame, int32_t transparentColor);
    void disposeFrame(uint32_t* canvas, int32_t prevIndex, const GraphicsControlBlock& cur, uint32_t bgColor);

#ifndef NDEBUG
    static const char* formatColorMap(const ColorMapObject* colorMap, char (&result)[MAX_PATH]);
#endif

    static void eraseBackground(uint32_t* canvas, const GifImageDesc& desc, int32_t width, int32_t height, uint32_t bgColor);
    static void drawLine(uint32_t* canvas, const GifByteType* src, const ColorMapObject* colorMap, int32_t width, int32_t transparentColor);

// Data members
private:
    GifFileType* mGIF;
};
#endif  // _GIF_LIB_H_

__END_NAMESPACE

#include "gdiutil.inl"

#endif  // __GDIUTIL_H__