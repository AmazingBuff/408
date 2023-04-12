//
// Created by AmazingBuff on 2023/4/6.
//

#pragma once

template<typename Ty_Key, typename Ty_Map>
struct Iterator
{
    using Pointer = Ty_Key*;
    using Map = Ty_Map*;
public:
    Iterator(Pointer ptr, Map map) : ptr(ptr), map(map) { }
    Iterator() : ptr(nullptr), map(nullptr) { }
    Iterator(const Iterator& other)
    {
        ptr = other.ptr;
        map = other.map;
    }

    Iterator& operator=(const Iterator& other)
    {
        ptr = other.ptr;
        map = other.map;
        return *this;
    }

    Ty_Key& operator*() const
    {
        return *ptr;
    }

    Ty_Key* operator->() const
    {
        return ptr;
    }

    Iterator& operator++()
    {
        return *this;
    }

    explicit operator bool() const
    {
        return ptr != nullptr && map != nullptr;
    }

    Iterator operator++(int)&
    {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator!=(const Iterator& other) const
    {
        return ptr != other.ptr || map != other.map;
    }

    bool operator==(const Iterator& other) const
    {
        return ptr == other.ptr && map == other.map;
    }
private:
    Pointer ptr;
    Map map;
};
