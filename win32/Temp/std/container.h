/////////////////////////////////////////////////////////////////////
// container.h : 头文件
//
// Copyright(c) 2006, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2006/12/25

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CONTAINER_H__
#define __CONTAINER_H__

namespace Utility {

/////////////////////////////////////////////////////////////////////
// node 类的定义
//

template <typename _Ty>
struct node
{
    node(const _Ty& _Val) : _Data(_Val), _Next(NULL)
    { }

    _Ty   _Data;
    node* _Next;
};

/////////////////////////////////////////////////////////////////////
// stack 类的定义
//

template <typename _Ty>
class stack
{
public:
    stack() throw()
        : _Ptr(NULL), _Top(-1), _Count(0)
    {
    }

    explicit stack(int _Count)
    {
        create(_Count);
    }

    ~stack()
    {
        release();
    }

public:
    void create(int _Count)
    {
        this->_Ptr   = new _Ty[_Count];
        this->_Top   = -1;
        this->_Count = _Count;
    }
    
    void release()
    {
        if (_Ptr != NULL)
        {
            delete [] _Ptr;
            _Ptr   = NULL;
            _Top   = -1;
            _Count = 0;
        }
    }

    void push(const _Ty& _Val)
    {
        if (full())
            throw std::out_of_range("The stack is full");

        _Ptr[++_Top] = _Val;
    }

    const _Ty& pop()
    {
        if (empty())
            throw std::out_of_range("The stack is empty");

        return _Ptr[_Top--];
    }

    bool full() const throw()
    {
        return (_Top == _Count - 1);
    }

    bool empty() const throw()
    {
        return (_Top == -1);
    }

private:
    _Ty* _Ptr;
    int  _Top;
    int  _Count;
};


/////////////////////////////////////////////////////////////////////
// dynamic_stack 类的定义
//

template <typename _Ty>
class dynamic_stack
{
public:
    dynamic_stack()
        : _Top(NULL)
    {
    }

    ~dynamic_stack()
    {
        release();
    }

public:
    void release()
    {
        node_ptr _Ptr = NULL;
        while (_Top != NULL)
        {
            _Ptr = _Top;
            _Top = _Top->_Next;
            delete _Ptr;
        }
    }

    bool empty() const throw()
    {
        return (_Top == NULL);
    }

    void push(const _Ty& _Val)
    {
        node_ptr _Ptr = new node<_Ty>(_Val);
        _Ptr->_Next = _Top;
        _Top = _Ptr;
    }

    _Ty pop()
    {
        if (empty())
            throw std::out_of_range("The stack is empty");

        node_ptr _Ptr = _Top;
        _Top     = _Ptr->_Next;
        _Ty _Val = _Ptr->_Data;
        delete _Ptr;

        return _Val;
    }

private:
    typedef typename node<_Ty>* node_ptr;

private:
    node_ptr _Top;
};


/////////////////////////////////////////////////////////////////////
//
// queue 类的定义
//
// 状态
//      empty  : _Front == _Rear
//      full   : (_Rear + 1) % _Count == _Front
//      length : (_Rear - _Front + _Count) % _Count 
//
// 修改指针
//      _Front = (_Front + 1) % _Count
//      _Rear  = (_Rear  + 1) % _Count
//

template <typename _Ty>
class queue
{
public:
    queue() throw()
        : _Ptr(NULL), _Count(0), _Front(0), _Rear(0)
    {
    }

    explicit queue(int _Count)
    {
        create(_Count);
    }

    ~queue()
    {
        release();
    }

public:
    void create(int _Count)
    {
        _Front = _Rear = 0;
        this->_Count = _Count + 1;
        _Ptr = new _Ty[this->_Count];
    }

    void release()
    {
        if (_Ptr != NULL)
        {
            delete [] _Ptr;
            _Ptr  = NULL;
            _Front = _Rear = _Count = 0;
        }
    }

    bool full() const throw()
    {
        return ((_Rear + 1) % _Count == _Front);
    }

    bool empty() const throw()
    {
        return (_Front == _Rear);
    }

    int length() const throw()
    {
        return ((_Rear - _Front + _Count) % _Count);
    }

    void insert(const _Ty& _Val)
    {
        if (full())
            throw std::out_of_range("The queue is full");

        _Ptr[_Rear] = _Val;
        _Rear = (_Rear + 1) % _Count;
    }

    const _Ty& erase()
    {
        if (empty())
            throw std::out_of_range("The queue is empty");

        const _Ty& _Val = _Ptr[_Front];
        _Front = (_Front + 1) % _Count;
        return _Val;
    }

private:
    _Ty* _Ptr;
    int  _Front;
    int  _Rear;
    int  _Count;
};


/////////////////////////////////////////////////////////////////////
// dynamic_queue 类的定义
//

template <typename _Ty>
class dynamic_queue
{
public:
    dynamic_queue() throw()
        : _Front(NULL), _Rear(NULL)
    {
    }

    ~dynamic_queue()
    {
        release();
    }

public:
    void release()
    {
        node_ptr _Ptr = NULL;
        while (_Front != NULL)
        {
            _Ptr   = _Front;
            _Front = _Front->_Next;
            delete _Ptr;
        }

        _Front = _Rear = NULL;
    }

    bool empty() const throw()
    {
        return (_Front == NULL || _Rear == NULL);
    }

    void insert(const _Ty& _Val)
    {
        node_ptr _Ptr = new node<_Ty>(_Val);
        if (empty())
            _Front = _Ptr;
        else
            _Rear->_Next = _Ptr;

        _Rear = _Ptr;
    }

    _Ty erase()
    {
        if (empty())
            throw std::out_of_range("The queue is empty");

        node_ptr _Ptr = _Front;
        _Front   = _Front->_Next;
        _Ty _Val = _Ptr->_Data;
        delete _Ptr;

        return _Val;
    }

private:
    typedef typename node<_Ty>* node_ptr;

private:
    node_ptr _Front;
    node_ptr _Rear;
};

} // namespace Utility

#endif  // __CONTAINER_H__