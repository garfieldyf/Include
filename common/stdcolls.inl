///////////////////////////////////////////////////////////////////////////////
// stdcolls.inl
//
// Author : Garfield
// Creation Date : 2020/4/12

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __STDCOLLS_INL__
#define __STDCOLLS_INL__

#ifndef __STDCOLLS_H__
    #error stdcolls.inl requires stdcolls.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the queue / stack class
//

template <typename _Ty>
using queue = container<std::queue<_Ty>>;

template <typename _Ty>
using stack = container<std::stack<_Ty>>;


///////////////////////////////////////////////////////////////////////////////
// Implementation of the container class
//

template <typename _TBase>
__INLINE__ container<_TBase>::container(container_type&& _Cont)
    : _TBase(std::move(_Cont))
{
}

template <typename _TBase>
__INLINE__ container<_TBase>::container(const container_type& _Cont)
    : _TBase(_Cont)
{
}

template <typename _TBase>
__INLINE__ void container<_TBase>::clear()
{
    c.clear();
}

template <typename _TBase>
__INLINE__ void container<_TBase>::shrink_to_fit()
{
    c.shrink_to_fit();
}

template <typename _TBase> template <typename _Predicate>
__INLINE__ void container<_TBase>::erase_if(_Predicate _Pred)
{
    auto _Last  = c.end();
    auto _Where = std::find_if(c.begin(), _Last, _Pred);
    if (_Where != _Last) {
        c.erase(_Where);
    }
}

template <typename _TBase>
__INLINE__ void container<_TBase>::erase(const value_type& _Val)
{
    auto _Last  = c.end();
    auto _Where = std::find(c.begin(), _Last, _Val);
    if (_Where != _Last) {
        c.erase(_Where);
    }
}

template <typename _TBase> template <typename _Predicate>
__INLINE__ void container<_TBase>::remove_if(_Predicate _Pred)
{
    auto _Last = c.end();
    c.erase(std::remove_if(c.begin(), _Last, _Pred), _Last);
}

template <typename _TBase>
__INLINE__ void container<_TBase>::remove(const value_type& _Val)
{
    auto _Last = c.end();
    c.erase(std::remove(c.begin(), _Last, _Val), _Last);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the _Blocking_container class
//

template <typename _Container>
__INLINE__ void _Blocking_container<_Container>::clear()
{
    mutex_lock _Lock(_Mymutex);
    _Mycont.clear();
}

template <typename _Container>
__INLINE__ void _Blocking_container<_Container>::shrink_to_fit()
{
    mutex_lock _Lock(_Mymutex);
    _Mycont.shrink_to_fit();
}

template <typename _Container>
__INLINE__ bool _Blocking_container<_Container>::empty() const
{
    mutex_lock _Lock(_Mymutex);
    return _Mycont.empty();
}

template <typename _Container>
__INLINE__ typename _Blocking_container<_Container>::size_type _Blocking_container<_Container>::size() const
{
    mutex_lock _Lock(_Mymutex);
    return _Mycont.size();
}

template <typename _Container> template <typename _Predicate>
__INLINE__ bool _Blocking_container<_Container>::_Pop(value_type& _Val, uint32_t _Timeout, _Predicate _Pred)
{
    bool _Result;
    auto _Cond = [this]() {
        return !_Mycont.empty();
    };

    unique_lock _Lock(_Mymutex);
    if (_Timeout == static_cast<uint32_t>(-1)) {
        // Waiting forever, if _Mycont is empty.
        _Mycond.wait(_Lock, _Cond);
        _Result = true;
    } else {
        // Waiting for _Timeout milliseconds, if _Mycont is empty.
        _Result = _Mycond.wait_for(_Lock, std::chrono::milliseconds(_Timeout), _Cond);
    }

    // Popup the element from the _Mycont, if waiting successful.
    if (_Result) {
        _Pred(_Val);
    }

    return _Result;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the blocking_deque class
//

template <typename _Ty>
__INLINE__ void blocking_deque<_Ty>::push_front(value_type&& _Val)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.push_front(std::move(_Val));
    }

    _Mycond.notify_one();
}

template <typename _Ty>
__INLINE__ void blocking_deque<_Ty>::push_front(const value_type& _Val)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.push_front(_Val);
    }

    _Mycond.notify_one();
}

template <typename _Ty>
__INLINE__ void blocking_deque<_Ty>::push_back(value_type&& _Val)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.push_back(std::move(_Val));
    }

    _Mycond.notify_one();
}

template <typename _Ty>
__INLINE__ void blocking_deque<_Ty>::push_back(const value_type& _Val)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.push_back(_Val);
    }

    _Mycond.notify_one();
}

template <typename _Ty> template <typename... _ValArgs>
__INLINE__ void blocking_deque<_Ty>::emplace_front(_ValArgs&&... _Args)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.emplace_front(std::forward<_ValArgs>(_Args)...);
    }

    _Mycond.notify_one();
}

template <typename _Ty> template <typename... _ValArgs>
__INLINE__ void blocking_deque<_Ty>::emplace_back(_ValArgs&&... _Args)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.emplace_back(std::forward<_ValArgs>(_Args)...);
    }

    _Mycond.notify_one();
}

template <typename _Ty>
__INLINE__ bool blocking_deque<_Ty>::pop_front(value_type& _Val, uint32_t _Timeout/* = -1*/)
{
    return this->_Pop(_Val, _Timeout, [this](value_type& _Out) {
        _Out = std::move(_Mycont.front());
        _Mycont.pop_front();
    });
}

template <typename _Ty>
__INLINE__ bool blocking_deque<_Ty>::pop_back(value_type& _Val, uint32_t _Timeout/* = -1*/)
{
    return this->_Pop(_Val, _Timeout, [this](value_type& _Out) {
        _Out = std::move(_Mycont.back());
        _Mycont.pop_back();
    });
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the priority_queue class
//

template <typename _Ty, typename _Comparator>
__INLINE__ priority_queue<_Ty, _Comparator>::priority_queue(const _Comparator& _Comp, container_type&& _Cont)
    : _Mybase(_Comp, std::move(_Cont))
{
}

template <typename _Ty, typename _Comparator>
__INLINE__ priority_queue<_Ty, _Comparator>::priority_queue(const _Comparator& _Comp, const container_type& _Cont)
    : _Mybase(_Comp, _Cont)
{
}

template <typename _Ty, typename _Comparator>
__INLINE__ void priority_queue<_Ty, _Comparator>::clear()
{
    this->c.clear();
}

template <typename _Ty, typename _Comparator>
__INLINE__ void priority_queue<_Ty, _Comparator>::shrink_to_fit()
{
    this->c.shrink_to_fit();
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the priority_blocking_queue class
//

template <typename _Ty, typename _Comparator>
__INLINE__ void priority_blocking_queue<_Ty, _Comparator>::push(value_type&& _Val)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.push(std::move(_Val));
    }

    _Mycond.notify_one();
}

template <typename _Ty, typename _Comparator>
__INLINE__ void priority_blocking_queue<_Ty, _Comparator>::push(const value_type& _Val)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.push(_Val);
    }

    _Mycond.notify_one();
}

template <typename _Ty, typename _Comparator> template <typename... _ValArgs>
__INLINE__ void priority_blocking_queue<_Ty, _Comparator>::emplace(_ValArgs&&... _Args)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.emplace(std::forward<_ValArgs>(_Args)...);
    }

    _Mycond.notify_one();
}

template <typename _Ty, typename _Comparator>
__INLINE__ bool priority_blocking_queue<_Ty, _Comparator>::pop(value_type& _Val, uint32_t _Timeout/* = -1*/)
{
    return this->_Pop(_Val, _Timeout, [this](value_type& _Out) {
        _Out = std::move(const_cast<value_type&>(_Mycont.top()));
        _Mycont.pop();
    });
}

}  // namespace stdutil

#endif  // __STDCOLLS_INL__