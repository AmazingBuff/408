#include"../textbook/BasicVariable.h"
#include"stack.h"
#include"vector.h"

template<typename T>
struct CircularQueue
{
private:
    T* data;
    uint32_t front_;
    uint32_t rear_;
    uint32_t length;
public:
    CircularQueue(uint32_t size = 10) : front_(0), rear_(0), length(size)
    {
        data = new T[size];
    }

    ~CircularQueue()
    {
        delete[] data;
    }

    void enqueue(T& value)
    {
        assert(!full());
        data[rear_] = value;
        rear_ = (rear_ + 1) % length;
    }

    void enqueue(T&& value)
    {
        assert(!full());
        data[rear_] = value;
        rear_ = (rear_ + 1) % length;
    }

    void dequeue()
    {
        assert(!empty());
        front_ = (front_ + 1) % length;
    }

    T front() const
    {
        assert(!empty());
        return data[front_];
    }

    bool empty() const
    {
        return front_ == rear_;
    }

    bool full() const
    {
        return (rear_ + 1) % length == front_;
    }

    uint32_t size() const
    {
        if (front_ > rear_)
            return rear_ + length - front_;
        else
            return rear_ - front_;
    }

    uint32_t capacity() const
    {
        return length;
    }
};


template<typename T>
struct Queue
{
private:
    List<T>* front_ = nullptr;
    List<T>* rear_ = nullptr;
    uint32_t length = 0;
public:
    Queue()
    {
        front_ = new List<T>;
        rear_ = front_;
    }

    ~Queue()
    {
        delete front_;
    }

    void enqueue(T& value)
    {
        List<T>* newNode = new List<T>(value);
        rear_->next = newNode;
        rear_ = newNode;
        length++;
    }

    void enqueue(T&& value)
    {
        List<T>* newNode = new List<T>(value);
        rear_->next = newNode;
        rear_ = newNode;
        length++;
    }

    void dequeue()
    {
        assert(!empty());
        List<T>* delNode = front_->next;
        front_->next = delNode->next;
        if(rear_ == delNode)
            rear_ = front_;
        delete delNode;
        length--;
    }

    T front() const
    {
        assert(!empty());
        return front_->next->data;
    }

    bool empty() const
    {
        return front_ == rear_;
    }

    uint32_t size() const
    {
        return length;
    }

    void destroy()
    {
        while(!empty())
            dequeue();
    }
};



//2.
void reverseQueue(Queue<char>& queue)
{
    Stack<char> stack;
    while(!queue.empty())
    {
        stack.push(queue.front());
        queue.dequeue();
    }
    while(!stack.empty())
    {
        queue.enqueue(stack.top());
        stack.pop();
    }
}

//3.
template<typename T>
struct QueueFromStack
{
private:
    Stack<T> in;
    Stack<T> out;
public:
    T front()
    {
        assert(!empty());
        if(out.empty())
        {
            while(!in.empty())
            {
                out.push(in.top());
                in.pop();
            }
        }
        return out.top();
    }

    void enqueue(T& value)
    {
        in.push(value);
    }

    void enqueue(T&& value)
    {
        in.push(value);
    }

    void dequeue()
    {
        assert(!empty());
        if(out.empty())
        {
            while(!in.empty())
            {
                out.push(in.top());
                in.pop();
            }
        }
        out.pop();
    }

    bool empty() const
    {
        return in.empty() && out.empty();
    }

    uint32_t size() const
    {
        return in.size() + out.size();
    }
};

//4.
template<typename T>
struct LinkCircularQueue
{
private:
    List<T>* front_ = nullptr;
    List<T>* rear_ = nullptr;
    uint32_t length = 0;
    uint32_t space = 0;
public:
    LinkCircularQueue()
    {
        front_ = new List<T>;
        rear_ = front_;
        rear_->next = front_;
    }

    ~LinkCircularQueue()
    {
        delete front_;
    }

    void enqueue(T& value)
    {
        if (full())
        {
            List<T>* newNode = new List<T>;
            newNode->next = rear_->next;
            rear_->data = value;
            rear_->next = newNode;
            rear_ = newNode;
            space++;
        }
        else
        {
            rear_->data = value;
            rear_ = rear_->next;
        }
        length++;
    }

    void enqueue(T&& value)
    {
        if (full())
        {
            List<T>* newNode = new List<T>;
            newNode->next = rear_->next;
            rear_->data = value;
            rear_->next = newNode;
            rear_ = newNode;
            space++;
        }
        else
        {
            rear_->data = value;
            rear_ = rear_->next;
        }
        length++;
    }

    void dequeue()
    {
        assert(!empty());
        front_ = front_->next;
        length--;
    }

    T front() const
    {
        return front_->data;
    }

    bool empty() const
    {
        return front_ == rear_;
    }

    bool full() const
    {
        return rear_->next == front_;
    }

    uint32_t size() const
    {
        return length;
    }

    uint32_t capacity() const
    {
        return space;
    }
};


//Page. 92
//1.
bool match(Vector<char>& brackets)
{
    Stack<char> matchStack;
    matchStack.push(brackets[0]);
    for(uint32_t i = 1; i < brackets.size(); i++)
    {
        if(matchStack.top() == '(' && brackets[i] == ')')
            matchStack.pop();
        else if(matchStack.top() == '[' && brackets[i] == ']')
            matchStack.pop();
        else if(matchStack.top() == '{' && brackets[i] == '}')
            matchStack.pop();
        else
            matchStack.push(brackets[i]);
    }
    if(matchStack.empty())
        return true;
    else
        return false;
}

//2.

//3.
int compute(int order, int value)
{
    assert(order >= 0);
    if(order == 0)
        return 1;
    else if(order == 1)
        return 2 * value;
    else
    {
        int i = 2;
        int p0 = 1;
        int p1 = 2 * value;
        while (i <= order)
        {
            p1 = 2 * value * p1 - 2 * (i - 1) * p0;
            p0 = p1; 
            i++;
        }
        return p1; 
    }
}