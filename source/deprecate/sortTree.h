#pragma once

#include"tree.hpp"

//page 277
//6
template<typename T, typename Compare>
bool is_binary_sort_tree(TreeNode<T>* node)
{
    if (node == nullptr)
        return true;

    bool left = true, right = true;
    if (node->left != nullptr && !Compare()(node->left->data, node->data))
        left = false;

    if (node->right != nullptr && Compare()(node->right->data, node->data))
        right = false;

    return left && right && is_binary_sort_tree(node->left) && is_binary_sort_tree(node->right);
}

//7
template<typename T, typename Compare>
uint32_t level(TreeNode<T>* root, TreeNode<T>* node)
{
    TreeNode<T>* cur = root;
    uint32_t i = 1;
    while (cur != node)
    {
        if (Compare()(node->data, cur->data))
            cur = cur->left;
        else
            cur = cur->right;
        i++;
    }
    return i;
}

//8
template<typename T, typename Compare>
int is_avl_tree(TreeNode<T>* node)
{
    if (node == nullptr)
        return 0;

    int left = is_avl_tree(node->left);
    int right = is_avl_tree(node->right);
    if (abs(left - right) >= 2 || left == -1 || right == -1)
        return -1;
    else
        return std::max(left, right) + 1;
}

//9
template<typename T, typename Compare>
void get_min_and_max(TreeNode<T>* root, T* min, T* max)
{
    TreeNode<T>* min_node = root;
    TreeNode<T>* max_node = root;
    while (min_node->left != nullptr)
        min_node = min_node->left;
    while (max_node->right != nullptr)
        max_node = max_node->right;

    *min = min_node->data;
    *max = max_node->data;
}

//10
template<typename T, typename Compare>
void output(TreeNode<T>* node, T key, void(*print)(T))
{
    if (node == nullptr)
        return;

    output(node->right, key, print);
    if (Compare()(key, node->data))
        print(node->data);
    output(node->left, key, print);
}


//11
template<typename T>
struct TTreeNode: TreeNode<T>
{
    uint32_t count = 0;
    TTreeNode() : TreeNode() {}
};

template<typename T, typename Compare>
TTreeNode<T>* get_k_min(TTreeNode<T>* node, uint32_t* k)
{
    if (*k > node->count + 1)
    {
        *k = *k - node->count - 1;
        return nullptr;
    }
    else if (*k == node->count + 1)
        return node;
    else
    {
        TTreeNode<T>* left = get_k_min(node->left, k);
        if (left != nullptr)
            return left;
        TTreeNode<T>* right = get_k_min(node->right, k);
        if (right != nullptr)
            return right;
        return nullptr;
    }
}