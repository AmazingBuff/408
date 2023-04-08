#include"vector.hpp"
#include"list.hpp"

template<typename T>
struct Stack
{
private:
    List<T>* topPtr = nullptr;
    uint32_t length = 0;
public:
    T top() const
    {
        assert(!empty());
        return topPtr->data;
    }

    void push(T& value)
    {
        auto newNode = new List<T>(value);
        newNode->next = topPtr;
        topPtr = newNode;
        length++;
    }

    void push(T&& value)
    {
        auto newNode = new List<T>(value);
        newNode->next = topPtr;
        topPtr = newNode;
        length++;
    }

    void pop()
    {
        assert(!empty());
        List<T>* delNode = topPtr;
        topPtr = topPtr->next;
        delete delNode;
        length--;
    }

    void destroy()
    {
        while(topPtr)
            pop();
    }

    bool empty() const
    {
        return topPtr == nullptr;
    }

    uint32_t size() const
    {
        return length;
    }
};



//ElemType = int

//page.67
//3.
bool legalIO(Vector<char>& operators)
{
    Stack<char> st;
    for(auto& op : operators)
    {
        if(op == 'I')
            st.push(op);
        else if(op == 'O')
        {
            if(st.empty())
                return false;
            else
                st.pop();
        } 
    }
    if(st.empty())
        return true;
    else
        return false;
}

//4.
bool symmetricCenter(List<char>* head)
{
    //find the middle of list
    List<char>* fast = head->next;
    List<char>* slow = head->next;
    
    while(fast && fast->next)
    {
        fast = fast->next->next;
        slow = slow->next;
    }
    
    //store former to a stack
    List<char>* cur = head->next;
    Stack<char> st;
    while(cur != slow)
    {
        st.push(cur->data);
        cur = cur->next;
    }

    List<char>* middle = nullptr;
    if(!fast)
        middle = slow;
    else
        middle = slow->next;

    while(middle)
    {
        if(middle->data == st.top())
            st.pop();
        else
            return false;
        middle = middle->next;
    }

    if(st.empty())
        return true;
    else
        return false;
}

//5.
template<typename T>
struct SharedStack
{
private:
    uint32_t length;
    uint32_t leftTop;
    uint32_t rightTop;
    T* data;
    bool leftState;
public:
    explicit SharedStack(uint32_t initLength = 10, bool leftState = true) : length(initLength),
        leftTop(0), rightTop(initLength - 1), leftState(leftState)
    {
        data = new T[initLength];
    }

    ~SharedStack()
    {
        delete[] data;
    }

    void switchState(bool is_left)
    {
        this->leftState = is_left;
    }

    void push(T& value)
    {
        assert((leftTop - rightTop) != 1);
        if(leftState)
        {
            data[leftTop] = value;
            leftTop++;
        }
        else
        {
            data[rightTop] = value;
            rightTop--;
        }
    }

    void push(T&& value)
    {
        assert((leftTop - rightTop) != 1);
        if(leftState)
        {
            data[leftTop] = value;
            leftTop++;
        }
        else
        {
            data[rightTop] = value;
            rightTop--;
        }
    }

    void pop()
    {
        assert(leftTop != 0 && rightTop != length - 1);
        if(leftState)
            leftTop--;
        else
            rightTop++;
    }

    T top() const
    {
        return leftState ? data[leftTop - 1] : data[rightTop + 1];
    }

    bool empty() const
    {
        return leftTop == 0 && rightTop == length - 1;
    }

    uint32_t size() const
    {
        return leftTop + length - 1 - rightTop;
    }

    uint32_t capacity() const
    {
        return length;
    }
};
