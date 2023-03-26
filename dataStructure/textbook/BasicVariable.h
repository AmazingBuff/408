#include<iostream>
#include<functional>
#include<assert.h>
#include<cstring>
#include<cstdlib>

using ElemType = int;

enum class Status : int
{
    ERROR      = 0,
    OK         = 1,
    TRUE       = 1,
    FLASE      = 0,
    OVER       = -2
};

using Compare = std::function<Status(ElemType, ElemType)>;
//typedef Status (*Compare)(ElemType, ElemType);

template<typename T>
struct List
{
    T data;
    List* next = nullptr;
    List(T& element) : data(element) { }
    List() : data(0) { }
};


template<typename T, typename U>
int binarySearch(U& container, T target)
{
    int left = 0;
    int right = container.size() - 1;
    while(left <= right)
    {
        int middle = (left + right) >> 1;
        if(target > container[middle])
            left = middle + 1;
        else if(target < container[middle])
            right = middle - 1;
        else
            return middle;
    }
    return -1;
}