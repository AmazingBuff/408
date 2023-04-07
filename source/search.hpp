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
