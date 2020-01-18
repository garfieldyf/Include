///////////////////////////////////////////////////////////////////////////////
// imagealgth.inl
//
// Author : Garfield
// Creation Date : 2013/6/8

#ifndef __IMAGEALGTH_INL__
#define __IMAGEALGTH_INL__

#ifndef __IMAGEALGTH_H__
    #error imagealgth.inl requires imagealgth.h to be included first
#endif

__BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
// Implementation of the ColorF class
//

__INLINE__ ColorF::ColorF(float r, float g, float b)
    : red(r), blue(b), green(g)
{
}

template <typename _Ty>
__INLINE__ void ColorF::calculate(const _Ty& color, float matrix)
{
    red   += color.red   * matrix;
    green += color.green * matrix;
    blue  += color.blue  * matrix;
}

__END_NAMESPACE

#endif  // __IMAGEALGTH_INL__
