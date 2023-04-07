#include"../textbook/BasicVariable.h"
#include"queue.h"
#include"vector.h"
#include"list.h"

template<typename T>
struct TreeNode
{
    T data = 0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode(T& element) : data(element) {}
    TreeNode() {}
};

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
            //store left pointer to avoid revising
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
void generateFromPreOrderAndInOrder(Vector<T>& preOrder, Vector<T>& inOrder, TreeNode<T>*& root)
{
    if (preOrder.size() == 1 || inOrder.size() == 1)
    {
        root = new TreeNode<T>(preOrder[0]);
        return;
    }
    else if (preOrder.empty() || inOrder.empty())
        return;

    root = new TreeNode<T>(preOrder[0]);
    uint32_t inMidIndex = inOrder.find(preOrder[0]);
    Vector<T> inLeft(inOrder.begin(), inOrder.begin() + inMidIndex);
    Vector<T> inRight(inOrder.begin() + inMidIndex + 1, inOrder.end());

    uint32_t preMidIndex = inLeft.size();
    Vector<T> preLeft(preOrder.begin() + 1, preOrder.begin() + preMidIndex + 1);
    Vector<T> preRight(preOrder.begin() + preMidIndex + 1, preOrder.end());
    
    generateFromPreOrderAndInOrder(preLeft, inLeft, root->left);
    generateFromPreOrderAndInOrder(preRight, inRight, root->right);
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
    if(root == nullptr)
        return nullptr;
    else if(root == p || root == q)
        return root;
    TreeNode<T>* left = publicAncestor(root->left);
    TreeNode<T>* right = publicAncestor(root->right);
    if(left == p || left == q)
        return left;
    else if(right == p || right == q)
        return right;
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
template<typename T>
T depthWPL(TreeNode<T>* root)
{
    Queue<TreeNode<T>*> queue;
    if (root)
        queue.enqueue(root);
    T ret = 0;
    uint32_t depth = 0;
    while (!queue.empty())
    { 
        depth++;
        uint32_t length = queue.size();
        for (uint32_t i = 0; i < length; i++)
        {
            root = queue.front();
            if (root->left == nullptr && root->right == nullptr)
                ret += depth * root->data;
            else
            {
                if (root->left)
                    queue.enqueue(root->left);
                if (root->right)
                    queue.enqueue(root->right);
            }
            queue.dequeue();
        }   
    }
    return ret;
}

//20
template<typename T = char>
void generateEqualExpression(TreeNode<T>* root, TreeNode<T>* prev, Vector<T>& ret)
{
    if (root->left == nullptr && root->right == nullptr && root == prev->left)
    {
        ret.push_back('(');
        ret.push_back(root->data);
        return;
    }
    else if (root->left == nullptr && root->right == nullptr && root == prev->right)
    {
        ret.push_back(root->data);
        ret.push_back(')');
        return;
    }
    else if (root->left == nullptr)
    {
        ret.push_back('(');
        ret.push_back(root->data);
        generateEqualExpression(root->right, root, ret);
        ret.push_back(')');
        return;
    }
    generateEqualExpression(root->left, root, ret);
    ret.push_back(root->data);
    generateEqualExpression(root->right, root, ret);
}

//21.



template<typename T>
struct CSTreeNode
{
public:
    T data;
    CSTreeNode* firstChild = nullptr;
    CSTreeNode* nextSibling = nullptr;
    CSTreeNode(T& element) : data(element) {}
    CSTreeNode() : data(0) {}

    CSTreeNode(const TreeNode<T>* root)
    {
        assert(root);
        CSTreeNode* newRoot = transformation(root);
        data = newRoot->data;
        firstChild = newRoot->firstChild;
        nextSibling = newRoot->nextSibling;
    }
private:
    CSTreeNode transformation(const TreeNode<T>* root)
    {
        if(root == nullptr)
            return nullptr;
        CSTreeNode* left = transformation(root->left);
        CSTreeNode* right = transformation(root->right);
        CSTreeNode* node = new CSTreeNode(root->data);
        node->firstChild = left;
        node->nextSibling = right;
        return node;
    }
};

template<typename T>
struct Tree
{
    T data;
    Tree** children = nullptr;
    uint32_t degree = 0;

    Tree(T& element, uint32_t& degree) : data(element), degree(degree) {}
    Tree() : data(0) {}
};


template<typename T>
Tree<T>* generateTree(Vector<T>& levelOrder, Vector<uint32_t>& degree)
{
    assert(levelOrder.size() == degree.size());
    Tree<T>* root = new Tree<T>(levelOrder[0], degree[0]);
    Queue<Tree<T>*> queue;
    queue.enqueue(root);
    uint32_t count = 1;
    while (!queue.empty())
    {
        Tree<T>* cur = queue.front();
        for (uint32_t i = 0; i < cur->children.capacity(); i++)
        {
            Tree<T>* node = new Tree<T>(levelOrder[count], degree[count]);
            queue.enqueue(node);
            cur->children.push_back(node);
            count++;
        }
        queue.dequeue();
    }
    return root;
}


template<typename T>
CSTreeNode<T>* transformCS(Tree<T>* root)
{
    assert(root);
    CSTreeNode<T>* ret = new CSTreeNode<T>(root->data);
    Queue<Tree<T>*> queue;
    Queue<CSTreeNode<T>*> csQueue;
    queue.enqueue(root);
    csQueue.enqueue(ret);
    while (!queue.empty())
    {
        root = queue.front();
        CSTreeNode<T>* cur = csQueue.front();
        CSTreeNode<T>* prev = nullptr;
        for (uint32_t i = 0; i < root->children.capacity(); i++)
        {
            CSTreeNode<T>* node = new CSTreeNode<T>(root->children[i]->data);
            if (i == 0)
            {
                prev = node;
                cur->firstChild = prev;
            }
            else
            {
                prev->nextSibling = node;
                prev = node;
            }
            queue.enqueue(root->children[i]);
            csQueue.enqueue(node);
        }
        queue.dequeue();
        csQueue.dequeue();
    }
    return ret;
}

//page. 170
//4.
template<typename T>
uint32_t leafNodeInForest(CSTreeNode<T>* root)
{
    if (root == nullptr)
        return 0;
    return leafNodeInForest(root->firstChild) + leafNodeInForest(root->nextSibling) + (root->firstChild ? 0 : 1);
}

//5.
template<typename T>
uint32_t depthInCS(CSTreeNode<T>* root)
{
    if (root == nullptr)
        return 0;
    uint32_t left = depthInCS(root->firstChild);
    uint32_t right = depthInCS(root->nextSibling);
    return left >= right ? left + 1 : right;
}

//6.
template<typename T>
CSTreeNode<T>* generateFromLevelOrder(Vector<T>& levelOrder, Vector<uint32_t>& degrees)
{
    assert(levelOrder.size() == degrees.size());
    CSTreeNode<T>* root = new CSTreeNode<T>(levelOrder[0]);
    Queue<CSTreeNode<T>*> queue;
    queue.enqueue(root);
    uint32_t count = 1;
    uint32_t pos = 0;
    while (!queue.empty())
    {
        CSTreeNode<T>* cur = queue.front();
        CSTreeNode<T>* prev = nullptr;
        uint32_t degree = degrees[pos];
        for (uint32_t i = 0; i < degree; i++)
        {
            CSTreeNode<T>* node = new CSTreeNode<T>(levelOrder[count]);
            if (i == 0)
            {
                prev = node;
                cur->firstChild = prev;
            }
            else
            {
                prev->nextSibling = node;
                prev = node;
            }
            queue.enqueue(node);
            count++;
        }
        queue.dequeue();
        pos++;
    }
    return root;
}


//Huffman tree
template<typename T>
struct HuffmanNode
{
    T data = 0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    uint32_t weight = 0;
    HuffmanNode(const T& element, const uint32_t& weight) : data(element), weight(weight) {}
    HuffmanNode() {}
};

template<typename T>
struct HuffmanTree
{
private:
    HuffmanNode<T>* root = nullptr;

public:
    HuffmanTree(HuffmanNode<T>* root) : root(root) {}

    Vector<uint32_t> HuffmanCodeTrans() const
    {
        Queue<HuffmanNode<T>*> queue;
        Queue<uint32_t> codeQueue;
        if (root)
        {
            queue.enqueue(root);
            codeQueue.enqueue(0xf0000000);
        }
        Vector<uint32_t> ret;
        while (!queue.empty())
        {
            uint32_t length = queue.size();
            for (uint32_t i = 0; i < length; i++)
            {
                HuffmanNode<T>* cur = queue.front();
                uint32_t code = codeQueue.front();
                if (cur->left == nullptr && cur->right == nullptr)
                    ret.push_back(code);
                else
                {
                    if (cur->right)
                    {
                        queue.enqueue(cur->right);
                        codeQueue.enqueue(code << 1 | 1);
                    } 
                    if (cur->left)
                    {
                        queue.enqueue(cur->left);
                        codeQueue.enqueue(code << 1 | 0);
                    }  
                }
                queue.dequeue();
                codeQueue.dequeue();
            }   
        }
        return ret;
    }

    uint32_t WPL() const
    {
        Queue<HuffmanNode<T>*> queue;
        if (root)
            queue.enqueue(root);
        uint32_t ret = 0;
        uint32_t depth = 0;
        while (!queue.empty())
        { 
            depth++;
            uint32_t length = queue.size();
            for (uint32_t i = 0; i < length; i++)
            {
                HuffmanNode<T>* cur = queue.front();
                if (cur->left == nullptr && cur->right == nullptr)
                    ret += depth * cur->weight;
                else
                {
                    if (cur->left)
                        queue.enqueue(cur->left);
                    if (cur->right)
                        queue.enqueue(cur->right);
                }
                queue.dequeue();
            }   
        }
        return ret;
    }
};


template<typename T>
HuffmanNode<T>* createHuffmanTree(LinkedList<T>& values, LinkedList<uint32_t>& weights)
{
    assert(values.size() == weights.size());
    auto compare_less = [](uint32_t a, uint32_t b)
    {
        if(a < b)
            return true;
        else
            return false;
    };

    HuffmanNode<T>* cur = nullptr;
    LinkedList<HuffmanNode<T>*> prev;
    LinkedList<uint32_t> preWeight;
    while(!weights.empty())
    {
        uint32_t minIndex = weights.findMin(compare_less);
        uint32_t min2Index = weights.findMin(compare_less, minIndex);
        uint32_t newWeight = weights[minIndex] + weights[min2Index];

        weights.pop(minIndex);
        weights.pop(min2Index);
        weights.push_back(newWeight);
        values.pop(minIndex);
        values.pop(min2Index);
        values.push_back(0);

        HuffmanNode<T>* leftNode = nullptr;
        HuffmanNode<T>* rightNode = nullptr;

        uint32_t weIndex = preWeight.find(weights[minIndex]);
        if(weIndex != preWeight.end())
        {
            leftNode = prev[weIndex];
            preWeight.pop(weIndex);
            prev.pop(weIndex);
        }
        else
            leftNode = new HuffmanNode<T>(values[minIndex], weights[minIndex]);
        
        weIndex = preWeight.find(weights[min2Index]);
        if(weIndex != preWeight.end())
        {
            rightNode = prev[weIndex];
            preWeight.pop(weIndex);
            prev.pop(weIndex);
        }
        else
            rightNode = new HuffmanNode<T>(values[min2Index], weights[min2Index]);
        
        cur = new HuffmanNode<T>(0, newWeight);
        cur->left = leftNode;
        cur->right = rightNode;

        prev.push_back(cur);
        preWeight.push_back(newWeight);
    }
}