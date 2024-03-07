/*
   Copyright 2024 JDavid(Blackhack) <davidaristi.0504@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef LittleVector_h__
#define LittleVector_h__

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstring>
#include <cstdint>
#include <stdlib.h>
#endif

template <class T>
class LittleVector
{
private:
    bool _over_allocation;
    size_t _capacity;
    size_t _size;
    T* _content;

public:
    using iterator = T*;

    explicit LittleVector()
    {
        _over_allocation = true;
        _capacity = 0;
        _size = 0;
        _content = nullptr;
    }

    explicit LittleVector(size_t n) : LittleVector()
    {
        reserve(n);
        for (size_t i = 0; i < n; ++i)
            push_back(T());
    }

    explicit LittleVector(size_t n, const T& val) : LittleVector()
    {
        insert(begin(), n, val);
    }

    LittleVector(const LittleVector& x)
    {
        _over_allocation = x._over_allocation;

        // Initialize to 0 and nullptr so reserve() works correctly
        _capacity = 0;
        _size = 0;
        _content = nullptr;
        
        reserve(x._size);
        _size = x._size;

        for (size_t i = 0; i < _size; ++i)
            _content[i] = x._content[i];   
    }

    LittleVector &operator=(const LittleVector &x)
    {
        if (this != &x)
        {
            clear();
            _over_allocation = x._over_allocation;

            // It will only re-allocate if necessary
            reserve(x._size);

            _size = x._size;

            for (size_t i = 0; i < _size; ++i)
                _content[i] = x._content[i];
        }

        return *this;
    }

    ~LittleVector()
    {
        clear();
        _m_allocation(0);
    }

    /**
     *  @brief Enable or disable over allocation.
     *  @param val bool to set true or false
     *
     *  @note By default, capacity grows by a factor of 2 to reduce allocations and copies.
     *  However, this can lead to increased memory usage, so it can be disabled.
     *  The disadvantage is that it will now cause an increase in allocations and copies,
     *  reducing speed and accelerating memory fragmentation in embedded systems.
     *  The best option is always to use void reserve(size_t n), effectively reducing
     *  allocations to one in the vector's lifetime and guaranteeing that you use only
     *  the amount of memory needed.
     */
    void SetOverAllocation(bool val) { _over_allocation = val; }

    iterator begin()
    {
        return _content;
    }

    iterator end()
    {
        return _content + _size;
    }

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    bool empty() const
    {
        return _size == 0;
    }

    void reserve(size_t n)
    {
        if (_capacity >= n)
            return;

        _m_allocation(n);
    }

    void shrink_to_fit()
    {
        if (_capacity == _size)
            return;

        _m_allocation(_size);
    }

    T& operator[](size_t n)
    {
        return _content[n];
    }

    T& at(size_t n)
    {
        if (n >= _size)
            abort(); // There are no C++ exceptions on Arduino

        return _content[n];
    }

    T& front()
    {
        return *begin();
    }

    T& back()
    {
        return *(end() - 1);
    }

    T* data()
    {
        return _content;
    }

    void push_back(const T& val)
    {
        if (_capacity == _size)
            reserve(_capacity == 0 ? 1 : (_over_allocation ? _capacity * 2 : _capacity + 1));

        _content[_size] = val;
        ++_size;
    }

    void pop_back()
    {
        if (empty())
            return;

        _content[_size - 1].~T();
        --_size;
    }

    iterator insert(iterator position, const T& val)
    {
        size_t array_index = static_cast<size_t>(position - begin());

        if (_capacity == _size)
            reserve(_capacity == 0 ? 1 : (_over_allocation ? _capacity * 2 : _capacity + 1));

        for (size_t i = _size; i > array_index; --i)
            _content[i] = _content[i - 1];

        _content[array_index] = val;

        ++_size;

        return begin() + array_index;
    }

    iterator insert(iterator position, size_t n, const T& val)
    {
        size_t array_index = static_cast<size_t>(position - begin());

        if (_capacity < _size + n)
            reserve(_capacity == 0 ? n : (_over_allocation ? __max__(_capacity * 2, _capacity + n) : _capacity + n));

        for (size_t i = _size + n - 1; i > array_index; --i)
            _content[i] = _content[i - n];

        for (size_t i = array_index; i < array_index + n; ++i)
            _content[i] = val;

        _size += n;

        return begin() + array_index;
    }

    iterator erase(iterator position)
    {
        if (empty())
            return end();

        size_t array_index = static_cast<size_t>(position - begin());

        _content[array_index].~T();

        for (size_t i = array_index; i < _size - 1; ++i)
            _content[i] = _content[i + 1];

        --_size;

        return begin() + array_index;
    }

    iterator erase(iterator first, iterator last)
    {
        if (first == last)
            return first;

        size_t first_index = static_cast<size_t>(first - begin());
        size_t last_index = static_cast<size_t>(last - begin());
        size_t diff_index = last_index - first_index;

        for (size_t i = first_index; i < _size; ++i)
        {
            if (i < last_index)
                _content[i].~T();
            if (i + diff_index < _size)
                _content[i] = _content[i + diff_index];
        }

        _size -= diff_index;

        return begin() + first_index;
    }

    void clear()
    {
        for (size_t i = 0; i < _size; ++i)
            _content[i].~T();
        _size = 0;
    }

private:
    void _m_allocation(size_t n)
    {
        T* new_alloc = nullptr;

        if (n > 0)
        {
            new_alloc = reinterpret_cast<T*>(new uint8_t[n * sizeof(T)]);

            for (size_t i = 0; i < _size; ++i)
                new_alloc[i] = _content[i];
        }

        delete[] reinterpret_cast<uint8_t*>(_content);
        _content = new_alloc;

        _capacity = n;
    }

    size_t __max__(size_t a, size_t b) { return (a > b ? a : b); }
};

#endif // LittleVector_h__
