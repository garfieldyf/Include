///////////////////////////////////////////////////////////////////////////////
// stdcolls.inl
//
// Author : Garfield
// Creation Date : 2020/4/12

#ifndef __STDCOLLS_INL__
#define __STDCOLLS_INL__

#ifndef __STDCOLLS_H__
    #error stdcolls.inl requires stdcolls.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the container class
//

template <typename _TBase> template <typename _Iter>
__INLINE__ container<_TBase>::container(_Iter _First, _Iter _Last)
    : _TBase(_First, _Last)
{
}

template <typename _TBase>
__INLINE__ container<_TBase>::container(std::initializer_list<value_type> _List)
    : _TBase(_List)
{
}

template <typename _TBase>
__INLINE__ bool container<_TBase>::erase(const value_type& _Val)
{
    const auto _Last  = end();
    const auto _Where = std::find(begin(), _Last, _Val);
    const bool _Found = (_Where != _Last);
    if (_Found) {
        erase(_Where);
    }

    return _Found;
}

template <typename _TBase> template <typename _Predicate>
__INLINE__ bool container<_TBase>::erase_if(_Predicate _Pred)
{
    const auto _Last  = end();
    const auto _Where = std::find_if(begin(), _Last, _Pred);
    const bool _Found = (_Where != _Last);
    if (_Found) {
        erase(_Where);
    }

    return _Found;
}

template <typename _TBase>
__INLINE__ typename container<_TBase>::size_type container<_TBase>::remove(const value_type& _Val)
{
    const auto _Last  = end();
    const auto _First = std::remove(begin(), _Last, _Val);
    const auto _Count = _Last - _First;
    if (_Count > 0) {
        erase(_First, _Last);
    }

    return _Count;
}

template <typename _TBase> template <typename _Predicate>
__INLINE__ typename container<_TBase>::size_type container<_TBase>::remove_if(_Predicate _Pred)
{
    const auto _Last  = end();
    const auto _First = std::remove_if(begin(), _Last, _Pred);
    const auto _Count = _Last - _First;
    if (_Count > 0) {
        erase(_First, _Last);
    }

    return _Count;
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
__INLINE__ bool _Blocking_container<_Container>::erase(const value_type& _Val)
{
    mutex_lock _Lock(_Mymutex);
    return _Mycont.erase(_Val);
}

template <typename _Container> template <typename _Predicate>
__INLINE__ bool _Blocking_container<_Container>::erase_if(_Predicate _Pred)
{
    mutex_lock _Lock(_Mymutex);
    return _Mycont.erase_if(_Pred);
}

template <typename _Container>
__INLINE__ typename _Blocking_container<_Container>::size_type _Blocking_container<_Container>::remove(const value_type& _Val)
{
    mutex_lock _Lock(_Mymutex);
    return _Mycont.remove(_Val);
}

template <typename _Container> template <typename _Predicate>
__INLINE__ typename _Blocking_container<_Container>::size_type _Blocking_container<_Container>::remove_if(_Predicate _Pred)
{
    mutex_lock _Lock(_Mymutex);
    return _Mycont.remove_if(_Pred);
}

template <typename _Container>
__INLINE__ void _Blocking_container<_Container>::swap(_Blocking_container& _Right)
{
    if (this != &_Right) {
        std::lock(_Mymutex, _Right._Mymutex);
        mutex_lock _Lock1(_Mymutex, std::adopt_lock);
        mutex_lock _Lock2(_Right._Mymutex, std::adopt_lock);
        _Mycont.swap(_Right._Mycont);
    }
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

template <typename _Ty, typename _Alloc> template <typename... _ValArgs>
__INLINE__ void blocking_deque<_Ty, _Alloc>::push_front(_ValArgs&&... _Args)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.emplace_front(std::forward<_ValArgs>(_Args)...);
    }

    _Mycond.notify_one();
}

template <typename _Ty, typename _Alloc> template <typename... _ValArgs>
__INLINE__ void blocking_deque<_Ty, _Alloc>::push_back(_ValArgs&&... _Args)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.emplace_back(std::forward<_ValArgs>(_Args)...);
    }

    _Mycond.notify_one();
}

template <typename _Ty, typename _Alloc>
__INLINE__ bool blocking_deque<_Ty, _Alloc>::pop_front(value_type& _Val, uint32_t _Timeout/* = -1*/)
{
    return _Pop(_Val, _Timeout, [this](value_type& _Out) {
        _Out = std::move(_Mycont.front());
        _Mycont.pop_front();
    });
}

template <typename _Ty, typename _Alloc>
__INLINE__ bool blocking_deque<_Ty, _Alloc>::pop_back(value_type& _Val, uint32_t _Timeout/* = -1*/)
{
    return _Pop(_Val, _Timeout, [this](value_type& _Out) {
        _Out = std::move(_Mycont.back());
        _Mycont.pop_back();
    });
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the priority_queue class
//

template <typename _Ty, typename _Container, typename _Comparator> template <typename _Iter>
__INLINE__ priority_queue<_Ty, _Container, _Comparator>::priority_queue(_Iter _First, _Iter _Last)
    : super(_First, _Last)
{
}

template <typename _Ty, typename _Container, typename _Comparator>
__INLINE__ priority_queue<_Ty, _Container, _Comparator>::priority_queue(std::initializer_list<_Ty> _List, const _Comparator& _Comp/* = _Comparator()*/)
    : super(_Comp, _List)
{
}

template <typename _Ty, typename _Container, typename _Comparator>
__INLINE__ void priority_queue<_Ty, _Container, _Comparator>::clear()
{
    c.clear();
}

template <typename _Ty, typename _Container, typename _Comparator>
__INLINE__ void priority_queue<_Ty, _Container, _Comparator>::shrink_to_fit()
{
    c.shrink_to_fit();
}

template <typename _Ty, typename _Container, typename _Comparator>
__INLINE__ bool priority_queue<_Ty, _Container, _Comparator>::erase(const value_type& _Val)
{
    const bool _Result = c.erase(_Val);
    if (_Result) {
        std::make_heap(c.begin(), c.end(), comp);
    }

    return _Result;
}

template <typename _Ty, typename _Container, typename _Comparator> template <typename _Predicate>
__INLINE__ bool priority_queue<_Ty, _Container, _Comparator>::erase_if(_Predicate _Pred)
{
    const bool _Result = c.erase_if(_Pred);
    if (_Result) {
        std::make_heap(c.begin(), c.end(), comp);
    }

    return _Result;
}

template <typename _Ty, typename _Container, typename _Comparator>
__INLINE__ typename priority_queue<_Ty, _Container, _Comparator>::size_type priority_queue<_Ty, _Container, _Comparator>::remove(const value_type& _Val)
{
    const auto _Count = c.remove(_Val);
    if (_Count > 0) {
        std::make_heap(c.begin(), c.end(), comp);
    }

    return _Count;
}

template <typename _Ty, typename _Container, typename _Comparator> template <typename _Predicate>
__INLINE__ typename priority_queue<_Ty, _Container, _Comparator>::size_type priority_queue<_Ty, _Container, _Comparator>::remove_if(_Predicate _Pred)
{
    const auto _Count = c.remove_if(_Pred);
    if (_Count > 0) {
        std::make_heap(c.begin(), c.end(), comp);
    }

    return _Count;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the priority_blocking_queue class
//

template <typename _Ty, typename _Container, typename _Comparator> template <typename... _ValArgs>
__INLINE__ void priority_blocking_queue<_Ty, _Container, _Comparator>::push(_ValArgs&&... _Args)
{
    {
        mutex_lock _Lock(_Mymutex);
        _Mycont.emplace(std::forward<_ValArgs>(_Args)...);
    }

    _Mycond.notify_one();
}

template <typename _Ty, typename _Container, typename _Comparator>
__INLINE__ bool priority_blocking_queue<_Ty, _Container, _Comparator>::pop(value_type& _Val, uint32_t _Timeout/* = -1*/)
{
    return _Pop(_Val, _Timeout, [this](value_type& _Out) {
        _Out = const_cast<value_type&&>(_Mycont.top());
        _Mycont.pop();
    });
}

}  // namespace stdutil

#endif  // __STDCOLLS_INL__