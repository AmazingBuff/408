#include"../textbook/BasicVariable.h"

template<typename T>
struct Vector
{
private:
    T* data;
    uint32_t space;
    uint32_t length;
    void increaseCapacity()
    {
        uint32_t newSpace = space > 20 ? space + 8 : 2 * space;
        T* newData = (T*)realloc(data, newSpace * sizeof(T));
        assert(newData);
        data = newData;
        space = newSpace;
    }
public:
    Vector(uint32_t size = 0) : space(size > 10 ? size + 8 : 2 * size + 1), length(size)
    {
        data = (T*)malloc(sizeof(T) * space);
        assert(data);
    }

    ~Vector()
    {
        free(data);
    }

    void push_back(T& value)
    {
        if (full())
            increaseCapacity();
        data[length] = value;
        length++;
    }

    void push_back(T&& value)
    {
        if (full())
            increaseCapacity();
        data[length] = value;
        length++;
    }

    void pop_back()
    {
        assert(!empty());
        length--;
    }

    void clear()
    {
        length = 0;
    }

    void resize(uint32_t newSize)
    {
        T* newData = (T*)realloc(data, newSize * sizeof(T));
        assert(newData);
        data = newData;
        space = newSize;
        while (!full())
            push_back(0);
    }

    T* begin()
    {
        return data;
    }

    T* end()
    {
        return data + length;
    }

    const T* cbegin() const
    {
        return data;
    }

    const T* cend() const
    {
        return data + length;
    }

    T& operator[](int index)
    {
        assert(index > -1 && index < length);
        return data[index];
    }

    const T& operator[](int index) const
    {
        assert(index > -1 && index < length);
        return data[index];
    }

    bool full() const
    {
        return length == space;
    }

    bool empty() const
    {
        return length == 0;
    }

    uint32_t size() const
    {
        return length;
    }

    uint32_t capacity() const
    {
        return space;
    }
};
