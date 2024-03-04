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
    T *_content;
    size_t _size;
    size_t _capacity;
    size_t _allocation_factor;

public:
    using iterator = T *;

    LittleVector()
    {
        _content = nullptr;
        _size = 0;
        _capacity = 0;
        _allocation_factor = 2;
    }

    ~LittleVector()
    {
        clear();
        _m_allocation(0);
    }

    void SetAllocationFactor(size_t factor) { _allocation_factor = factor; }

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

    void push_back(const T &val)
    {
        if (_capacity == _size)
            reserve(_capacity == 0 ? 1 : _capacity * _allocation_factor);

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
        T *new_alloc = nullptr;

        if (n > 0)
        {
            new_alloc = reinterpret_cast<T *>(new uint8_t[n * sizeof(T)]);

            for (size_t i = 0; i < _size; ++i)
                new_alloc[i] = _content[i];
        }

        delete[] reinterpret_cast<uint8_t *>(_content);
        _content = new_alloc;

        _capacity = n;
    }
};

#endif // LittleVector_h__
