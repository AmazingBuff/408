#pragma once

#include"tree.hpp"
#include"hash.hpp"
#include"stack.hpp"

template<typename T, typename U>
int binarySearch(U& container, T target)
{
	int left = 0;
	int right = container.size() - 1;
	while (left <= right)
	{
		int middle = (left + right) >> 1;
		if (target > container[middle])
			left = middle + 1;
		else if (target < container[middle])
			right = middle - 1;
		else
			return middle;
	}
	return -1;
}


template<typename T>
struct CompareLess
{
    bool operator()(const T& a, const T& b) const
    {
        return a < b;
    }
};


template<typename Ty_Key, typename Ty_Val, typename Compare = CompareLess<Ty_Key>, typename Pred = Equal_To<Ty_Key>>
struct BinarySortTree
{
    using Hash = HashNode<Ty_Key, Ty_Val>;
    using SortTreeNode = TreeNode<Hash>;

    SortTreeNode* root = nullptr;

    BinarySortTree() = default;
    ~BinarySortTree()
    {
        if(root)
            destroy();
        root = nullptr;
    }

    BinarySortTree(const std::initializer_list<Hash>& list)
    {
        for(auto iterator = list.begin(); iterator != list.end(); iterator++)
            insert(*iterator);
    }

    void insert(const Ty_Key& key, const Ty_Val& value)
    {
        Hash hash(key, value);
        insert(hash, root);
    }

    void insert(const Hash& element)
    {
        insert(element, root);
    }

    void erase(const Ty_Key& key)
    {
        assert(root != nullptr);
        SortTreeNode* prev = root;
        SortTreeNode* cur = prev;
        if(root->data.key != key)
            find_both(key, root, cur, prev);

        if(cur != nullptr && prev != nullptr)
        {
            if(cur->left != nullptr && cur->right != nullptr)
            {
                SortTreeNode* cur_left_child = cur->left;
                SortTreeNode* cur_right_child = cur->right;
                SortTreeNode* rightest_child = cur_left_child;
                //find the rightest node of the left subtree of cur
                while(rightest_child->right != nullptr)
                    rightest_child = rightest_child->right;

                if(cur == prev->left)
                    prev->left = cur_left_child;
                else if(cur == prev->right)
                    prev->right = cur_left_child;
                else
                    root = cur_left_child;
                delete cur;
                cur = nullptr;
                rightest_child->right = cur_right_child;
            }
            else if(cur->left != nullptr || cur->right != nullptr)
            {
                SortTreeNode* cur_child = cur->left ? cur->left : cur->right;

                if(cur == prev->left)
                    prev->left = cur_child;
                else if(cur == prev->right)
                    prev->right = cur_child;
                else
                    root = cur_child;
                delete cur;
                cur = nullptr;
            }
            else
            {
                if(cur == prev->left)
                    prev->left = nullptr;
                else if(cur == prev->right)
                    prev->right = nullptr;
                else
                    root = nullptr;
                delete cur;
                cur = nullptr;
            }
        }
    }

    SortTreeNode* find(const Ty_Key& key) const
    {
        assert(root != nullptr);
        SortTreeNode* prev = root;
        SortTreeNode* cur = prev;
        find_both(key, root, cur, prev);

        if(cur != nullptr && prev != nullptr)
            return cur;
        else
            return nullptr;
    }

    void destroy()
    {
        Queue<SortTreeNode*> queue;
        if(root)
            queue.enqueue(root);
        while(!queue.empty())
        {
            SortTreeNode* front = queue.front();
            if(front->left)
                queue.enqueue(front->left);
            if(front->right)
                queue.enqueue(front->right);
            queue.dequeue();
            delete front;
        }
    }

private:
    void find_both(const Ty_Key& key, SortTreeNode* node, SortTreeNode*& cur, SortTreeNode*& prev)
    {
        if(node == nullptr)
        {
            cur == nullptr;
            prev == nullptr;
            return;
        }
        else if(Pred()(key, node->data.key))
        {
            cur = node;
            return;
        }
        else if(Compare()(key, node->data.key))
        {
            prev = node;
            find_both(key, node->left, cur, prev);
        }
        else
        {
            prev = node;
            find_both(key, node->right, cur, prev);
        }
    }

    void insert(const Hash& element, SortTreeNode*& node)
    {
        if(node == nullptr)
            node = new SortTreeNode(element);
        else if(Pred()(element.key, node->data.key))
            node->data.value = element.value;
        else if(Compare()(element.key, node->data.key))
            insert(element, node->left);
        else
            insert(element, node->right);
    }
};

template<typename T>
struct AVLTreeNode
{
    int balance_factor = 0;
    T data = 0;
    AVLTreeNode* left = nullptr;
    AVLTreeNode* right = nullptr;
    explicit AVLTreeNode(const T& element) : data(element) {}
    AVLTreeNode() = default;
};

template<typename Ty_Key, typename Ty_Val, typename Compare = CompareLess<Ty_Key>, typename Pred = Equal_To<Ty_Key>>
struct AVLTree
{
    using Hash = HashNode<Ty_Key, Ty_Val>;
    using BlanceSortTree = AVLTreeNode<Hash>;

    BlanceSortTree* root = nullptr;

    AVLTree() = default;
    ~AVLTree()
    {
        if(root)
            destroy();
        root = nullptr;
    }

    AVLTree(const std::initializer_list<Hash>& list)
    {
        for(auto iterator = list.begin(); iterator != list.end(); iterator++)
            insert(*iterator);
    }

    void insert(const Ty_Key& key, const Ty_Val& value)
    {
        Hash hash(key, value);
        insert(hash, root, root);
    }

    void insert(const Hash& element)
    {
        insert(element, root, root);
    }

    BlanceSortTree* find(const Ty_Key& key) const
    {
        assert(root != nullptr);
        BlanceSortTree* prev = root;
        BlanceSortTree* cur = prev;
        find_both(key, root, cur, prev);

        if(cur != nullptr && prev != nullptr)
            return cur;
        else
            return nullptr;
    }

    void destroy()
    {
        Queue<BlanceSortTree*> queue;
        if(root)
            queue.enqueue(root);
        while(!queue.empty())
        {
            BlanceSortTree* front = queue.front();
            if(front->left)
                queue.enqueue(front->left);
            if(front->right)
                queue.enqueue(front->right);
            queue.dequeue();
            delete front;
        }
    }

    void erase(const Ty_Key& key)
    {
        assert(root != nullptr);
        //with stack
        Stack<BlanceSortTree*> stack;
        BlanceSortTree* prev = root;
        BlanceSortTree* cur = prev;
        while(true)
        {
            if(cur != nullptr)
            {
                stack.push(cur);
                if(Pred()(key, cur->data.key))
                    break;
                else if(Compare()(key, cur->data.key))
                    cur = cur->left;
                else
                    cur = cur->right;
            }
            else
            {
                stack.destroy();
                return;
            }
        }

        //three conditions
        if(cur->left == nullptr && cur->right == nullptr)
        {
            int prev_depth = 1;
            int cur_depth = 0;
            BlanceSortTree* deleted = cur;
            while(!stack.empty())
            {
                cur = stack.top();
                stack.pop();
                if(stack.empty())
                {
                    delete root;
                    root = nullptr;
                    return;
                }
                else
                {
                    prev = stack.top();
                    if(prev_depth == cur_depth)
                    {
                        stack.destroy();
                        delete deleted;
                        return;
                    }
                    else if(cur->balance_factor == -2)
                    {
                        if(cur->right->balance_factor == -1)
                            right_to_right(cur, prev);
                        else if(cur->right->balance_factor == 1)
                            right_to_left(cur, prev);
                    }
                    else if(cur->balance_factor == 2)
                    {
                        if(cur->left->balance_factor == 1)
                            left_to_left(cur, prev);
                        else if(cur->left->balance_factor == -1)
                            left_to_right(cur, prev);
                    }
                    else if(cur == prev->left)
                    {
                        int prev_right = prev_depth - prev->balance_factor;
                        prev->balance_factor = cur_depth - prev_right;
                        prev_depth = std::max(prev_right, prev_depth) + 1;
                        cur_depth = std::max(cur_depth, prev_right) + 1;
                    }
                    else if(cur == prev->right)
                    {
                        int prev_left = prev_depth + prev->balance_factor;
                        prev->balance_factor = prev_left - cur_depth;
                        prev_depth = std::max(prev_left, prev_depth) + 1;
                        cur_depth = std::max(cur_depth, prev_left) + 1;
                    }
                }
            }
            delete deleted;
        }
        else if(cur->left != nullptr && cur->right != nullptr)
        {
            //track deleted node's right child
            BlanceSortTree* cur_left = cur->left;
            BlanceSortTree* cur_right = cur->right;
            BlanceSortTree* rightest = cur_left;
            delete cur;
            stack.pop();
            while(rightest->right != nullptr)
                rightest = rightest->right;
            rightest->right = cur_right;

            if(cur_right->balance_factor)

            if(stack.empty())
            prev = stack.top();

        }

    }

private:
    struct Info
    {
        int prev_depth;
        int cur_depth;
        bool operator==(const Info& other) const
        {
            return prev_depth == other.prev_depth && cur_depth == other.cur_depth;
        }
    };

    void find_both(const Ty_Key& key, BlanceSortTree* node, BlanceSortTree*& cur, BlanceSortTree*& prev)
    {
        if(node == nullptr)
        {
            cur == nullptr;
            prev == nullptr;
            return;
        }
        else if(Pred()(key, node->data.key))
        {
            cur = node;
            return;
        }
        else if(Compare()(key, node->data.key))
        {
            prev = node;
            find_both(key, node->left, cur, prev);
        }
        else
        {
            prev = node;
            find_both(key, node->right, cur, prev);
        }
    }

    BlanceSortTree* find_min_unbalance_subtree(BlanceSortTree* node) const
    {
        if(node == nullptr)
            return nullptr;
    }

    //four rotation
    void right_to_left(BlanceSortTree* node, BlanceSortTree*& prev)
    {
        BlanceSortTree* node_right = node->right;
        BlanceSortTree* node_right_left = node_right->left;
        BlanceSortTree* node_right_left_left = node_right_left->left;
        BlanceSortTree* node_right_left_right = node_right_left->right;
        node_right->left = node_right_left_right;
        node->right = node_right_left_left;
        node_right_left->left = node;
        node_right_left->right = node_right;
        if(node_right_left->balance_factor == 1)
        {
            node_right_left->balance_factor = 0;
            node_right->balance_factor = -1;
            node->balance_factor = 0;
        }
        else if(node_right_left->balance_factor == -1)
        {
            node_right_left->balance_factor = 0;
            node_right->balance_factor = 0;
            node->balance_factor = 1;
        }
        else
        {
            node_right->balance_factor = 0;
            node->balance_factor = 0;
        }
        if(node == prev)
            prev = node_right_left;
        else if(node == prev->left)
            prev->left = node_right_left;
        else
            prev->right = node_right_left;
    }

    void left_to_right(BlanceSortTree* node, BlanceSortTree*& prev)
    {
        BlanceSortTree* node_left = node->left;
        BlanceSortTree* node_left_right = node_left->right;
        BlanceSortTree* node_left_right_left = node_left_right->left;
        BlanceSortTree* node_left_right_right = node_left_right->right;
        node_left->right = node_left_right_left;
        node->left = node_left_right_right;
        node_left_right->left = node_left;
        node_left_right->right = node;
        if(node_left_right->balance_factor == 1)
        {
            node_left_right->balance_factor = 0;
            node_left->balance_factor = 0;
            node->balance_factor = -1;
        }
        else if(node_left_right->balance_factor == -1)
        {
            node_left_right->balance_factor = 0;
            node_left->balance_factor = 1;
            node->balance_factor = 0;
        }
        else
        {
            node_left->balance_factor = 0;
            node->balance_factor = 0;
        }
        if(node == prev)
            prev = node_left_right;
        else if(node == prev->left)
            prev->left = node_left_right;
        else
            prev->right = node_left_right;
    }

    void left_to_left(BlanceSortTree* node, BlanceSortTree*& prev)
    {

        BlanceSortTree* node_left = node->left;
        BlanceSortTree* node_left_right = node_left->right;
        node_left->left = node;
        node_left->balance_factor = 0;
        node->right = node_left_right;
        node->balance_factor = 0;
        if(node == prev)
            prev = node_left;
        else if(node == prev->left)
            prev->left = node_left;
        else
            prev->right = node_left;
    }

    void right_to_right(BlanceSortTree* node, BlanceSortTree*& prev)
    {
        BlanceSortTree* node_right = node->right;
        BlanceSortTree* node_right_left = node_right->left;
        node_right->left = node;
        node_right->balance_factor = 0;
        node->right = node_right_left;
        node->balance_factor = 0;
        if(prev == node)
            prev = node_right;
        else if(node == prev->left)
            prev->left = node_right;
        else
            prev->right = node_right;
    }

    Info insert(const Hash& element, BlanceSortTree*& node, BlanceSortTree*& prev)
    {
        if(node == nullptr)
        {
            node = new BlanceSortTree(element);
            if(node == prev)
                return {0, 0};
            else
            {
                if(node == prev->left)
                    prev->balance_factor += 1;
                else
                    prev->balance_factor -= 1;
                return {0, 1};
            }
        }
        else if(Pred()(element.key, node->data.key))
        {
            node->data.value = element.value;
            return {0, 0};
        }
        else if(Compare()(element.key, node->data.key))
        {
            Info node_left = insert(element, node->left, node);
            if(node_left.prev_depth == node_left.cur_depth)
                return {0, 0};
            else if(node->balance_factor == -2)
            {
                if(node->right->balance_factor == -1)
                    right_to_right(node, prev);
                else if(node->right->balance_factor == 1)
                    right_to_left(node, prev);
                return {node_left.cur_depth, node_left.cur_depth};
            }
            else if(node->balance_factor == 2)
            {
                if(node->left->balance_factor == 1)
                    left_to_left(node, prev);
                else if(node->left->balance_factor == -1)
                    left_to_right(node, prev);
                return {node_left.cur_depth, node_left.cur_depth};
            }
            else
            {
                int node_right = node_left.cur_depth - node->balance_factor;
                int node_cur_depth = std::max(node_left.cur_depth, node_right) + 1;
                int node_prev_depth = std::max(node_left.prev_depth, node_right) + 1;

                if(node == prev->left)
                {
                    int prev_right = node_prev_depth - prev->balance_factor;
                    prev->balance_factor = node_cur_depth - prev_right;
                }
                else if(node == prev->right)
                {
                    int prev_left = prev->balance_factor + node_prev_depth;
                    prev->balance_factor = prev_left - node_cur_depth;
                }

                return {node_prev_depth, node_cur_depth};
            }
        }
        else
        {
            Info node_right = insert(element, node->right, node);
            if(node_right.prev_depth == node_right.cur_depth)
                return {0, 0};
            else if(node->balance_factor == -2)
            {
                if(node->right->balance_factor == -1)
                    right_to_right(node, prev);
                else if(node->right->balance_factor == 1)
                    right_to_left(node, prev);
                return {node_right.cur_depth, node_right.cur_depth};
            }
            else if(node->balance_factor == 2)
            {
                if(node->left->balance_factor == 1)
                    left_to_left(node, prev);
                else if(node->left->balance_factor == -1)
                    left_to_right(node, prev);
                return {node_right.cur_depth, node_right.cur_depth};
            }
            else
            {
                int node_left = node_right.cur_depth + node->balance_factor;
                int node_cur_depth = std::max(node_right.cur_depth, node_left) + 1;
                int node_prev_depth = std::max(node_right.prev_depth, node_left) + 1;

                if(node == prev->left)
                {
                    int prev_right = node_prev_depth - prev->balance_factor;
                    prev->balance_factor = node_cur_depth - prev_right;
                }
                else if(node == prev->right)
                {
                    int prev_left = prev->balance_factor + node_prev_depth;
                    prev->balance_factor = prev_left - node_cur_depth;
                }

                return {node_prev_depth, node_cur_depth};
            }
        }
    }

    int refresh(BlanceSortTree* node)
    {
        if(node == nullptr)
            return 0;
        int left = refresh(node->left);
        int right = refresh(node->right);
        node->balance_factor = left - right;
        return std::max(left, right) + 1;
    }
};