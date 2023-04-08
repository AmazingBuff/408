#include<cstring>
#include<cstdlib>
#include<cassert>
#include<initializer_list>
#include<cstdint>
#include<algorithm>

template<typename T>
struct Vector
{
private:
    T* data;
    uint32_t space;
    uint32_t length;

    bool is_basic_variable() const
    {
        return typeid(T) == typeid(uint32_t) || typeid(T) == typeid(int32_t) ||
               typeid(T) == typeid(uint8_t) || typeid(T) == typeid(int8_t) ||
               typeid(T) == typeid(float) || typeid(T) == typeid(double) ||
               typeid(T) == typeid(bool) || typeid(T) == typeid(char) ||
               typeid(T) == typeid(uint64_t) || typeid(T) == typeid(int64_t);
    }
public:
    explicit Vector(uint32_t size = 0) : space(size > 10 ? size + 8 : 2 * size), length(0)
    {
        if (size == 0)
            data = nullptr;
        else
        {
            data = new T[space];
            memset(data, 0, sizeof(T) * space);
            length = space;
        }
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
            if(is_basic_variable())
                memcpy(data, &other[0], sizeof(T) * space);
            else
            {
                for(uint32_t i = 0; i < space; i++)
                    data[i] = other[i];
            }
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
            if(is_basic_variable())
                memcpy(data, first, sizeof(T) * space);
            else
            {
                for(uint32_t i = 0; i < space; i++)
                    data[i] = first[i];
            }
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
            if(is_basic_variable())
            {
                memcpy(data, &first[0], sizeof(T) * first.size());
                memcpy(data + first.size(), &second[0], sizeof(T) * second.size());
            }
            else
            {
                uint32_t i = 0;
                for(; i < first.size(); i++)
                    data[i] = first[i];
                for(uint32_t j = 0; j < second.size(); j++)
                    data[i + j] = second[j];
            }
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

    explicit Vector(const T& value, uint32_t size = 1) : length(size), space(size)
    {
        data = new T[size];
        for(uint32_t i = 0; i < size; i++)
            data[i] = value;
    }

    Vector<T>& operator=(const Vector<T>& other)
    {
        if(this == &other)
            return *this;
        if (space)
            delete[] data;
        space = other.capacity();
        length = other.size();
        if (space)
        {
            data = new T[space];
            if(is_basic_variable())
                memcpy(data, &other[0], sizeof(T) * space);
            else
            {
                for(uint32_t i = 0; i < space; i++)
                    data[i] = other[i];
            }
        }
        else
            data = nullptr;
        return *this;
    }

    uint32_t find(T& element) const
    {
        uint32_t i = 0;
        for (; i < length; i++)
        {
            if (data[i] == element)
                return i;
        }
        return length;
    }

    uint32_t findMin(bool(*compare_lesser)(T, T)) const
    {
        T min = data[0];
        uint32_t ret = 0;
        for (uint32_t i = 1; i < length; i++)
        {
            if (compare_lesser(data[i], min))
            {
                min = data[i];
                ret = i;
            }
        }
        return ret;
    }

    uint32_t findMin(bool(*compare_lesser)(T, T), const uint32_t& exceptedIndex) const
    {
        T min = data[0];
        uint32_t ret = 0;
        for (uint32_t i = 1; i < length; i++)
        {
            if (compare_lesser(data[i], min) && i != exceptedIndex)
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
        {
            uint32_t newSpace = space > 20 ? space + 8 : 2 * space + 1;
            reserve(newSpace);
        }
        data[length] = value;
        length++;
    }

    void push_back(T&& value)
    {
        if (full())
        {
            uint32_t newSpace = space > 20 ? space + 8 : 2 * space + 1;
            reserve(newSpace);
        }
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
        if(newSize <= space)
            length = newSize;
        else
            reserve(newSize);
    }

    T back() const
    {
        return data[length - 1];
    }

    T front() const
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

    T& operator[](uint32_t index)
    {
        assert(index < length);
        return data[index];
    }

    const T& operator[](uint32_t index) const
    {
        assert(index < length);
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

    void destroy()
    {
        delete[] data;
        data = nullptr;
        length = 0;
        space = 0;
    }

    void reserve(uint32_t newSpace)
    {
        T* newData = new T[newSpace];
        if (data)
        {
            if(is_basic_variable())
                memcpy(newData, data, sizeof(T) * length);
            else
            {
                for (uint32_t i = 0; i < length; i++)
                    newData[i] = data[i];
            }
            delete[] data;
        }
        data = newData;
        space = newSpace;
    }

    T* statistics() const
    {
        return data;
    }
};