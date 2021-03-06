////////////////////////////////////////////////////////////////////////////////////
// algorithm.h : 头文件
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2006/12/25

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

namespace Utility {

    /***
    *
    *   int sequence_search(_Ty* _Source, size_t _Size, const _Ty& _Val)
    *
    *   功能:
    *       顺序查找
    *
    *   参数:
    *       _Ty* _Source    _Ty 类型数组指针
    *       size_t _Size    数组的大小
    *       const _Ty& _Val 待查找值的常量引用
    *
    *   返回值:
    *       int 待查找值在源数组的位置(下标)
    *
    *   注意:
    *       该函数从源数组中查找第一个符合要求值. 若找到, 则返回在在源数组的位置
    *       否则返回 -1. 该算法的执行时间为 O(n)
    *
    *******************************************************************************/

    template <typename _Ty>
    inline int sequence_search(_Ty* _Source, size_t _Size, const _Ty& _Val)
    {
        _ASSERTE(_Source != NULL);

        int _Index = static_cast<int>(_Size - 1);
        while (_Index >= 0 && _Source[_Index] != _Val)
            --_Index;

        return _Index;
    }


    /***
    *
    *   int binary_search(_Ty* _Source, size_t _Size, const _Ty& _Val)
    *
    *   功能:
    *       折半查找
    *
    *   参数:
    *       _Ty* _Source    _Ty 类型数组指针
    *       size_t _Size    数组的大小
    *       const _Ty& _Val 待查找值的常量引用
    *
    *   返回值:
    *       int 待查找值在源数组的位置(下标)
    *
    *   注意:
    *       该函数从源数组中查找第一个符合要求值. 若找到, 则返回在在源数组的位置
    *       否则返回 -1.该算法要求数组中的元素必须为升序.该算法的执行时间为 O(log2n)
    *
    *******************************************************************************/

    template <typename _Ty>
    inline int binary_search(_Ty* _Source, size_t _Size, const _Ty& _Val)
    {
        _ASSERTE(_Source != NULL);

        int _Low = 0, _High = static_cast<int>(_Size - 1);
        int _Middle = (_Low + _High) / 2;

        while (_Low <= _High && _Source[_Middle] != _Val)
        {
            if (_Source[_Middle] > _Val)
                _High = _Middle - 1;
            else
                _Low  = _Middle + 1;

            _Middle = (_Low + _High) / 2;
        }

        return (_Low <= _High) ? _Middle : -1;
    }


    /***
    *
    *   void select_sort(_Ty* _Source, size_t _Size)
    *
    *   功能:
    *       选择排序
    *
    *   参数:
    *       _Ty* _Source    _Ty 类型数组指针
    *       size_t _Size    数组的大小
    *
    *   返回值:
    *       void
    *
    *   注意:
    *       该函数对源数组按升序排序, 该算法的执行时间为 O(n²)
    *
    *******************************************************************************/

    template <typename _Ty>
    inline void select_sort(_Ty* _Source, size_t _Size)
    {
        _ASSERTE(_Source != NULL);

        size_t _Min = 0;    // 最小元素的下标
        for (size_t i = 0; i < _Size - 1; ++i)
        {
            _Min = i;
            for (size_t j = i + 1; j < _Size; ++j)
            {
                if (_Source[_Min] > _Source[j])
                    _Min = j;   // 交换下标
            }

            // 交换数值
            if (_Min != i)
            {
                _Ty _Temp     = _Source[_Min];
                _Source[_Min] = _Source[i];
                _Source[i]    = _Temp;
            }
        }
    }


    /***
    *
    *   void bubble_sort(_Ty* _Source, size_t _Size)
    *
    *   功能:
    *       气泡排序
    *
    *   参数:
    *       _Ty* _Source    _Ty 类型数组指针
    *       size_t _Size    数组的大小
    *
    *   返回值:
    *       void
    *
    *   注意:
    *       该函数对源数组按升序排序, 该算法的执行时间为 O(n²)
    *
    *******************************************************************************/

    template <typename _Ty>
    inline void bubble_sort(_Ty* _Source, size_t _Size)
    {
        _ASSERTE(_Source != NULL);
        bool _Swap = false;     // true 表示交换过数据, false 没有

        do
        {
            _Swap = false;
            for (size_t i = 0; i < _Size - 1; ++i)
            {
                if (_Source[i] > _Source[i + 1])
                {
                    _Swap = true;
                    _Ty _Temp  = _Source[i];
                    _Source[i] = _Source[i + 1];
                    _Source[i + 1] = _Temp;
                }
            }
        }
        while (--_Size > 1 && _Swap);
    }


    /***
    *
    *   void insert_sort(_Ty* _Source, size_t _Size)
    *
    *   功能:
    *       插入排序
    *
    *   参数:
    *       _Ty* _Source    _Ty 类型数组指针
    *       size_t _Size    数组的大小
    *
    *   返回值:
    *       void
    *
    *   注意:
    *       该函数对源数组按升序排序, 该算法的执行时间为 O(n²)
    *
    *******************************************************************************/

    template <typename _Ty>
    inline void insert_sort(_Ty* _Source, size_t _Size)
    {
        _ASSERTE(_Source != NULL);

        for (size_t i = 1; i < _Size; ++i)
        {
            _Ty _Temp = _Source[i];
            size_t _Index = i - 1;

            while (_Index >= 0 && _Temp < _Source[_Index])
            {
                _Source[_Index + 1] = _Source[_Index];
                --_Index;
            }

            if (_Index != i - 1)
                _Source[_Index + 1] = _Temp;
        }
    }

} // namespace Utility

#endif  // __ALGORITHM_H__