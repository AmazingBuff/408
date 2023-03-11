#include"BasicVariable.h"

const uint32_t LIST_INIT_SIZE = 100;
const uint32_t LISTINCREMENT = 10;

class SeqList
{
private:
    ElemType* firstElement;     //the first location of storage space
    int       length;      //current length of sequential list
    int       listSize;    //the storage space of allocation

public:
    //initialize
    Status InitList()
    {
        firstElement = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
        if(!firstElement)
            exit(static_cast<int>(Status::OVER));
        length = 0;
        listSize = LIST_INIT_SIZE;
        return Status::OK;
    }

    //insert a element before the location
    Status ListInsert(int location, ElemType element)
    {
        if(location < 0 || location > length)
            return Status::ERROR;
        if(length >= listSize)
        {
            ElemType* newBase = (ElemType*)realloc(firstElement, (listSize + LISTINCREMENT) * sizeof(ElemType));
            if(!newBase)
                exit(static_cast<int>(Status::ERROR));
            firstElement = newBase;
            listSize += LISTINCREMENT;
        }
        ElemType* locationElement = firstElement + location;
        //form back to front
        ElemType* changedElement = firstElement + length;
        while(changedElement > locationElement)       
        {
            *changedElement = *(changedElement - 1);
            changedElement--;
        }
        *locationElement = element;
        length++;

        return Status::OK;
    }

    //delete the element in the location
    Status ListDelete(int location)
    {
        if(location < 0 || location > length - 1)
            return Status::ERROR;
        ElemType* locationElement = firstElement + location;
        ElemType* lastElement = firstElement + length - 1;
        while(locationElement < lastElement)
            *locationElement = *(locationElement + 1);
        length--;
        return Status::OK;
    }

    //pop_back
    Status PopBack()
    {
        if(Empty())
            return Status::ERROR;
        length--;
        return Status::OK;
    }

    //push_back
    Status PushBack(ElemType element)
    {
        if(length >= listSize)
        {
            ElemType* newBase = (ElemType*)realloc(firstElement, (listSize + LISTINCREMENT) * sizeof(ElemType));
            if(!newBase)
                exit(static_cast<int>(Status::ERROR));
            firstElement = newBase;
            listSize += LISTINCREMENT;
        }

        *(firstElement + length) = element;
        length++;
        return Status::OK;
    }

    //find the first element which satisfy the compare function
    //compare will be like compare(element, value)
    //return the index if find while -1 if not
    int LookUp(ElemType element, Compare compare)
    {
        for(int i = 0; i < length; i++)
        {
            if(compare(element, *(firstElement + i)) == Status::TRUE)
                return i;
            else
                i++;
        }
        return -1;
    }

    ElemType operator[](int i)
    {
        assert(i >= 0 && i <= length - 1);
        return *(firstElement + i);
    } 

    const ElemType operator[](int i) const
    {
        assert(i >= 0 && i <= length - 1);
        return *(firstElement + i);
    }

    int Capacity() const
    {
        return listSize;
    }

    int Length() const
    {
        return length;
    }

    bool Empty() const
    {
        return length == 0;
    }

    ElemType* begin() const
    {
        return firstElement;
    }

    ElemType* end() const
    {
        return firstElement + length;
    }
};
