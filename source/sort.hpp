#include"search.hpp"

//insert sort
template<typename Ty_Key, typename Compare = CompareLess<Ty_Key>>
void directlyInsertSort(Vector<Ty_Key>& arr)
{
    for(int i = 1; i < arr.size(); i++)
    {
        if(Compare()(arr[i], arr[i - 1]))
        {
            Ty_Key tmp = arr[i];
            arr[i] = arr[i - 1];
            int j = i - 2;
            while(j >= 0 && Compare()(tmp, arr[j]))
            {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = tmp;
        }
    }
}

template<typename Ty_Key, typename Compare = CompareLess<Ty_Key>>
void binaryInsertSort(Vector<Ty_Key>& arr)
{
    for(int i = 1; i < arr.size(); i++)
    {
        int left = 0;
        int right =  i - 1;
        while(left <= right)
        {
            int middle = (left + right) >> 1;
            if(Compare()(arr[i], arr[middle]))
                right = middle - 1;
            else
                left = middle + 1;
        }

        Ty_Key tmp = arr[i];
        for(int j = i; j > left; j--)
            arr[j] = arr[j - 1];
        arr[left] = tmp;
    }
}

template<typename Ty_Key, typename Compare = CompareLess<Ty_Key>>
void twoWayInsertSort(Vector<Ty_Key>& arr)
{
    Vector<Ty_Key> ret;
    ret.resize(arr.size());
    int first = 0;
    int last = 0;
    ret[0] = arr[0];
    for(int i = 1; i < arr.size(); i++)
    {
        if(Compare()(arr[i], ret[0]))
        {
            int left = first;
            int right = ret.size() - 1;
            while(left <= right)
            {
                int middle = (left + right) >> 1;
                if(Compare()(arr[i], ret[middle]))
                    right = middle - 1;
                else
                    left = middle + 1;
            }

            right = right == -1 ? ret.size() - 1 : right;
            first = first == 0 ? ret.size() - 1 : first - 1;

            for(int j = first; j < right; j++)
                ret[j] = ret[j + 1];
            ret[right] = arr[i];
        }
        else
        {
            int left = 0;
            int right = last;
            while(left <= right)
            {
                int middle = (left + right) >> 1;
                if(Compare()(arr[i], ret[middle]))
                    right = middle - 1;
                else
                    left = middle + 1;
            }

            last++;
            for(int j = last; j > left; j--)
                ret[j] = ret[j - 1];
            ret[left] = arr[i];
        }
    }

    //exchange
    auto reverse = [](Ty_Key* left, Ty_Key* right)
    {
        while(left < right)
        {
            Ty_Key tmp = *left;
            *left = *right;
            *right = tmp;
            left++;
            right--;
        }
    };

    reverse(ret.begin(), ret.begin() + last);
    reverse(ret.begin() + first, ret.end() - 1);
    reverse(ret.begin(), ret.end() - 1);

    arr = ret;
}

template<typename Ty_Key, typename Compare = CompareLess<Ty_Key>>
void shellSort(Vector<Ty_Key>& arr, const Vector<int>& dlta)
{
    for(auto& k : dlta)
    {
        for(int i = k; i < arr.size(); i++)
        {
            if(Compare()(arr[i], arr[i - k]))
            {
                Ty_Key tmp = arr[i];
                arr[i] = arr[i - k];
                int j = i - 2 * k;
                while(j >= 0 && Compare()(tmp, arr[j]))
                {
                    arr[j + k] = arr[j];
                    j -= k;
                }
                arr[j + k] = tmp;
            }
        }
    }
}


//exchange sort
template<typename Ty_Key, typename Compare = CompareLess<Ty_Key>>
void bubbleSort(Vector<Ty_Key>& arr)
{
    for(uint32_t i = arr.size() - 1; i > 0; i--)
    {
        bool exchange = false;
        for(uint32_t j = 0; j < i; j++)
        {
            if(Compare()(arr[j + 1], arr[j]))
            {
                Ty_Key tmp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = tmp;
                exchange = true;
            }
        }
        if(!exchange)
            break;
    }
}

template<typename Ty_Key, typename Compare = CompareLess<Ty_Key>>
void quickSort(Vector<Ty_Key>& arr, int left, int right)
{
    if(left >= right)
        return;
    int head = left;
    int tail = right;
    Ty_Key key = arr[head];
    while(head < tail)
    {
        while(head < tail && Compare()(key, arr[tail]))
            tail--;
        arr[head] = arr[tail];
        head++;

        while(head < tail && Compare()(arr[head], key))
            head++;
        arr[tail] = arr[head];
        tail--;
    }
    arr[head] = key;

    quickSort(arr, left, head - 1);
    quickSort(arr, tail + 1, right);
}


//select sort
template<typename Ty_Key, typename Compare = CompareLess<Ty_Key>>
void selectSort(Vector<Ty_Key>& arr)
{
    for(uint32_t i = arr.size() - 1; i > 0; i--)
    {
        uint32_t max_index = i;
        for(uint32_t j = 0; j < i; j++)
        {
            if(Compare()(arr[max_index], arr[j]))
                max_index = j;
        }
        if(max_index != i)
        {
            Ty_Key tmp = arr[i];
            arr[i] = arr[max_index];
            arr[max_index] = tmp;
        }
    }
}

template<typename Ty_Key, typename Compare = CompareLess<Ty_Key>>
void heapSort(Vector<Ty_Key>& arr)
{
    //construct heap

}