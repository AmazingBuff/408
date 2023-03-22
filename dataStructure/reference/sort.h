

template<typename T>
void quickSort(T* arr, int left, int right, bool(*compare_lesser)(T,T))
{
    if(left >= right)
        return;
    int head = left;
    int tail = right;
    bool swicth_ = true;
    while(head < tail)
    {
        if(swicth_)
        {
            if(compare_lesser(arr[tail], arr[head]))
            {
                T tmp = arr[head];
                arr[head] = arr[tail];
                arr[tail] = tmp;
                swicth_ = false;
                head++;
            }
            else
                tail--;
        }
        else
        {
            if(compare_lesser(arr[tail], arr[head]))
            {
                T tmp = arr[head];
                arr[head] = arr[tail];
                arr[tail] = tmp;
                swicth_ = true;
                tail--;
            }
            else
                head++;
        }
    }
    quickSort(arr, left, head - 1, compare_lesser);
    quickSort(arr, tail + 1, right, compare_lesser);
}