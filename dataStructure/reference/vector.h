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
        uint32_t newSpace = space > 20 ? space + 8 : 2 * space + 1;
        T* newData = new T[newSpace];
        memcpy(newData, data, space);
        delete[] data;
        data = newData;
        space = newSpace;
    }
public:
    Vector(uint32_t size = 0) : space(size > 10 ? size + 8 : 2 * size), length(0)
    {
        if (size == 0)
            data = nullptr;
        else
            data = new T[space];
    }

    ~Vector()
    {
        if(space)
            delete[] data;
    }

    Vector(const Vector<T>& other)
    {
        space = other.capacity();
        length = other.size();
        if (space)
        {
            data = new T[space];
            memcpy(data, &other[0], sizeof(T) * space);
        }
        else
            data = nullptr;
    }

    Vector(T* first, T* last)
    {
        length = last - first;
        space = length;
        if (space)
        {
            data = new T[space];
            memcpy(data, first, sizeof(T) * space);
        }
        else
            data = nullptr;
    }

    Vector(Vector<T>& first, Vector<T>& second)
    {
        length = first.size() + second.size();
        space = length;
        if (space)
        {
            data = new T[space];
            memcpy(data, &first[0], sizeof(T) * first.size());
            memcpy(data + first.size(), &second[0], sizeof(T) * second.size());
        }
        else
            data = nullptr;
    }

    Vector(const std::initializer_list<T>& list)
    {
        length = list.size();
        space = length;
        data = new T[space];
        auto iterator = list.begin();
        for (uint32_t i = 0; i < list.size(); i++)
            data[i] = *iterator++;
    }

    Vector<T>& operator=(const Vector<T>& other)
    {
        if (space)
            delete[] data;
        space = other.capacity();
        length = other.size();
        if (space)
        {
            data = new T[space];
            memcpy(data, &other[0], sizeof(T) * space);
        }
        else
            data = nullptr;
        return *this;
    }

    uint32_t find(T& element) const
    {
        for (uint32_t i = 0; i < length; i++)
        {
            if (data[i] == element)
                return i;
        }
        return length;
    }

    uint32_t findMin(bool(*compare_lesser)(T,T)) const
    {
        T min = data[0];
        uint32_t ret = 0;
        for(uint32_t i = 1; i < length; i++)
        {
            if(compare_lesser(data[i], min))
            {
                min = data[i];
                ret = i;
            }
        }
        return ret;
    }

    uint32_t findMin(bool(*compare_lesser)(T,T), const T& exception) const
    {
        T min = data[0];
        uint32_t ret = 0;
        for(uint32_t i = 1; i < length; i++)
        {
            if(compare_lesser(data[i], min) && data[i] != exception)
            {
                min = data[i];
                ret = i;
            }
        }
        return ret;
    }

    uint32_t findMin(bool(*compare_lesser)(T,T), const uint32_t exceptedIndex) const
    {
        T min = data[0];
        uint32_t ret = 0;
        for(uint32_t i = 1; i < length; i++)
        {
            if(compare_lesser(data[i], min) && i != exceptedIndex)
            {
                min = data[i];
                ret = i;
            }
        }
        return ret;
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
        T* newData = new T[newSize];
        if (data)
            memcpy(newData, data, newSize * sizeof(T));
        data = newData;
        space = newSize;
        if (space > length)
            memset(data + length, 0, sizeof(T) * (space - length));
    }

    T back() const
    {
        return data[length - 1];
    }

    T head() const
    {
        return data[0];
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

    T* statistics() const
    {
        return data;
    }
};