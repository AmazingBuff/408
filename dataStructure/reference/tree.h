#include"../textbook/BasicVariable.h"
#include"queue.h"
#include"vector.h"

template<typename T>
struct BinaryTree
{
private:
    TreeNode<T>* root = nullptr;

    void preOrderRecursion(TreeNode<T>* node, void(*visit)(T, Vector<T>&), Vector<T>& arr) const
    {
        if (node == nullptr)
            return;
        else
        {
            visit(node->data, arr);
            preOrderRecursion(node->left, visit, arr);
            preOrderRecursion(node->right, visit, arr);
        }
    }

    void inOrderRecursion(TreeNode<T>* node, void(*visit)(T, Vector<T>&), Vector<T>& arr) const
    {
        if (node == nullptr)
            return;
        else
        {
            inOrderRecursion(node->left, visit, arr);
            visit(node->data, arr);
            inOrderRecursion(node->right, visit, arr);
        }
    }

    void postOrderRecursion(TreeNode<T>* node, void(*visit)(T, Vector<T>&), Vector<T>& arr) const
    {
        if (node == nullptr)
            return;
        else
        {
            postOrderRecursion(node->left, visit, arr);
            postOrderRecursion(node->right, visit, arr);
            visit(node->data, arr);
        }
    }

    void preOrderStack(TreeNode<T>* node, void(*visit)(T, Vector<T>&), Vector<T>& arr) const
    {
        TreeNode<T>* cur = node;
        Stack<TreeNode<T>*> stack;
        while (cur || !stack.empty())
        {
            if (cur)
            {
                visit(cur->data, arr);
                stack.push(cur);
                cur = cur->left;
            }
            else
            {
                cur = stack.top();
                stack.pop();
                cur = cur->right;
            }
        }
    }

    void inOrderStack(TreeNode<T>* node, void(*visit)(T, Vector<T>&), Vector<T>& arr) const
    {
        TreeNode<T>* cur = node;
        Stack<TreeNode<T>*> stack;
        while (cur || !stack.empty())
        {
            if (cur)
            {
                stack.push(cur);
                cur = cur->left;
            }
            else
            {
                cur = stack.top();
                visit(cur->data, arr);
                stack.pop();
                cur = cur->right;
            }
        }
    }

    void postOrderStack(TreeNode<T>* node, void(*visit)(T, Vector<T>&), Vector<T>& arr) const
    {
        TreeNode<T>* prev = node;
        TreeNode<T>* cur = prev;
        Stack<TreeNode<T>*> stack;
        while (cur || !stack.empty())
        {
            if (cur)
            {
                stack.push(cur);
                cur = cur->left;
            }
            else
            {
                cur = stack.top();
                if (!cur->right || prev == cur->right)
                {
                    prev = cur;
                    visit(cur->data, arr);
                    stack.pop();
                    cur = nullptr;
                }
                else
                    cur = cur->right;
            }
        }
    }

    void levelOrder(TreeNode<T>* node, void(*visit)(T, Vector<T>&), Vector<T>& arr) const
    {
        Queue<TreeNode<T>*> queue;
        queue.enqueue(node);
        while (!queue.empty())
        {
            TreeNode<T>* front = queue.front();
            visit(front->data, arr);
            if(front->left)
                queue.enqueue(front->left);
            if(front->right)
                queue.enqueue(front->right);
            queue.dequeue();
        }
    }
    uint32_t depthOrder(TreeNode<T>* node) const
    {
        if (node == nullptr)
            return 0;
        return std::max(depthOrder(node->left), depthOrder(node->right)) + 1;
    }
public:
    enum class Mode
    {
        preOrderRecursion,
        inOrderRecursion,
        postOrderRecursion,
        preOrderStack,
        inOrderStack,
        postOrderStack,
        levelOrder
    };

    BinaryTree() { }
    ~BinaryTree()
    {
        if (!empty())
            destroy();
    }

    BinaryTree(const BinaryTree<T>& other)
    {
        auto visit = [](T value, Vector<T>& arr)
        {
            arr.push_back(value);
        };
        Vector<T> values = other.traverse(visit, BinaryTree<T>::Mode::levelOrder);
        create(values);
    }

    BinaryTree<T>& operator=(const BinaryTree<T>& other)
    {
        if (this == &other)
            return *this;
        else
        {
            auto visit = [](T value, Vector<T>& arr)
            {
                arr.push_back(value);
            };
            Vector<T> values = other.traverse(visit, BinaryTree<T>::Mode::levelOrder);
            create(values);
        }
    }

    void create(Vector<T>& values)
    {
        assert(!values.empty());
        assert(empty());
        Queue<TreeNode<T>*> queue;
        root = new TreeNode<T>(values[0]);
        queue.enqueue(root);
        uint32_t count = 1;
        while (!queue.empty())
        {
            TreeNode<T>* front = queue.front();
            front->left = new TreeNode<T>(values[count]);
            queue.enqueue(front->left);
            count++;
            if (count == values.size())
            {
                queue.destroy();
                break;
            }
            front->right = new TreeNode<T>(values[count]);
            queue.enqueue(front->right);
            count++;
            if (count == values.size())
            {
                queue.destroy();
                break;
            }
            queue.dequeue();  
        }
    }

    void create(T* values, uint32_t size)
    {
        assert(values);
        assert(empty());
        Queue<TreeNode<T>*> queue;
        root = new TreeNode<T>(values[0]);
        queue.enqueue(root);
        uint32_t count = 1;
        while (!queue.empty())
        {
            TreeNode<T>* front = queue.front();
            front->left = new TreeNode<T>(values[count]);
            queue.enqueue(front->left);
            count++;
            if (count == size)
            {
                queue.destroy();
                break;
            }
            front->right = new TreeNode<T>(values[count]);
            queue.enqueue(front->right);
            count++;
            if (count == size)
            {
                queue.destroy();
                break;
            }
            queue.dequeue();
        }
    }

    void destroy()
    {
        Queue<TreeNode<T>*> queue;
        queue.enqueue(root);
        while (!queue.empty())
        {
            TreeNode<T>* front = queue.front();
            if (front->left)
                queue.enqueue(front->left);
            if (front->right)
                queue.enqueue(front->right);
            queue.dequeue();
            delete front;
        }
    }

    uint32_t depth() const
    {
        return depthOrder(root);
    }

    TreeNode<T>* getRoot() const
    {
        return root;
    }

    bool empty() const
    {
        return root == nullptr;
    }

    Vector<T> traverse(void(*visit)(T,Vector<T>&), Mode mode) const
    {
        assert(root);
        Vector<T> arr;
        if (mode == Mode::preOrderRecursion)
            preOrderRecursion(root, visit, arr);
        else if (mode == Mode::inOrderRecursion)
            inOrderRecursion(root, visit, arr);
        else if (mode == Mode::postOrderRecursion)
            postOrderRecursion(root, visit, arr);
        else if (mode == Mode::preOrderStack)
            preOrderStack(root, visit, arr);
        else if (mode == Mode::inOrderStack)
            inOrderStack(root, visit, arr);
        else if (mode == Mode::postOrderStack)
            postOrderStack(root, visit, arr);
        else if (mode == Mode::levelOrder)
            levelOrder(root, visit, arr);
        return arr;
    }
};


//Page 129 5
int findFirstPublicAncestor(int i, int j)
{
    while(i != j)
    {
        if(i < j)
            j = (j - 1) >> 1;
        else
            i = (i - 1) >> 1;
    }
    return i;
}

//threaded binary tree
template<typename T>
struct ThreadedTreeNode
{
    enum class Type : bool
    {
        child,
        thread
    };
    T data;
    //false for child, true for thread
    Type leftTag = Type::child;
    Type rightTag = Type::child;
    ThreadedTreeNode* left = nullptr;
    ThreadedTreeNode* right = nullptr;
    ThreadedTreeNode(T& element) : data(element) { }
    ThreadedTreeNode() : data(0) { }
};

template<typename T>
struct ThreadedBinaryTree
{
public:
    enum class Mode
    {
        preOrder,
        inOrder,
        postOrder,
        //level order is not suitable for threaded binary tree
        levelOrder
    };
private:
    ThreadedTreeNode<T>* head = nullptr;
    Mode type;

    void createThreadedTree(BinaryTree<T>& binaryTree)
    {
        TreeNode<T>* cur = binaryTree.getRoot();
        head = new ThreadedTreeNode<T>;
        ThreadedTreeNode<T>* root = new ThreadedTreeNode<T>(cur->data);
        head->left = root;
        head->leftTag = ThreadedTreeNode<T>::Type::child;
        head->right = head;
        head->rightTag = ThreadedTreeNode<T>::Type::thread;

        Queue<TreeNode<T>*> binaryQueue;
        Queue<ThreadedTreeNode<T>*> threadedQueue;
        binaryQueue.enqueue(cur);
        threadedQueue.enqueue(root);
        while (!binaryQueue.empty())
        {
            cur = binaryQueue.front();
            root = threadedQueue.front();
            if (cur->left)
            {
                binaryQueue.enqueue(cur->left);
                ThreadedTreeNode<T>* newNode = new ThreadedTreeNode<T>(cur->left->data);
                root->left = newNode;
                threadedQueue.enqueue(root->left);
            }
            if (cur->right)
            {
                binaryQueue.enqueue(cur->right);
                ThreadedTreeNode<T>* newNode = new ThreadedTreeNode<T>(cur->right->data);
                root->right = newNode;
                threadedQueue.enqueue(root->right);
            }
            binaryQueue.dequeue();
            threadedQueue.dequeue();
        }
    }

    void preOrderRecursion(ThreadedTreeNode<T>*& node, ThreadedTreeNode<T>*& prev) const
    {
        if (node == nullptr)
            return;
        else
        {
            //store left pointer to aviod revising
            ThreadedTreeNode<T>* left = node->left;
            //can't handle next for current parse
            if (!node->left)
            {
                node->leftTag = ThreadedTreeNode<T>::Type::thread;
                node->left = prev;
            }
            if (!prev->right)
            {
                prev->rightTag = ThreadedTreeNode<T>::Type::thread;
                prev->right = node;
            }
            prev = node;
            preOrderRecursion(left, prev);
            preOrderRecursion(node->right, prev);
        }
    }

    void inOrderRecursion(ThreadedTreeNode<T>*& node, ThreadedTreeNode<T>*& prev) const
    {
        if (node == nullptr)
            return;
        else
        {
            inOrderRecursion(node->left, prev);
            //can't handle next for current parse
            if (!node->left)
            {
                node->leftTag = ThreadedTreeNode<T>::Type::thread;
                node->left = prev;
            }
            if (!prev->right)
            {
                prev->rightTag = ThreadedTreeNode<T>::Type::thread;
                prev->right = node;
            }
            prev = node;
            inOrderRecursion(node->right, prev);
        }
    }

    void postOrderRecursion(ThreadedTreeNode<T>*& node, ThreadedTreeNode<T>*& prev) const
    {
        if (node == nullptr)
            return;
        else
        {
            postOrderRecursion(node->left, prev);
            postOrderRecursion(node->right, prev);
            //can't handle next for current parse
            if (!node->left)
            {
                node->leftTag = ThreadedTreeNode<T>::Type::thread;
                node->left = prev;
            }
            if (!prev->right)
            {
                prev->rightTag = ThreadedTreeNode<T>::Type::thread;
                prev->right = node;
            }
            prev = node;
        }
    }

    void levelOrder(BinaryTree<T>& binaryTree)
    {
        TreeNode<T>* cur = binaryTree.getRoot();
        head = new ThreadedTreeNode<T>;
        ThreadedTreeNode<T>* root = new ThreadedTreeNode<T>(cur->data);
        head->left = root;
        head->leftTag = ThreadedTreeNode<T>::Type::child;
        head->right = head;
        head->rightTag = ThreadedTreeNode<T>::Type::thread;
        ThreadedTreeNode<T>* prev = head;

        Queue<TreeNode<T>*> binaryQueue;
        Queue<ThreadedTreeNode<T>*> threadedQueue;
        binaryQueue.enqueue(cur);
        threadedQueue.enqueue(root);
        while (!binaryQueue.empty())
        {
            cur = binaryQueue.front();
            root = threadedQueue.front();
            if (cur->left)
            {
                binaryQueue.enqueue(cur->left);
                ThreadedTreeNode<T>* newNode = new ThreadedTreeNode<T>(cur->left->data);
                root->left = newNode;
                threadedQueue.enqueue(root->left);
            }

            if (cur->right)
            {
                binaryQueue.enqueue(cur->right);
                ThreadedTreeNode<T>* newNode = new ThreadedTreeNode<T>(cur->right->data);
                root->right = newNode;
                threadedQueue.enqueue(root->right);
            }

            if(!root->left)
            {
                root->left = prev;
                root->leftTag = ThreadedTreeNode<T>::Type::thread;
            }
            
            if (!prev->right)
            {
                prev->right = root;
                prev->rightTag = ThreadedTreeNode<T>::Type::thread;
            }
            prev = root;

            binaryQueue.dequeue();
            threadedQueue.dequeue();
        }

        //handle last node
        prev->right = head;
        prev->rightTag = ThreadedTreeNode<T>::Type::thread;
        head->right = prev;
    }
    
public:
    ThreadedBinaryTree(BinaryTree<T>& binaryTree, Mode mode)
    {
        assert(!binaryTree.empty());
        type = mode;

        createThreadedTree(binaryTree);
        ThreadedTreeNode<T>* prev = head;
        ThreadedTreeNode<T>* cur = prev->left;

        if (mode == Mode::preOrder)
            preOrderRecursion(cur, prev);
        else if (mode == Mode::inOrder)
            inOrderRecursion(cur, prev);
        /*else if (mode == Mode::postOrderRecursion)
            postOrderRecursion(binaryTree);*/

        //handle last node
        prev->right = head;
        prev->rightTag = ThreadedTreeNode<T>::Type::thread;
        head->right = prev;
    }

    ~ThreadedBinaryTree()
    {
        Queue<ThreadedTreeNode<T>*> queue;
        queue.enqueue(head);
        while (!queue.empty())
        {
            ThreadedTreeNode<T>* front = queue.front();
            if (front->leftTag == ThreadedTreeNode<T>::Type::child)
                queue.enqueue(front->left);
            if (front->rightTag == ThreadedTreeNode<T>::Type::child)
                queue.enqueue(front->right);
            queue.dequeue();
            delete front;
        }
    }

    Vector<T> traverse(void(*visit)(T, Vector<T>&)) const
    {
        assert(!empty());
        Vector<T> arr;
        //traverse from head to tail
        ThreadedTreeNode<T>* cur = head->left;

        if (type == Mode::preOrder)
        {
            while (cur != head)
            {
                visit(cur->data, arr);
                if (cur->leftTag == ThreadedTreeNode<T>::Type::child)
                    cur = cur->left;
                else
                    cur = cur->right;
            }
        }
        else if (type == Mode::inOrder)
        {
            while (cur != head)
            {
                //find left first node
                while (cur->leftTag == ThreadedTreeNode<T>::Type::child)
                    cur = cur->left;
                visit(cur->data, arr);
                //find left first node of right subtree
                while (cur->rightTag == ThreadedTreeNode<T>::Type::thread && cur->right != head)
                {
                    cur = cur->right;
                    visit(cur->data, arr);
                }
                cur = cur->right;
            }
        }
        //else if(type == Mode::postOrder)
        return arr;
    }

    bool empty() const
    {
        return head == nullptr;
    }
};


//Page 143
//3.
template<typename T>
void postOrderStack(TreeNode<T>* node, void(*visit)(T, Vector<T>&), Vector<T>& arr)
{
    TreeNode<T>* prev = node;
    TreeNode<T>* cur = prev;
    Stack<TreeNode<T>*> stack;
    while (cur || !stack.empty())
    {
        if (cur)
        {
            stack.push(cur);
            cur = cur->left;
        }
        else
        {
            cur = stack.top();
            if (!cur->right || prev == cur->right)
            {
                prev = cur;
                visit(cur->data, arr);
                stack.pop();
                cur = nullptr;
            }
            else
                cur = cur->right;
        }
    }
}

//4.
template<typename T>
Vector<T> reverseLevelOrder(TreeNode<T>* root)
{
    Vector<T> arr;
    Queue<TreeNode<T>*> queue;
    Stack<TreeNode<T>*> stack;
    if(root != nullptr)
        queue.enqueue(root);
    while(!queue.empty())
    {
        TreeNode<T>* front = queue.front();
        stack.push(front);
        if(front->left)
            queue.enqueue(front->left);
        if(front->right)
            queue.enqueue(front->right);
        queue.pop();
    }

    while(!stack.empty())
    {
        TreeNode<T>* top = stack.top();
        arr.push_back(top->data);
        stack.pop();
    }
    return arr;
}

//5
template<typename T>
uint32_t heightStack(TreeNode<T>* root)
{
    uint32_t height = 0;
    TreeNode<T>* prev = root;
    Stack<TreeNode<T>*> stack;
    while(root || !stack.empty())
    {
        if(root)
        {
            stack.push(root);
            root = root->left;
        }
        else
        {
            root = stack.top();
            if(root->right == nullptr || prev == root->right)
            {
                prev = root;
                if(stack.size() > height)
                    height = stack.size();
                stack.pop();
                root = nullptr;
            }
            else
                root = root->right;
        }
    }
    return height;
}

//6.
template<typename T>
BinaryTree<T> generateFromPreOrderAndInOrder(Vector<T>& preOrder, Vector<T>& inOrder)
{
    Vector<T> levelOrder;
    T root = preOrder[0];
    levelOrder.push_back(root);
    Vector<T> left;
    Vector<T> right;
    
    for(auto& it : inOrder)
    {
        if(it == root)
        {
            left = Vector<T>(inOrder.begin(), &it);
            right = Vector<T>(&it + 1, inOrder.end());
            break;
        }
    }
}


//7.
template<typename T>
bool completeBinaryTree(TreeNode<T>* root)
{
    Queue<TreeNode<T>*> queue;
    if(root)
        queue.enqueue(root);
    uint32_t k = 1;
    uint32_t count = 0;
    while(!queue.empty())
    {
        for(uint32_t i = 0; i < queue.size(); i++)
        {
            TreeNode<T>* front = queue.front();
            if(front->left)
                queue.enqueue(front->left);
            if(front->right)
                queue.enqueue(front->right);
            queue.dequeue();
        }
        if(queue.size() < 1 << k)
        {
            count++;
            if(count > 1)
                return false;
        }
    }
    return true;
}


//8
template<typename T>
uint32_t twoNode(TreeNode<T>* root)
{
    if(root == nullptr)
        return 0;
    uint32_t left = twoNode(root->left);
    uint32_t right = twoNode(root->right);
    if(root->left && root->right)
        return left + right + 1;
    else
        return left + right;
}

//9.
template<typename T>
void swapSubtree(TreeNode<T>* root)
{
    if(root == nullptr)
        return;
    TreeNode<T>* tmp = root->left;
    root->left = root->right;
    root->right = tmp;
    swapSubtree(root->left);
    swapSubtree(root->right);
}


//10.
template<typename T>
T preOrderValue(TreeNode<T>* root, uint32_t k)
{
    Stack<TreeNode<T>*> stack;
    uint32_t i = 0;
    while(root || !stack.empty())
    {
        if(root)
        {
            i++;
            if(i == k)
                return root->data;
            stack.push(root);
            root = root->left;
        }
        else
        {
            root = stack.top();
            stack.pop();
            root = root->right;
        }
    }
}

//11.
template<typename T>
void delTree(TreeNode<T>*& root)
{
    if(root == nullptr)
        return;
    delTree(root->left);
    delTree(root->right);
    delete root;
}

template<typename T>
void delSameValue(TreeNode<T>*& root, T value)
{
    if(root == nullptr)
        return;
    if(root->data == value)
    {
        delTree(root);
        root = nullptr;
        return;
    }
    delSameValue(root->left, value);
    delSameValue(root->right, value);
}

//12.
template<typename T>
void printAllAncestor(TreeNode<T>* root, T value, Vector<T>& arr)
{
    if (root == nullptr)
        return;
    printAllAncestor(root->left, value, arr);
    if (arr.empty())
        printAllAncestor(root->right, value, arr); 
    
    if (root->data == value || !arr.empty())
        arr.push_back(root->data);
}

//13.
template<typename T>
TreeNode<T>* publicAncestor(TreeNode<T>* root, TreeNode<T>* p, TreeNode<T>* q)
{
    
}

//14.
template<typename T>
uint32_t maxWidth(TreeNode<T>* root)
{
    Queue<TreeNode<T>*> queue;
    if(root)
        queue.enqueue(root);
    uint32_t width = 0;
    while(!queue.empty())
    {
        for(uint32_t i = 0; i < queue.size(); i++)
        {
            TreeNode<T>* front = queue.front();
            if(front->left)
                queue.enqueue(front->left);
            if(front->right)
                queue.enqueue(front->right);
            queue.dequeue();
        }
        if(width < queue.size())
            width = queue.size();
    }
    return width;
}

//15
template<typename T>
Vector<T> postOrderToPreOrder(Vector<T>& pre)
{
    if(pre.size() == 1)
        return pre;
    uint32_t mid = pre.size() >> 1;
    Vector<T> left(pre.begin() + 1, pre.begin() + mid + 1);
    Vector<T> right(pre.begin() + mid + 1, pre.end());
    Vector<T> newLeft = postOrderToPreOrder(left);
    Vector<T> newRight = postOrderToPreOrder(right);
    Vector<T> ret(newLeft, newRight);
    ret.push_back(pre[0]);
    return ret;
}

//16.
template<typename T>
void linkLeafNode(TreeNode<T>*& root, TreeNode<T>*& prev)
{
    if(root == nullptr)
        return;
    if(!root->left && !root->right)
    {
        if(prev)
            prev->right = root;
        prev = root;
        return;
    }
    linkLeafNode(root->left, prev);
    linkLeafNode(root->right, prev);
}

//17.
template<typename T>
bool similarBinaryTree(TreeNode<T>* p, TreeNode<T>* q)
{
    if(!p && !q)
        return true;
    return similarBinaryTree(p->left, q->left) && similarBinaryTree(p->right, q->right) && (p && q);
}

//18

//19

//20
template<typename T = char>
Vector<T> generateEqualExpression(TreeNode<T>* root)
{
    if(root == nullptr)
        return {};
    Vector<T> front = generateEqualExpression(root->left);
    Vector<T> tail = generateEqualExpression(root->right);
}