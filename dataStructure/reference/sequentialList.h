#include"../textbook/SequentialList.h"

//1. ElemType = int
Status deleteMin(SeqList& list, ElemType deleteElement)
{
    if(list.Empty())
        return Status::ERROR;
    int index = 0;
    ElemType variable = INT_MAX;
    for(int i = 0; i < list.Length(); i++)
    {
        if(list[i] < variable)
        {
            variable = list[i];
            index = i;
        }
    }
    list[index] = list[list.Length() - 1];
    deleteElement = variable;
    return Status::OK;
}

//2. ElemType = int
void reverse(SeqList& list, int left, int right)
{
    if(list.Empty() || left > list.Length() - 1 || left >= right || right < 0)
        return;
    while(left < right)
    {
        ElemType tmp = list[right];
        list[right] = list[left];
        list[left] = tmp;
    }
}

//3. ElemType = int
void deleteSame(SeqList& list, ElemType element)
{
    int fast = 0;
    int slow = 0;
    int length = list.Length();
    while(fast <= length)
    {   
        if(list[fast] != element)
        {
            if(slow != fast)
                list[slow] = list[fast];
            slow++;
        }
        fast++;
    }
    int nums = fast - slow;
    while(nums--)
        list.PopBack();
}

//4. ElemType = int
Status deleteOrderedZone(SeqList& list, ElemType lower, ElemType upper)
{
    int left = 0;
    int right = list.Length() - 1;
    if(lower > list[right] || upper < list[left] || list.Empty() || lower > upper)
        return Status::ERROR;
    //binary search the lower and upper
    while(left <= right)
    {
        int middle = (left + right) >> 1;
        if(lower <= list[middle])
            right = middle - 1;
        else
            left = middle + 1;
    }
    int lowIndex = right + 1;

    left = right + 2;
    right = list.Length() - 1;
    while(left <= right)
    {
        int middle = (left + right) >> 1;
        if(upper >= list[middle])
            left = middle + 1;
        else
            right = middle - 1;
    }
    int upIndex = left - 1;

    int length = list.Length();
    int fast = upIndex + 1;
    int slow = lowIndex;
    while(fast <= length)
    {
        list[slow] = list[fast];
        fast++;
        slow++;
    }

    int nums = upIndex - lowIndex + 1;
    while(nums--)
        list.PopBack();
}

//5. ElemType = int
Status deleteZone(SeqList& list, ElemType lower, ElemType upper)
{
    int left = 0;
    int right = list.Length() - 1;
    if(lower > list[right] || upper < list[left] || list.Empty() || lower > upper)
        return Status::ERROR;

    int fast = 0;
    int slow = 0;
    while(fast <= right)
    {
        if(!(list[fast] >= lower && list[fast] <= upper))
        {
            if(fast != slow)
                list[slow] = list[fast];
            slow++;
        }
        fast++;
    }

    int nums = fast - slow;
    while(nums--)
        list.PopBack();
}

//6. ElemType = int
void deleteRepetitive(SeqList& list)
{
    int right = list.Length() - 1;
    int fast = 0;
    int slow = 0;
    while(fast <= right)
    {
        if(list[fast] != list[slow])
        else
        {
            slow++;
            if(slow != fast)
                list[slow] = list[fast];
        }
        fast++;
    }

    int nums = fast - slow - 1;
    while(nums--)
        list.PopBack();
}

//7. ElemType = int
SeqList mergeOrderedList(SeqList& list1, SeqList& list2)
{
    SeqList out;
    out.InitList();

    int head1 = 0;
    int head2 = 0;
    while(head1 <= list1.Length() - 1 && head2 <= list2.Length() - 1)
    {
        if(list1[head1] < list2[head2])
        {
            out.PushBack(list1[head1]);
            head1++;
        }
        else
        {
            out.PushBack(list2[head2]);
            head2++;
        }
    }

    while(head1 <= list1.Length() - 1)
    {
        out.PushBack(list1[head1]);
        head1++;
    }

    while(head2 <= list2.Length() - 1)
    {
        out.PushBack(list2[head2]);
        head2++;
    }

    return out;
}

//8. ElemType = int
void exchangePosition(SeqList& list, int listM, int listN)
{
    reverse(list, 0, list.Length() - 1);
    reverse(list, 0, listM - 1);
    reverse(list, listM, list.Length() - 1);
}

//9. ElemType = int
void searchElement(SeqList& list, ElemType element)
{
    int left = 0;
    int right = list.Length() - 1;
    while(left <= right)
    {
        int middle = (left + right) >> 1;
        if(element < list[middle])
            right = middle - 1;
        else if(element > list[middle])
            left = middle + 1;
        else
        {
            ElemType tmp = list[middle];
            list[middle] = list[middle + 1];
            list[middle + 1] = tmp;
            return;
        }
    }

    list.ListInsert(left, element);
}

//10. ElemType = int
void rotate(SeqList& list, int k)
{
    //time O(n), space O(1)
    reverse(list, 0, k - 1);
    reverse(list, k, list.Length() - 1);
    reverse(list, 0, list.Length() - 1);
}

//11. ElemType = int
ElemType searchMidNumber(SeqList& list1, SeqList& list2)
{
    //precise position
    int midPosition = (list1.Length() + list2.Length()) >> 1 + 1;
    //index
    int list1Left = 0;
    int list2Left = 0;
    while(midPosition)
    {
        //get the k min number
        int middle = std::max(midPosition >> 1 - 1, 0);
        int list1Mid = list1Left + middle;
        int list2Mid = list2Left + middle;
        if(list1[list1Mid] >= list2[list2Mid])
            list2Left = list2Mid + 1;
        else
            list1Left = list1Mid + 1;
        midPosition -= middle + 1;
    }
    return 
}

//12. ElemType = int
int getMainElement(SeqList& list)
{
    std::unordered_map<ElemType, int> hash;
    for(auto& i : list)
    {
        if(hash.count(i))
            hash[i]++;
        else
        {
            if(hash.empty())
                hash.emplace(std::make_pair(i, 1));
            else
                hash.begin()->second--;
            if(hash.begin()->second == 0)
                hash.erase(hash.begin()->first);
        }
    }

    if(!hash.empty())
    {
        int count = 0;
        ElemType m = hash.begin()->first;
        for(auto& i : list)
        {
            if(i == m)
                count++;
        }
        if(count > list.Length() >> 1)
            return m;
    }
    return -1;
}

//13. ElemType = int
ElemType getMinPositive(SeqList& list)
{
    std::vector<ElemType> storage(list.Length());
    for(auto& i : list)
    {
        if(i <= list.Length() && i > 0)
            storage[i - 1]++;
    }

    for(int i = 0; i < list.Length(); i++)
    {
        if(storage[i] == 0)
            return i + 1;
    }
    return list.Length() + 1;
}

//14. ElemType = int
 
