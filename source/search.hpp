#pragma once

#include"tree.hpp"
#include"hash.hpp"

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
        if(root->data.key == key)
        {
            if(root->left != nullptr && root->right != nullptr)
            {
                SortTreeNode* left_child = root->left;
                SortTreeNode* right_child = root->right;
                SortTreeNode* rightest_child = left_child;
                //find the rightest node of the left subtree of cur
                while(rightest_child->right != nullptr)
                    rightest_child = rightest_child->right;
                delete root;
                root = left_child;
                rightest_child->right = right_child;
            }
            else if(root->left != nullptr || root->right != nullptr)
            {
                SortTreeNode* child = root->left ? root->left : root->right;
                delete root;
                root = child;
            }
            else
            {
                delete root;
                root = nullptr;
            }
        }
        else
        {
            SortTreeNode* prev = root;
            SortTreeNode* cur = prev;
            find_both(key, root, cur, prev);

            if(cur != nullptr && prev != nullptr)
            {
                bool left_children = cur == prev->left;

                if(cur->left != nullptr && cur->right != nullptr)
                {
                    SortTreeNode* cur_left_child = cur->left;
                    SortTreeNode* cur_right_child = cur->right;
                    SortTreeNode* rightest_child = cur_left_child;
                    //find the rightest node of the left subtree of cur
                    while(rightest_child->right != nullptr)
                        rightest_child = rightest_child->right;

                    delete cur;

                    if(left_children)
                        prev->left = cur_left_child;
                    else
                        prev->right = cur_left_child;
                    rightest_child->right = cur_right_child;
                }
                else if(cur->left != nullptr || cur->right != nullptr)
                {
                    SortTreeNode* cur_child = cur->left ? cur->left : cur->right;
                    delete cur;
                    if(left_children)
                        prev->left = cur_child;
                    else
                        prev->right = cur_child;
                }
                else
                {
                    delete cur;
                    if(left_children)
                        prev->left = nullptr;
                    else
                        prev->right = nullptr;
                }
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
struct AVLTreeNode : public TreeNode<T>
{
    int balance_factor = 0;
    explicit AVLTreeNode(const T& element)
    {
        this->data = element;
    }
    AVLTreeNode() = default;
};

template<typename Ty_Key, typename Ty_Val, typename Compare = CompareLess<Ty_Key>, typename Pred = Equal_To<Ty_Key>>
struct AVLTree
{
    using Hash = HashNode<Ty_Key, Ty_Val>;
    using BlanceSortTree = AVLTreeNode<Hash>;

    BlanceSortTree* root = nullptr;

    AVLTree() = default;

    AVLTree(const std::initializer_list<Hash>& list)
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
        if(root->data.key == key)
        {
            if(root->left != nullptr && root->right != nullptr)
            {
                BlanceSortTree* left_child = root->left;
                BlanceSortTree* right_child = root->right;
                BlanceSortTree* rightest_child = left_child;
                //find the rightest node of the left subtree of cur
                while(rightest_child->right != nullptr)
                    rightest_child = rightest_child->right;
                delete root;
                root = left_child;
                rightest_child->right = right_child;
            }
            else if(root->left != nullptr || root->right != nullptr)
            {
                BlanceSortTree* child = root->left ? root->left : root->right;
                delete root;
                root = child;
            }
            else
            {
                delete root;
                root = nullptr;
            }
        }
        else
        {
            BlanceSortTree* prev = root;
            BlanceSortTree* cur = prev;
            find_both(key, root, cur, prev);

            if(cur != nullptr && prev != nullptr)
            {
                bool left_children = cur == prev->left;

                if(cur->left != nullptr && cur->right != nullptr)
                {
                    BlanceSortTree* cur_left_child = cur->left;
                    BlanceSortTree* cur_right_child = cur->right;
                    BlanceSortTree* rightest_child = cur_left_child;
                    //find the rightest node of the left subtree of cur
                    while(rightest_child->right != nullptr)
                        rightest_child = rightest_child->right;

                    delete cur;

                    if(left_children)
                        prev->left = cur_left_child;
                    else
                        prev->right = cur_left_child;
                    rightest_child->right = cur_right_child;
                }
                else if(cur->left != nullptr || cur->right != nullptr)
                {
                    BlanceSortTree* cur_child = cur->left ? cur->left : cur->right;
                    delete cur;
                    if(left_children)
                        prev->left = cur_child;
                    else
                        prev->right = cur_child;
                }
                else
                {
                    delete cur;
                    if(left_children)
                        prev->left = nullptr;
                    else
                        prev->right = nullptr;
                }
            }
        }
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

private:
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

    void insert(const Hash& element, BlanceSortTree*& node)
    {
        if(node == nullptr)
            node = new BlanceSortTree(element);
        else if(Pred()(element.key, node->data.key))
            node->data.value = element.value;
        else if(Compare()(element.key, node->data.key))
            insert(element, node->left);
        else
            insert(element, node->right);
    }
};