#include<cstdlib>
#include<iostream>
#include<cassert>
#include<cstring>

struct String
{
private:
    char* str;
    uint32_t length = 0;
public:
    explicit String(uint32_t size = 0) : length(size)
    {
        str = new char[size + 1];
        memset(str, 0, sizeof(char) * (size + 1));
    }

    String(uint32_t size, const char value) : length(size)
    {
        str = new char[size + 1];
        memset(str, value, sizeof(char) * size);
        str[size] = '\0';
    }

    ~String()
    {
        delete[] str;
    }

    String(const String& other)
    {
        length = other.size();
        str = new char[length + 1];
        memset(str, 0, sizeof(char) * (length + 1));
        strcpy(str, &other[0]);
    }

    explicit String(const char* other)
    {
        length = strlen(other);
        str = new char[length + 1];
        memset(str, 0, sizeof(char) * (length + 1));
        strcpy(str, other);
    }

    char* begin()
    {
        return str;
    }

    char* end()
    {
        return str + length - 1;
    }

    const char* begin() const
    {
        return str;
    }

    const char* end() const
    {
        return str + length - 1;
    }

    char& operator[](int index)
    {
        assert(index > -1 && index < length);
        return str[index];
    }

    const char& operator[](int index) const
    {
        assert(index > -1 && index < length);
        return str[index];
    }

    String& operator=(const String& other)
    {
        if (this == &other)
            return *this;
        delete[] str;
        length = other.size();
        str = new char[length + 1];
        memset(str, 0, sizeof(char) * (length + 1));
        strcpy(str, &other[0]);
        return *this;
    }

    String& operator+(const String& other)
    {
        uint32_t newSize = other.size() + length;
        char* newStr = new char[newSize + 1];
        assert(newStr);
        memset(newStr, 0, sizeof(char) * (newSize + 1));
        memcpy(newStr, str, sizeof(char) * length);
        memcpy(newStr + length, &other[0], sizeof(char) * other.size());
        char* delStr = str;
        str = newStr;
        length = newSize;
        delete[] delStr;
        return *this;
    }

    String& operator=(const char* other)
    {
        delete[] str;
        length = strlen(other);
        str = new char[length + 1];
        memset(str, 0, sizeof(char) * (length + 1));
        strcpy(str, other);
        return *this;
    }

    bool operator==(const String& other) const
    {
        if (this == &other)
            return true;
        return strcmp(str, &other[0]) == 0;
    }

    bool operator!=(const String& other) const
    {
        return !operator==(other);
    }

    bool operator>(const String& other) const
    {
        return strcmp(str, &other[0]) > 0;
    }

    bool operator<(const String& other) const
    {
        return strcmp(str, &other[0]) < 0;
    }

    String substr(const uint32_t pos, const uint32_t size) const
    {
        String ret(size);
        memcpy(&ret[0], &str[pos], size * sizeof(char));
        return ret;
    }

    uint32_t find(const char* sub, uint32_t pos = 0) const
    {
        //obtain next and nextval array
        uint32_t size = strlen(sub);
        assert(size > 0);
        int* next = new int[size];
        int* nextval = new int[size];
        next[0] = -1;
        next[1] = 0;
        nextval[0] = -1;
        for (uint32_t j = 1; j < size - 1; j++)
        {
            int k = next[j];
            if (sub[j] == sub[k])
            {
                next[j + 1] = k + 1;
                do 
                {
                    k = next[k];
                } while (k != next[0] && sub[k] == sub[j]);
                nextval[j] = k;
            }
            else 
            {
                nextval[j] = k;
                do
                {
                    k = next[k];
                    if (sub[k] == sub[j])     
                        break;
                } while (k != next[0]);
                next[j + 1] = k + 1;
            }
        }

        int k = next[size - 1];
        if (sub[size - 1] == sub[k])
        {
            do
            {
                k = next[k];
            } while (k != next[0] && sub[k] == sub[size - 1]);
        }
        nextval[size - 1] = k;

        //match with kmp
        uint32_t j = 0;
        uint32_t i = pos;
        while(i < length)
        {
            if (str[i] == sub[j])
            {
                if (j == size - 1)
                    break;
                i++;
                j++;
            }
            else
            {
                if (nextval[j] == -1)
                {
                    j = 0;
                    i++;
                }
                else
                    j = nextval[j];
            }
        }

        delete[] next;
        delete[] nextval;

        if (j == size - 1)
            return i - j;
        else
            return length;
    }

    const char* c_str() const
    {
        return str;
    }

    bool empty() const
    {
        return length == 0;
    }

    uint32_t size() const
    {
        return length;
    }

    void clear()
    {
        length = 0;
    }
};