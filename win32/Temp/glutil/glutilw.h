///////////////////////////////////////////////////////////////////////////////
// glutil.h : 头文件
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2007/8/16

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __GLUTIL_H__
#define __GLUTIL_H__

#ifndef _INC_MATH
    #error glutil.h requires math.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
//

#ifdef GLUTIL_DISABLE_WARNING
  #pragma warning(push, 3)
  #pragma warning(disable : 4244)
#endif  // GLUTIL_DISABLE_WARNING

namespace glutil {

///////////////////////////////////////////////////////////////////////////
// CGLPoint 类的定义
//

template <typename _Ty>
class CGLPoint
{
// 构造/销毁
public:
    CGLPoint() throw();
    CGLPoint(const CGLPoint& that) throw();
    CGLPoint(_Ty initX, _Ty initY, _Ty initZ = 0) throw();
    template <typename _Tx> CGLPoint(const CGLPoint<_Tx>& that) throw();

// 操作
public:
    _Ty* GetData() throw();
    const _Ty* GetData() const throw();

    bool operator==(const CGLPoint& point) const throw();
    bool operator!=(const CGLPoint& point) const throw();

    CGLPoint& operator=(const CGLPoint& that) throw();
    template <typename _Tx> CGLPoint& operator=(const CGLPoint<_Tx>& that) throw();

    void Offset(const CGLPoint& point) throw();
    void Offset(_Ty xOffset, _Ty yOffset, _Ty zOffset = 0) throw();
    template <typename _Tx> void Offset(const CGLPoint<_Tx>& point) throw();

// 成员
public:
    _Ty x;
    _Ty y;
    _Ty z;
};

template <typename _Ty>
inline CGLPoint<_Ty>::CGLPoint() throw()
    : x(0), y(0), z(0)
{
}

template <typename _Ty>
inline CGLPoint<_Ty>::CGLPoint(const CGLPoint& that) throw()
    : x(that.x), y(that.y), z(that.z)
{
}

template <typename _Ty>
inline CGLPoint<_Ty>::CGLPoint(_Ty initX, _Ty initY, _Ty initZ /*= 0*/) throw()
    : x(initX), y(initY), z(initZ)
{
}

template <typename _Ty> template <typename _Tx>
inline CGLPoint<_Ty>::CGLPoint(const CGLPoint<_Tx>& that) throw()
    : x(that.x), y(that.y), z(that.z)
{
}

template <typename _Ty>
inline _Ty* CGLPoint<_Ty>::GetData() throw()
{
    return (_Ty*)this;
}

template <typename _Ty>
inline const _Ty* CGLPoint<_Ty>::GetData() const throw()
{
    return (_Ty*)this;
}

template <typename _Ty>
inline bool CGLPoint<_Ty>::operator==(const CGLPoint& point) const throw()
{
    return (!(x > point.x || x < point.x) && !(y > point.y || y < point.y) && !(z > point.z || z < point.z));
}

template <typename _Ty>
inline bool CGLPoint<_Ty>::operator!=(const CGLPoint& point) const throw()
{
    return !(operator==(point));
}

template <typename _Ty>
inline CGLPoint<_Ty>& CGLPoint<_Ty>::operator=(const CGLPoint& that) throw()
{
    if (this != &that)
    {
        x = that.x;
        y = that.y;
        z = that.z;
    }

    return *this;
}

template <typename _Ty> template <typename _Tx>
inline CGLPoint<_Ty>& CGLPoint<_Ty>::operator=(const CGLPoint<_Tx>& that) throw()
{
    x = that.x;
    y = that.y;
    z = that.z;

    return *this;
}

template <typename _Ty>
inline void CGLPoint<_Ty>::Offset(const CGLPoint& point) throw()
{
    Offset(point.x, point.y, point.z);
}

template <typename _Ty> template <typename _Tx>
inline void CGLPoint<_Ty>::Offset(const CGLPoint<_Tx>& point) throw()
{
    Offset(point.x, point.y, point.z);
}

template <typename _Ty>
inline void CGLPoint<_Ty>::Offset(_Ty xOffset, _Ty yOffset, _Ty zOffset /*= 0*/) throw()
{
    x += xOffset;
    y += yOffset;
    z += zOffset;
}


///////////////////////////////////////////////////////////////////////////
// CGLVertex 类的定义
//

template <typename _Ty>
class CGLVertex : public CGLPoint<_Ty>
{
// 构造
public:
    CGLVertex() throw();
    CGLVertex(const CGLVertex& that) throw();
    CGLVertex(_Ty initX, _Ty initY, _Ty initZ = 0, _Ty initW = 1.0) throw();
    template <typename _Tx> CGLVertex(const CGLVertex<_Tx>& that) throw();

// 操作
public:
    CGLVertex& operator=(const CGLPoint& that) throw();
    CGLVertex& operator=(const CGLVertex& that) throw();
    template <typename _Tx> CGLVertex& operator=(const CGLPoint<_Tx>& that) throw();
    template <typename _Tx> CGLVertex& operator=(const CGLVertex<_Tx>& that) throw();

    _Ty Distance(const CGLVertex& vertex) const throw();
    _Ty Distance(_Ty xSrc, _Ty ySrc, _Ty zSrc = 0, _Ty wSrc = 1.0) const throw();
    template <typename _Tx> _Ty Distance(const CGLVertex<_Tx>& vertex) const throw();

// 成员
public:
    _Ty w;
};

//template <typename _Ty>
//inline _Ty CGLVertex<_Ty>::Distance(const CGLVertex& source) const throw()
//{
//  return ::sqrt((x - source.x) * (x - source.x) + (y - source.y) * (y - source.y) + (z - source.z) * (z - source.z));
//}
//
//template <typename _Ty>
//inline _Ty CGLVertex<_Ty>::Distance(_Ty xSrc, _Ty ySrc, _Ty zSrc /*= 0*/) const throw()
//{
//  return ::sqrt((x - xSrc) * (x - xSrc) + (y - ySrc) * (y - ySrc) + (z - zSrc) * (z - zSrc));
//}
//
//template <typename _Ty> template <typename _Tx>
//inline _Ty CGLVertex<_Ty>::Distance(const CGLVertex<_Tx>& source) const throw()
//{
//  return ::sqrt((x - source.x) * (x - source.x) + (y - source.y) * (y - source.y) + (z - source.z) * (z - source.z));
//}

}  // namespace glutil

#ifdef GLUTIL_DISABLE_WARNING
  #pragma warning(default: 4244)
  #pragma warning(pop)
#endif  // GLUTIL_DISABLE_WARNING

#endif  // __GLUTIL_H__