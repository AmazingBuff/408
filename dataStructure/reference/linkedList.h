#include"../textbook/BasicVariable.h"
#include"Vector.h"

struct LinkedList
{
    ElemType data;
    LinkedList* next = nullptr;
    LinkedList(ElemType& element) : data(element) { }
    LinkedList() : data(-1) { }
};

struct LoopLinkedList
{
    LinkedList* head = nullptr;
    LinkedList* rear = nullptr;
};


struct DLinkedList
{
    ElemType data;
    DLinkedList* prior = nullptr;
    DLinkedList* next = nullptr;
    DLinkedList(ElemType& element) : data(element) { }
    DLinkedList() : data(0) { }
};



//ElemType = int

//1.
LinkedList* deleteValue(LinkedList* list, ElemType value)
{
    if(!list)
        return nullptr;
    
    LinkedList* r = deleteValue(list->next, value);
    list->next = r;
    if(list->data == value)
    {
        LinkedList* ret = list->next;
        delete list;
        return ret;
    }
    else
        return list;
}

//2.
void deleteValueWithHead(LinkedList* head, ElemType value)
{
    LinkedList* cur = head->next;
    LinkedList* prev = cur;
    while(cur)
    {
        if(cur->data == value)
        {
            prev->next = cur->next;
            delete cur;
            cur = prev->next;
        }
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
}

//3.
void reverseOutput(LinkedList* head, Vector<ElemType>& output)
{
    if(!head)
        return;
    reverseOutput(head->next, output);
    output.push_back(head->data);
}

//4.
void deleteMinValue(LinkedList* head)
{
    LinkedList* fast = head;
    LinkedList* slow = fast;
    while(fast->next)
    {
        if(fast->next->data < slow->data)
            slow = fast;
        fast = fast->next;
    }
    LinkedList* del = slow->next;
    slow->next = del->next;
    delete del;
}

//5.
LinkedList* reverseNone(LinkedList* head)
{
    if(!head->next || !head->next->next)
        return head;
    LinkedList* prev = head->next;
    LinkedList* cur = prev->next;
    bool isFirst = true;
    while(cur)
    {
        LinkedList* mid = cur;
        cur = cur->next;
        mid->next = prev;
        if(isFirst)
        {
            prev->next = nullptr;
            isFirst = false;
        }
        prev = mid;
    }
    head->next = prev;
    return head;
}

//6.
LinkedList* sortLesser(LinkedList* head)
{
    LinkedList* prev = head->next;
    LinkedList* newOne = head;

    while(prev)
    {
        ElemType min = INT_MIN;
        LinkedList* cur = prev;
        while(cur)
        {
            if(cur->data < min)
                min = cur->data;
            cur = cur->next;
        }
        cur = prev;
        if(cur->data == min)
        {
            newOne->next = cur;
            newOne = newOne->next;
        }   
        else
        {
            while(cur->next)
            {
                if(cur->next->data == min)
                {
                    newOne->next = cur->next;
                    cur->next = cur->next->next;
                    newOne = newOne->next;
                }
                cur = cur->next;
            }
        }
        prev = prev->next;
    }
    return head;
}

//7.
LinkedList* deleteRangeNode(LinkedList* head, ElemType lesser, ElemType more)
{
    LinkedList* cur = head;
    while(cur->next)
    {
        if(cur->next->data > lesser && cur->next->data < more)
        {
            LinkedList* mid = cur->next;
            cur->next = cur->next->next;
            delete mid;
        }
        else
            cur = cur->next;
    }
    return head;
}

//8.
LinkedList* findPublicNode(LinkedList* list1, LinkedList* list2)
{
    LinkedList* head1 = list1;
    LinkedList* head2 = list2;
    while(head1 != head2)
    {
        head1 = head1->next;
        if(!head1)
            head1 = list2;
        head2 = head2->next;
        if(!head2)
            head2 = list1;
    }
    return head1;
}

//9.
Vector<ElemType> printOrdered(LinkedList* head)
{
    LinkedList* list = sortLesser(head);
    Vector<ElemType> out;
    while(list->next)
    {
        out.push_back(list->next->data);
        LinkedList* mid = list->next;
        list = list->next;
        delete mid;
    }
    return out;
}

//10.
void devideTow(LinkedList* head, LinkedList* odd, LinkedList* even)
{
    while(head->next)
    {
        odd->next = head->next;
        if(head->next->next)
        {
            even->next = head->next->next;
            head = head->next->next;
        }
        else
            break;
    }
}

//11.
void devideTowReverse(LinkedList* head, LinkedList* listA, LinkedList* listB)
{
    devideTow(head, listA, listB);
    listB = reverseNone(listB);
}

//12.
LinkedList* deleteSameValue(LinkedList* list)
{
    if(!list && !list->next)
        return list;
    LinkedList* slow = list;
    LinkedList* fast = slow->next;
    while(fast)
    {
        if(slow->data == fast->data)
        {
            fast = fast->next;
            slow->next = fast;
        }
        else
        {
            slow = fast;
            fast = fast->next;
        }
    }
    return list;
}


//13.
LinkedList* mergeTwoOrderedList(LinkedList* list1, LinkedList* list2)
{
    //merge
    LinkedList* newHead = new LinkedList;
    LinkedList* cur = newHead;
    LinkedList* head1 = list1;
    LinkedList* head2 = list2;
    while(head1 && head2)
    {
        if(head1->data > head2->data)
        {
            cur->next = head2;
            head2 = head2->next;
        }
        else
        {
            cur->next = head1;
            head1 = head1->next;
        }
        cur = cur->next;
    }
    if(head1)
        cur->next = head1;
    else if(head2)
        cur->next = head2;
    
    //reverse
    LinkedList* prev = newHead->next;
    if(prev->next)
    {
        cur = prev->next;
        bool isFirst = true;
        while(cur)
        {
            LinkedList* mid = cur;
            cur = cur->next;
            mid->next = prev;
            if(isFirst)
            {
                prev->next = nullptr;
                isFirst = false;
            }
            prev = mid;
        }
    }
    delete newHead;
    return prev;
}

//14.
LinkedList* generateFromPublicNode(LinkedList* head1, LinkedList* head2)
{
    LinkedList* newHead = new LinkedList;
    LinkedList* cur = newHead;
    while(head1->next && head2->next)
    {
        if(head1->next->data > head2->next->data)
            head2 = head2->next;
        else if(head1->next->data < head2->next->data)
            head1 = head1->next;
        else
        {
            LinkedList* newNode = new LinkedList(head1->next->data);
            cur->next = newNode;
            cur = cur->next;
            head1 = head1->next;
            head2 = head2->next;
        }
    }
    return newHead;
}

//15.
void generatePublicNode(LinkedList* head1, LinkedList* head2)
{
    LinkedList* cur = head1;
    while(head1->next && head2->next)
    {
        if(head1->next->data > head2->next->data)
            head2 = head2->next;
        else if(head1->next->data < head2->next->data)
            head1 = head1->next;
        else
        {
            cur->next = head1->next;
            cur = cur->next;
            head1 = head1->next;
            head2 = head2->next;
        }
    }
}

//16.
bool continuousSubsequence(LinkedList* list1, LinkedList* list2)
{
    bool isFirst = true;
    while(list1)
    {
        if(list1->data == list2->data)
        {
            list2 = list2->next;
            isFirst = false;
        }
        else if(!isFirst)
            return false;
        list1 = list1->next;
    }
    if(list2)
        return false;
    else
        return true;
}


//17.
bool symmetricDList(DLinkedList* head)
{
    if(head->next == head || head->next->next == head)
        return true;
    DLinkedList* last = head->prior;
    DLinkedList* first = head->next;
    while(first != last && first->next != last)
    {
        if(first->data != last->data)
            return false;
        else
        {
            first = first->next;
            last = last->prior;
        }
    }
    return true;
}


//18.
LinkedList* linkTwoList(LinkedList* head1, LinkedList* head2)
{
    //find list1's tail
    LinkedList* list1 = head1;
    while(list1->next != head1)
        list1 = list1->next;
    //find list2's tail
    LinkedList* list2 = head2;
    while(list2->next != head2)
        list2 = list2->next;
    list1->next = head2;
    list2->next = head1;
}


//19.
Vector<ElemType> outputMinValue(LinkedList* head)
{
    LinkedList* prev = head;
    Vector<ElemType> out;
    while(prev->next)
    {
        LinkedList* cur = prev->next;
        ElemType min = INT_MIN;
        while(cur)
        {
            if(cur->data < min)
                min = cur->data;
            cur = cur->next;
        }
        out.push_back(min);
        cur = prev;
        if(cur->data == min)
        {
            prev->next = cur->next;
            delete cur;
        }
        else
        {
            while(cur->next->data != min)
                cur = cur->next;
            LinkedList* tmp = cur->next;
            cur->next = tmp->next;
            delete tmp;
        }
    }
    return out;
}

//20.
struct LRUCache
{
    ElemType data;
    LRUCache* prior = nullptr;
    LRUCache* next = nullptr;
    uint32_t frequency = 0;
    LRUCache() : data(0) { }
    LRUCache(ElemType& element) : data(element) { }
};

LRUCache* Locate(LRUCache* head, ElemType value)
{
    LRUCache* prev = head;
    LRUCache* cur = prev;
    //find the position of the value
    while(prev)
    {
        if(prev->data == value)
        {
            prev->frequency++;
            break;
        }
        prev = prev->next;
    }
    //find the first position of same frequency
    while(cur)
    {
        if(cur->frequency > prev->frequency)
            cur = cur->next;
        else
            break;
    }
    //handle postion relation
    prev->prior->next = prev->next;
    prev->next->prior = prev->prior;

    cur->prior->next = prev;
    prev->prior = cur->prior;
    prev->next = cur;
    cur->prior = prev;

    return prev;
}


//21.
bool ring(LinkedList* list)
{
    LinkedList* fast = list;
    LinkedList* slow = fast;
    while(fast)
    {
        if(!fast->next)
            return false;
        fast = fast->next->next;
        slow = slow->next;
        if(fast == slow)
            return true;
    }
    return false;
}

//22.
ElemType lastkth(LinkedList* list, int k)
{
    LinkedList* fast = list;
    LinkedList* slow = fast;
    int n = 0;
    while(fast)
    {
        fast = fast->next;
        n++;
        if(n >= k)
            slow = slow->next;
    }
    if(n <= k)
        return slow->data;
    else
        return 0;
}

//23.
LinkedList* startPosition(LinkedList* head1, LinkedList* head2)
{
    LinkedList* list1 = head1->next;
    LinkedList* list2 = head2->next;
    while(list1 != list2)
    {
        list1 = list1->next;
        if(!list1)
            list1 = head2->next;
        list2 = list2->next;
        if(!list2)
            list2 = head1->next;
    }
    return list1;
}

//24.
void deleteRepetitiveNode(LinkedList* head, uint32_t n, uint32_t m)
{
    std::unordered_map<uint32_t, uint32_t> storage;
    while(head->next)
    {
        uint32_t value = abs(head->next->data);
        if(storage.count(value))
        {
            LinkedList* tmp = head->next;
            head->next = head->next->next;
            delete tmp;
        }
        else
            storage.emplace(std::make_pair(value, 1));
    }
}

//25
LinkedList* reverseList(LinkedList* list)
{
    if(!list)
        return nullptr;
    LinkedList* newNode = reverseList(list->next);
    if(newNode != nullptr)
        newNode->next = list;
    return list;
}

void changeElement(LinkedList* head)
{
    //find middle node
    LinkedList* fast = head;
    LinkedList* slow = fast;
    while(fast && fast->next)
    {
        fast = fast->next->next;
        slow = slow->next;
    }
    LinkedList* second = slow->next;
    slow->next = nullptr;
    //reverse second list
    LinkedList* last = second;
    while (last->next)
        last = last->next;
    reverseList(second);
    second->next = nullptr;

    //insert second to first
    LinkedList* prev = head->next;
    LinkedList* cur = prev->next;
    while(cur)
    {
        cur = cur->next;
        prev->next = last;
        if(!last)
            last = last->next;
        prev->next->next = cur;
        prev = cur;
    }
}




