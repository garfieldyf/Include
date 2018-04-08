///////////////////////////////////////////////////////////////////////////////
// dib.h : 头文件
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/8/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __DIBITMAP_H__
#define __DIBITMAP_H__

///////////////////////////////////////////////////////////////////////////////
// CDIBitmap 类的定义
//

class CDIBitmap
{
public:
    enum { bmpFlag = 0x4D42 };

// 构造/销毁
public:
    CDIBitmap();
    ~CDIBitmap();

// 操作
public:
    BOOL Load(LPCTSTR lpszFileName);
    BOOL Save(LPCTSTR lpszFileName);
    static BOOL Save(LPCTSTR lpszFileName, LPRGBQUAD lpColors, DWORD dwWidth, DWORD dwHeight);

    HBITMAP CreateDIBitmap(HDC hDC);
    BOOL GetDIBits(LPRGBQUAD lpColors, DWORD dwSize);

    void Release();
    int Draw(HDC hDC, int x, int y);

// 属性
public:
    LONG GetWidth() const;
    LONG GetHeight() const;
    const BITMAPINFO* GetBitmapInfo() const;

// 实现
protected:
    DWORD ReadFileHeader(HANDLE hFile);
    DWORD ReadBitmapInfo(HANDLE hFile, DWORD dwSize);
    static BOOL WriteFileHeader(HANDLE hFile, DWORD dwFileSize, DWORD dwOffBits);

    void GetDIBits8(LPRGBQUAD lpColors, LPBYTE lpColorBits);
    void GetDIBits24(LPRGBQUAD lpColors, LPBYTE lpColorBits);

// 成员
public:
    LPBITMAPINFO m_pBmpInfo;    // 位图信息
    LPBYTE       m_pColorBits;  // 位图像素数据
    DWORD        m_dwOffset;    // 每行填充的 bit 位
};

inline CDIBitmap::CDIBitmap()
    : m_pBmpInfo(NULL)
    , m_pColorBits(NULL)
    , m_dwOffset(0)
{
}

inline CDIBitmap::~CDIBitmap()
{
    Release();
}

inline LONG CDIBitmap::GetWidth() const
{
    return (m_pBmpInfo != NULL ? m_pBmpInfo->bmiHeader.biWidth : 0);
}

inline LONG CDIBitmap::GetHeight() const
{
    return (m_pBmpInfo != NULL ? m_pBmpInfo->bmiHeader.biHeight : 0);
}

inline const BITMAPINFO* CDIBitmap::GetBitmapInfo() const
{
    return m_pBmpInfo;
}

inline BOOL CDIBitmap::Load(LPCTSTR lpszFileName)
{
    _ASSERTE(lpszFileName);

    HANDLE hFile = INVALID_HANDLE_VALUE;
    BOOL bSuccessful = FALSE;
    Release();                  // 释放原来的内存

    __try
    {
        // 打开文件
        hFile = ::CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        // 获得文件的大小
        LARGE_INTEGER liFileSize = { 0 };
        if (!::GetFileSizeEx(hFile, &liFileSize))
            __leave;

        // 读取位图文件首部信息
        liFileSize.LowPart = ReadFileHeader(hFile);
        if (liFileSize.LowPart == 0)
            __leave;

        // 读取位图首部信息和颜色表
        liFileSize.LowPart = ReadBitmapInfo(hFile, liFileSize.LowPart);
        if (liFileSize.LowPart == 0)
            __leave;

        // 读取位图的像素信息
        _ASSERTE(m_pColorBits == NULL);
        m_pColorBits = reinterpret_cast<LPBYTE>(::malloc(liFileSize.LowPart));
        _ASSERTE(m_pColorBits);

        bSuccessful = ::ReadFile(hFile, m_pColorBits, liFileSize.LowPart, &liFileSize.LowPart, NULL);
    }
    __finally
    {
        if (!bSuccessful)
            Release();

        ::CloseHandle(hFile);
    }

    return bSuccessful;
}

inline BOOL CDIBitmap::Save(LPCTSTR lpszFileName)
{
    _ASSERTE(m_pColorBits);
    _ASSERTE(m_pBmpInfo);
    _ASSERTE(lpszFileName);

    BOOL bSuccessful = FALSE;
    if (m_pBmpInfo != NULL && m_pColorBits != NULL)
    {
        HANDLE hFile = INVALID_HANDLE_VALUE;
        __try
        {
            // 打开文件
            hFile = ::CreateFile(lpszFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile == INVALID_HANDLE_VALUE)
                __leave;

            // 写入位图文件首部信息
            DWORD dwFileSize = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + ::_msize(m_pBmpInfo) + ::_msize(m_pColorBits));
            if (!WriteFileHeader(hFile, dwFileSize, dwFileSize - ::_msize(m_pColorBits)))
                __leave;

            // 写入位图首部信息和颜色表
            if (!::WriteFile(hFile, m_pBmpInfo, static_cast<DWORD>(::_msize(m_pBmpInfo)), &dwFileSize, NULL))
                __leave;

            // 写入位图的像素信息
            bSuccessful = ::WriteFile(hFile, m_pColorBits, static_cast<DWORD>(::_msize(m_pColorBits)), &dwFileSize, NULL);
        }
        __finally
        {
            ::CloseHandle(hFile);
        }
    }

    return bSuccessful;
}

inline BOOL CDIBitmap::Save(LPCTSTR lpszFileName, LPRGBQUAD lpColors, DWORD dwWidth, DWORD dwHeight)
{
    _ASSERTE(lpColors);
    _ASSERTE(lpszFileName);

    BOOL bSuccessful = FALSE;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    __try
    {
        // 打开文件
        hFile = ::CreateFile(lpszFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            __leave;

        // 写入位图文件首部信息
        DWORD dwImageSize = sizeof(RGBQUAD) * dwWidth * dwHeight;
        DWORD dwFileSize  = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwImageSize;
        if (!WriteFileHeader(hFile, dwFileSize, dwFileSize - dwImageSize))
            __leave;

        // 写入位图首部信息
        BITMAPINFOHEADER bmiHeader = { sizeof(BITMAPINFOHEADER), dwWidth, dwHeight, 1, 32, 0, dwImageSize };
        if (!::WriteFile(hFile, &bmiHeader, sizeof(BITMAPINFOHEADER), &dwFileSize, NULL))
            __leave;

        // 写入位图的像素信息
        bSuccessful = ::WriteFile(hFile, lpColors, dwImageSize, &dwFileSize, NULL);
    }
    __finally
    {
        ::CloseHandle(hFile);
    }

    return bSuccessful;
}

inline HBITMAP CDIBitmap::CreateDIBitmap(HDC hDC)
{
    _ASSERTE(hDC);

    HBITMAP hBitmap = NULL;
    if (m_pBmpInfo != NULL && m_pColorBits != NULL)
        hBitmap = ::CreateDIBitmap(hDC, &m_pBmpInfo->bmiHeader, CBM_INIT, m_pColorBits, m_pBmpInfo, DIB_RGB_COLORS);

    return hBitmap;
}

inline BOOL CDIBitmap::GetDIBits(LPRGBQUAD lpColors, DWORD dwSize)
{
    _ASSERTE(lpColors);

    BOOL bSuccessful = FALSE;
    ::ZeroMemory(lpColors, dwSize);

    if (m_pBmpInfo != NULL && m_pColorBits != NULL && lpColors != NULL)
    {
        switch (m_pBmpInfo->bmiHeader.biBitCount)
        {
        // 256 色位图
        case 8:
            GetDIBits8(lpColors, m_pColorBits);
            break;

        // 24 位色位图
        case 24:
            GetDIBits24(lpColors, m_pColorBits);
            break;

        // 32 位色位图
        case 32:
            _ASSERTE(::_msize(m_pColorBits) == dwSize);
            ::memcpy(lpColors, m_pColorBits, dwSize);
            break;

        default:
            _ASSERTE(FALSE);
        }

        bSuccessful = TRUE;
    }

    return bSuccessful;
}

inline void CDIBitmap::Release()
{
    ::free(m_pBmpInfo);
    ::free(m_pColorBits);
    m_pBmpInfo = NULL;
    m_pColorBits = NULL;
    m_dwOffset = 0;
}

inline int CDIBitmap::Draw(HDC hDC, int x, int y)
{
    _ASSERTE(hDC);

    int nResult = GDI_ERROR;
    if (m_pBmpInfo != NULL && m_pColorBits != NULL)
    {
        nResult = ::StretchDIBits(hDC, x, y, m_pBmpInfo->bmiHeader.biWidth, m_pBmpInfo->bmiHeader.biHeight, 0, 0,
                                  m_pBmpInfo->bmiHeader.biWidth, m_pBmpInfo->bmiHeader.biHeight, m_pColorBits, m_pBmpInfo,
                                  DIB_RGB_COLORS, SRCCOPY);
    }

    return nResult;
}

inline DWORD CDIBitmap::ReadFileHeader(HANDLE hFile)
{
    _ASSERTE(hFile);

    DWORD dwReaded = 0, dwSize = 0;
    BITMAPFILEHEADER bmHeader = { 0 };

    if (::ReadFile(hFile, &bmHeader, sizeof(BITMAPFILEHEADER), &dwReaded, NULL))
    {
        if (dwReaded == sizeof(BITMAPFILEHEADER) && bmHeader.bfType == bmpFlag)
            dwSize = bmHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
    }

    return dwSize;
}

inline BOOL CDIBitmap::WriteFileHeader(HANDLE hFile, DWORD dwFileSize, DWORD dwOffBits)
{
    _ASSERTE(hFile);

    DWORD dwWritten = 0;
    BITMAPFILEHEADER bmHeader = { 0 };
    bmHeader.bfType    = bmpFlag;
    bmHeader.bfSize    = dwFileSize;
    bmHeader.bfOffBits = dwOffBits;

    return ::WriteFile(hFile, &bmHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
}

inline DWORD CDIBitmap::ReadBitmapInfo(HANDLE hFile, DWORD dwSize)
{
    _ASSERTE(hFile);
    _ASSERTE(dwSize >= 0);
    _ASSERTE(m_pBmpInfo == NULL);

    DWORD dwReaded = 0, dwSizeImage = 0;
    m_pBmpInfo = reinterpret_cast<LPBITMAPINFO>(::malloc(dwSize));
    _ASSERTE(m_pBmpInfo);

    if (::ReadFile(hFile, m_pBmpInfo, dwSize, &dwReaded, NULL) && dwSize == dwReaded)
    {
        DWORD dwLineSize = m_pBmpInfo->bmiHeader.biWidth * m_pBmpInfo->bmiHeader.biBitCount;
        m_dwOffset = dwLineSize % 32;
        if (m_dwOffset != 0)
            m_dwOffset = 32 - m_dwOffset;

        _ASSERTE(((dwLineSize + m_dwOffset) * m_pBmpInfo->bmiHeader.biHeight) % 32 == 0);
        dwSizeImage = ((dwLineSize + m_dwOffset) * m_pBmpInfo->bmiHeader.biHeight) / 8;
        m_dwOffset /= 8;
    }

    return dwSizeImage;
}

inline void CDIBitmap::GetDIBits8(LPRGBQUAD lpColors, LPBYTE lpColorBits)
{
    _ASSERTE(lpColors);
    _ASSERTE(lpColorBits);
    _ASSERTE(m_pBmpInfo);

    for (LONG i = 0; i < m_pBmpInfo->bmiHeader.biHeight; ++i, lpColorBits += m_dwOffset)
    {
        for (LONG j = 0; j < m_pBmpInfo->bmiHeader.biWidth; ++j)
            *lpColors++ = m_pBmpInfo->bmiColors[*lpColorBits++];
    }
}

inline void CDIBitmap::GetDIBits24(LPRGBQUAD lpColors, LPBYTE lpColorBits)
{
    _ASSERTE(lpColors);
    _ASSERTE(lpColorBits);
    _ASSERTE(m_pBmpInfo);

    for (LONG i = 0; i < m_pBmpInfo->bmiHeader.biHeight; ++i, lpColorBits += m_dwOffset)
    {
        for (LONG j = 0; j < m_pBmpInfo->bmiHeader.biWidth; ++j, ++lpColors)
        {
            lpColors->rgbBlue  = *lpColorBits++;
            lpColors->rgbGreen = *lpColorBits++;
            lpColors->rgbRed   = *lpColorBits++;
        }
    }
}

#endif  // __DIBITMAP_H__