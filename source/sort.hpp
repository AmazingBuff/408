#include"search.hpp"


template<typename T, typename Compare = CompareLess<T>>
void quickSort(T* arr, int left, int right)
{
    if(left >= right)
        return;
    int head = left;
    int tail = right;
    bool switch_ = true;
    while(head < tail)
    {
        if(switch_)
        {
            if(Compare()(arr[tail], arr[head]))
            {
                T tmp = arr[head];
                arr[head] = arr[tail];
                arr[tail] = tmp;
                switch_ = false;
                head++;
            }
            else
                tail--;
        }
        else
        {
            if(Compare()(arr[tail], arr[head]))
            {
                T tmp = arr[head];
                arr[head] = arr[tail];
                arr[tail] = tmp;
                switch_ = true;
                tail--;
            }
            else
                head++;
        }
    }
    quickSort(arr, left, head - 1);
    quickSort(arr, tail + 1, right);
}