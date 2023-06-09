#pragma once

#include"tree.hpp"
#include"hash.hpp"
#include"stack.hpp"

template<typename T>
struct CompareLess
{
    bool operator()(const T& a, const T& b) const
    {
        return a < b;
    }
};


template<typename T, typename Compare = CompareLess<T>>
int binarySearch(const Vector<T>& container, const T& target)
{
	int left = 0;
	int right = container.size() - 1;
	while (left <= right)
	{
		int middle = (left + right) >> 1;
		if (Compare()(container[middle], target))
			left = middle + 1;
		else if (Compare()(target, container[middle]))
			right = middle - 1;
		else
			return middle;
	}
	return -1;
}


template<typename T, typename Compare = CompareLess<T>>
int binarySearch(const Vector<T>& container, const T& target, const int& left, const int& right)
{
	if (left > right)
		return -1;
	int middle = (left + right) >> 1;
	if (Compare()(container[middle], target))
		return binarySearch(container, target, middle + 1, right);
	else if (Compare()(target, container[middle]))
		return binarySearch(container, target, left, middle - 1);
	else
		return middle;
}



template<typename Ty_Key, typename Ty_Val, typename Compare = CompareLess<Ty_Key>, typename Pred = Equal_To<Ty_Key>>
struct BinarySortTree
{
	using Hash = HashNode<Ty_Key, Ty_Val>;
	using SortTreeNode = TreeNode<Hash>;

	SortTreeNode* root = nullptr;

	BinarySortTree() = default;
	~BinarySortTree()
	{
		if (root)
			destroy();
		root = nullptr;
	}

	BinarySortTree(const std::initializer_list<Hash>& list)
	{
		for (auto iterator = list.begin(); iterator != list.end(); iterator++)
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
		if (root->data.key != key)
			find_both(key, root, cur, prev);

		if (cur != nullptr && prev != nullptr)
		{
			if (cur->left != nullptr && cur->right != nullptr)
			{
				SortTreeNode* cur_left_child = cur->left;
				SortTreeNode* cur_right_child = cur->right;
				SortTreeNode* rightest_child = cur_left_child;
				//find the rightest node of the left subtree of cur
				while (rightest_child->right != nullptr)
					rightest_child = rightest_child->right;

				if (cur == prev->left)
					prev->left = cur_left_child;
				else if (cur == prev->right)
					prev->right = cur_left_child;
				else
					root = cur_left_child;
				delete cur;
				cur = nullptr;
				rightest_child->right = cur_right_child;
			}
			else if (cur->left != nullptr || cur->right != nullptr)
			{
				SortTreeNode* cur_child = cur->left ? cur->left : cur->right;

				if (cur == prev->left)
					prev->left = cur_child;
				else if (cur == prev->right)
					prev->right = cur_child;
				else
					root = cur_child;
				delete cur;
				cur = nullptr;
			}
			else
			{
				if (cur == prev->left)
					prev->left = nullptr;
				else if (cur == prev->right)
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

		if (cur != nullptr && prev != nullptr)
			return cur;
		else
			return nullptr;
	}

	void destroy()
	{
		Queue<SortTreeNode*> queue;
		if (root)
			queue.enqueue(root);
		while (!queue.empty())
		{
			SortTreeNode* front = queue.front();
			if (front->left)
				queue.enqueue(front->left);
			if (front->right)
				queue.enqueue(front->right);
			queue.dequeue();
			delete front;
		}
	}

private:
	void find_both(const Ty_Key& key, SortTreeNode* node, SortTreeNode*& cur, SortTreeNode*& prev)
	{
		if (node == nullptr)
		{
			cur = nullptr;
			prev = nullptr;
			return;
		}
		else if (Pred()(key, node->data.key))
		{
			cur = node;
			return;
		}
		else if (Compare()(key, node->data.key))
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
		if (node == nullptr)
			node = new SortTreeNode(element);
		else if (Pred()(element.key, node->data.key))
			node->data.value = element.value;
		else if (Compare()(element.key, node->data.key))
			insert(element, node->left);
		else
			insert(element, node->right);
	}
};

template<typename T>
struct AVLTreeNodeNoParent
{
	int balance_factor = 0;
	T data = T();
	AVLTreeNodeNoParent* left = nullptr;
	AVLTreeNodeNoParent* right = nullptr;
	explicit AVLTreeNodeNoParent(const T& element) : data(element) {}
	AVLTreeNodeNoParent() = default;
};

template<typename Ty_Key, typename Ty_Val, typename Compare = CompareLess<Ty_Key>, typename Pred = Equal_To<Ty_Key>>
struct AVLTreeNoParent
{
	using Hash = HashNode<Ty_Key, Ty_Val>;
	using BlanceSortTree = AVLTreeNodeNoParent<Hash>;
private:
	BlanceSortTree* root = nullptr;
	BlanceSortTree* head = nullptr;
public:
	//in order iterator
	struct Iterator
	{
		using pointer = HashNode<Ty_Key, Ty_Val>*;
		using AVL_Tree = AVLTreeNoParent<Ty_Key, Ty_Val, Compare, Pred>*;
	public:
		Iterator(pointer ptr, AVL_Tree tree) : ptr(ptr), tree(tree) { }
		Iterator() : ptr(nullptr), tree(nullptr) { }
		Iterator(const Iterator& other)
		{
			ptr = other.ptr;
			tree = other.tree;
		}

		Iterator& operator=(const Iterator& other)
		{
			ptr = other.ptr;
			tree = other.tree;
			return *this;
		}

		HashNode<Ty_Key, Ty_Val>& operator*() const
		{
			return *ptr;
		}

		HashNode<Ty_Key, Ty_Val>* operator->() const
		{
			return ptr;
		}

		Iterator& operator++()
		{
			assert(*this != tree->end());
			BlanceSortTree* prev = tree->root;
			BlanceSortTree* cur = prev;

			Stack<BlanceSortTree*> stack;
			while (true)
			{
				stack.push(cur);
				if (Pred()(ptr->key, cur->data.key))
					break;
				else if (Compare()(ptr->key, cur->data.key))
					cur = cur->left;
				else
					cur = cur->right;
			}

			cur = stack.top();
			stack.pop();

			//find the direct next of cur
			if (cur->right != nullptr)
			{
				BlanceSortTree* direct_next = cur->right;
				while (direct_next->left != nullptr)
					direct_next = direct_next->left;

				ptr = &direct_next->data;
			}
			else if (!stack.empty())
			{
				prev = stack.top();
				while (prev->left != cur)
				{
					stack.pop();
					cur = prev;
					if (stack.empty())
						break;
					prev = stack.top();
				}
				if (stack.empty())
					ptr = tree->end().ptr;
				else
					ptr = &prev->data;
			}
			else
				ptr = tree->end().ptr;

			stack.destroy();
			return *this;
		}

		explicit operator bool() const
		{
			return ptr != nullptr && tree != nullptr;
		}

		Iterator operator++(int)&
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator!=(const Iterator& other) const
		{
			return ptr != other.ptr || tree != other.tree;
		}

		bool operator==(const Iterator& other) const
		{
			return ptr == other.ptr && tree == other.tree;
		}
	private:
		pointer ptr;
		AVL_Tree tree;
	};

	AVLTreeNoParent()
	{
		head = new BlanceSortTree;
	}
	~AVLTreeNoParent()
	{
		if (root)
			destroy();
		root = nullptr;
		delete head;
		head = nullptr;
	}

	AVLTreeNoParent(const std::initializer_list<Hash>& list)
	{
		head = new BlanceSortTree;
		for (auto iterator = list.begin(); iterator != list.end(); iterator++)
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

	Iterator find(const Ty_Key& key) const
	{
		assert(root != nullptr);
		BlanceSortTree* prev = root;
		BlanceSortTree* cur = prev;
		find_both(key, root, cur, prev);

		if (cur != nullptr)
			return Iterator(&cur->data, this);
		else
			return end();
	}

	Iterator begin()
	{
		assert(root != nullptr);
		BlanceSortTree* cur = root;
		while (cur->left != nullptr)
			cur = cur->left;
		return Iterator(&cur->data, this);
	}

	Iterator begin() const
	{
		assert(root != nullptr);
		BlanceSortTree* cur = root;
		while (cur->left != nullptr)
			cur = cur->left;
		return Iterator(&cur->data, this);
	}

	Iterator end()
	{
		assert(root != nullptr);
		return Iterator(&head->data, this);
	}

	Iterator end() const
	{
		assert(root != nullptr);
		return Iterator(&head->data, this);
	}

	void destroy()
	{
		Queue<BlanceSortTree*> queue;
		if (root)
			queue.enqueue(root);
		while (!queue.empty())
		{
			BlanceSortTree* front = queue.front();
			if (front->left)
				queue.enqueue(front->left);
			if (front->right)
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
		BlanceSortTree* cur = root;
		while (true)
		{
			if (cur != nullptr)
			{
				stack.push(cur);
				if (Pred()(key, cur->data.key))
					break;
				else if (Compare()(key, cur->data.key))
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

		erase(cur, stack);
	}

	int height(BlanceSortTree* node)
	{
		if (node == nullptr)
			return 0;
		int left = height(node->left);
		int right = height(node->right);
		node->balance_factor = left - right;
		return std::max(left, right) + 1;
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

	void erase(BlanceSortTree* cur, Stack<BlanceSortTree*>& stack)
	{
		BlanceSortTree* prev = nullptr;
		//three conditions
		if (cur->left == nullptr && cur->right == nullptr)
		{
			BlanceSortTree* deleted = cur;
			stack.pop();
			BlanceSortTree* deleted_parent = stack.empty() ? nullptr : stack.top();
			stack.push(cur);

			int prev_depth = 1;
			int cur_depth = 0;
			while (true)
			{
				cur = stack.top();
				stack.pop();
				if (!stack.empty())
				{
					prev = stack.top();
					if (cur == prev->left)
					{
						int prev_right = prev_depth - prev->balance_factor;
						prev->balance_factor = cur_depth - prev_right;
						prev_depth = std::max(prev_right, prev_depth) + 1;
						cur_depth = std::max(cur_depth, prev_right) + 1;
					}
					else
					{
						int prev_left = prev_depth + prev->balance_factor;
						prev->balance_factor = prev_left - cur_depth;
						prev_depth = std::max(prev_left, prev_depth) + 1;
						cur_depth = std::max(cur_depth, prev_left) + 1;
					}

					if (cur->balance_factor == -2)
					{
						if (cur->right->balance_factor == -1)
							right_to_right(cur, prev);
						else if (cur->right->balance_factor == 1)
							right_to_left(cur, prev);
					}
					else if (cur->balance_factor == 2)
					{
						if (cur->left->balance_factor == 1)
							left_to_left(cur, prev);
						else if (cur->left->balance_factor == -1)
							left_to_right(cur, prev);
					}

					if (prev_depth == cur_depth)
					{
						stack.destroy();
						if (deleted == deleted_parent->left)
							deleted_parent->left = nullptr;
						else
							deleted_parent->right = nullptr;
						delete deleted;
						return;
					}
				}
				else
				{
					if (deleted_parent == nullptr)
						root = nullptr;
					else if (deleted == deleted_parent->left)
						deleted_parent->left = nullptr;
					else
						deleted_parent->right = nullptr;
					delete deleted;
					return;
				}
			}
		}
		else if (cur->left != nullptr && cur->right != nullptr)
		{
			//use the second way to delete cur node
			//exchange deleted node with its direct prev or direct next in order
			if (cur->balance_factor == 0 || cur->balance_factor == 1)
			{
				BlanceSortTree* cur_left_rightest = cur->left;
				stack.push(cur_left_rightest);
				while (cur_left_rightest->right != nullptr)
				{
					stack.push(cur_left_rightest->right);
					cur_left_rightest = cur_left_rightest->right;
				}

				//exchange data and invoke erase again
				Hash tmp = cur->data;
				cur->data = cur_left_rightest->data;
				cur_left_rightest->data = tmp;
				erase(cur_left_rightest, stack);
			}
			else
			{
				BlanceSortTree* cur_right_leftest = cur->right;
				stack.push(cur_right_leftest);
				while (cur_right_leftest->left != nullptr)
				{
					stack.push(cur_right_leftest->left);
					cur_right_leftest = cur_right_leftest->left;
				}

				//exchange data and invoke erase again
				Hash tmp = cur->data;
				cur->data = cur_right_leftest->data;
				cur_right_leftest->data = tmp;
				erase(cur_right_leftest, stack);
			}
		}
		else if (cur->left != nullptr || cur->right != nullptr)
		{
			BlanceSortTree* deleted = cur;
			stack.pop();
			BlanceSortTree* deleted_parent = stack.empty() ? nullptr : stack.top();
			stack.push(cur);

			int prev_depth = 0;
			int cur_depth = 0;
			if (cur->left != nullptr)
			{
				int prev_left = cur->balance_factor;
				prev_depth = prev_left + 1;
				cur_depth = prev_left;
			}
			else
			{
				int prev_right = -cur->balance_factor;
				prev_depth = prev_right + 1;
				cur_depth = prev_right;
			}
			while (true)
			{
				cur = stack.top();
				stack.pop();
				if (!stack.empty())
				{
					prev = stack.top();
					if (cur == prev->left)
					{
						int prev_right = prev_depth - prev->balance_factor;
						prev->balance_factor = cur_depth - prev_right;
						prev_depth = std::max(prev_right, prev_depth) + 1;
						cur_depth = std::max(cur_depth, prev_right) + 1;
					}
					else
					{
						int prev_left = prev_depth + prev->balance_factor;
						prev->balance_factor = prev_left - cur_depth;
						prev_depth = std::max(prev_left, prev_depth) + 1;
						cur_depth = std::max(cur_depth, prev_left) + 1;
					}

					if (cur->balance_factor == -2)
					{
						if (cur->right->balance_factor == -1)
							right_to_right(cur, prev);
						else if (cur->right->balance_factor == 1)
							right_to_left(cur, prev);
					}
					else if (cur->balance_factor == 2)
					{
						if (cur->left->balance_factor == 1)
							left_to_left(cur, prev);
						else if (cur->left->balance_factor == -1)
							left_to_right(cur, prev);
					}

					if (prev_depth == cur_depth)
					{
						stack.destroy();
						if (deleted == deleted_parent->left)
						{
							if (deleted->left != nullptr)
								deleted_parent->left = deleted->left;
							else
								deleted_parent->left = deleted->right;
						}
						else
						{
							if (deleted->left != nullptr)
								deleted_parent->right = deleted->left;
							else
								deleted_parent->right = deleted->right;
						}
						delete deleted;
						return;
					}
				}
				else
				{
					if (deleted_parent == nullptr)
					{
						if (deleted->left != nullptr)
							root = deleted->left;
						else
							root = deleted->right;
					}
					else if (deleted == deleted_parent->left)
					{
						if (deleted->left != nullptr)
							deleted_parent->left = deleted->left;
						else
							deleted_parent->left = deleted->right;
					}
					else
					{
						if (deleted->left != nullptr)
							deleted_parent->right = deleted->left;
						else
							deleted_parent->right = deleted->right;
					}
					delete deleted;
					return;
				}
			}
		}
	}

	void find_both(const Ty_Key& key, BlanceSortTree* node, BlanceSortTree*& cur, BlanceSortTree*& prev) const
	{
		if (node == nullptr)
		{
			cur = nullptr;
			prev = nullptr;
			return;
		}
		else if (Pred()(key, node->data.key))
		{
			cur = node;
			return;
		}
		else if (Compare()(key, node->data.key))
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
		if (node_right_left->balance_factor == 1)
		{
			node_right_left->balance_factor = 0;
			node_right->balance_factor = -1;
			node->balance_factor = 0;
		}
		else if (node_right_left->balance_factor == -1)
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
		if (node == prev)
			prev = node_right_left;
		else if (node == prev->left)
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
		if (node_left_right->balance_factor == 1)
		{
			node_left_right->balance_factor = 0;
			node_left->balance_factor = 0;
			node->balance_factor = -1;
		}
		else if (node_left_right->balance_factor == -1)
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
		if (node == prev)
			prev = node_left_right;
		else if (node == prev->left)
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
		if (node == prev)
			prev = node_left;
		else if (node == prev->left)
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
		if (prev == node)
			prev = node_right;
		else if (node == prev->left)
			prev->left = node_right;
		else
			prev->right = node_right;
	}

	Info insert(const Hash& element, BlanceSortTree*& node, BlanceSortTree*& prev)
	{
		if (node == nullptr)
		{
			node = new BlanceSortTree(element);
			if (node == prev)
				return { 0, 0 };
			else
			{
				if (node == prev->left)
					prev->balance_factor += 1;
				else
					prev->balance_factor -= 1;
				return { 0, 1 };
			}
		}
		else if (Pred()(element.key, node->data.key))
		{
			node->data.value = element.value;
			return { 0, 0 };
		}
		else if (Compare()(element.key, node->data.key))
		{
			Info node_left = insert(element, node->left, node);
			if (node_left.prev_depth == node_left.cur_depth)
				return { 0, 0 };
			else if (node->balance_factor == -2)
			{
				if (node->right->balance_factor == -1)
					right_to_right(node, prev);
				else if (node->right->balance_factor == 1)
					right_to_left(node, prev);
				return { node_left.cur_depth, node_left.cur_depth };
			}
			else if (node->balance_factor == 2)
			{
				if (node->left->balance_factor == 1)
					left_to_left(node, prev);
				else if (node->left->balance_factor == -1)
					left_to_right(node, prev);
				return { node_left.cur_depth, node_left.cur_depth };
			}
			else
			{
				int node_right = node_left.cur_depth - node->balance_factor;
				int node_cur_depth = std::max(node_left.cur_depth, node_right) + 1;
				int node_prev_depth = std::max(node_left.prev_depth, node_right) + 1;

				if (node == prev->left)
				{
					int prev_right = node_prev_depth - prev->balance_factor;
					prev->balance_factor = node_cur_depth - prev_right;
				}
				else if (node == prev->right)
				{
					int prev_left = prev->balance_factor + node_prev_depth;
					prev->balance_factor = prev_left - node_cur_depth;
				}

				return { node_prev_depth, node_cur_depth };
			}
		}
		else
		{
			Info node_right = insert(element, node->right, node);
			if (node_right.prev_depth == node_right.cur_depth)
				return { 0, 0 };
			else if (node->balance_factor == -2)
			{
				if (node->right->balance_factor == -1)
					right_to_right(node, prev);
				else if (node->right->balance_factor == 1)
					right_to_left(node, prev);
				return { node_right.cur_depth, node_right.cur_depth };
			}
			else if (node->balance_factor == 2)
			{
				if (node->left->balance_factor == 1)
					left_to_left(node, prev);
				else if (node->left->balance_factor == -1)
					left_to_right(node, prev);
				return { node_right.cur_depth, node_right.cur_depth };
			}
			else
			{
				int node_left = node_right.cur_depth + node->balance_factor;
				int node_cur_depth = std::max(node_right.cur_depth, node_left) + 1;
				int node_prev_depth = std::max(node_right.prev_depth, node_left) + 1;

				if (node == prev->left)
				{
					int prev_right = node_prev_depth - prev->balance_factor;
					prev->balance_factor = node_cur_depth - prev_right;
				}
				else if (node == prev->right)
				{
					int prev_left = prev->balance_factor + node_prev_depth;
					prev->balance_factor = prev_left - node_cur_depth;
				}

				return { node_prev_depth, node_cur_depth };
			}
		}
	}
};


enum class Color : bool
{
	black = false,
	red = true
};

template<typename T>
struct RBTreeNode
{
	Color color = Color::red;
	T data = T();
	RBTreeNode* parent = nullptr;
	RBTreeNode* left = nullptr;
	RBTreeNode* right = nullptr;
	explicit RBTreeNode(const T& element) : data(element) {}
	RBTreeNode() = default;
};

template<typename Ty_Key, typename Ty_Val, typename Compare = CompareLess<Ty_Key>, typename Pred = Equal_To<Ty_Key>>
struct RBTree
{
	using Hash = HashNode<Ty_Key, Ty_Val>;
	using RBTreeNode = RBTreeNode<Hash>;
private:
	RBTreeNode* root = nullptr;
public:
	//in order iterator
	struct Iterator
	{
		using pointer = RBTreeNode*;
		using rb_tree = RBTree<Ty_Key, Ty_Val, Compare, Pred>*;
	public:
		Iterator(pointer ptr, rb_tree tree) : ptr(ptr), tree(tree) { }
		Iterator() : ptr(nullptr), tree(nullptr) { }
		Iterator(const Iterator& other)
		{
			ptr = other.ptr;
			tree = other.tree;
		}

		Iterator& operator=(const Iterator& other)
		{
			ptr = other.ptr;
			tree = other.tree;
			return *this;
		}

		Hash& operator*() const
		{
			return ptr->data;
		}

		Hash* operator->() const
		{
			return &ptr->data;
		}

		Iterator& operator++()
		{
			assert(*this != tree->end());

			if (ptr->right != nullptr)
			{
				RBTreeNode* ptr_right = ptr->right;
				while (ptr_right->left != nullptr)
					ptr_right = ptr_right->left;
				ptr = ptr_right;
			}
			else if (ptr == tree->root || ptr == ptr->parent->right)
				ptr = tree->root->parent;
			else if (ptr == ptr->parent->left)
				ptr = ptr->parent;

			return *this;
		}

		explicit operator bool() const
		{
			return ptr != nullptr && tree != nullptr;
		}

		Iterator operator++(int)&
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator!=(const Iterator& other) const
		{
			return ptr != other.ptr || tree != other.tree;
		}

		bool operator==(const Iterator& other) const
		{
			return ptr == other.ptr && tree == other.tree;
		}
	private:
		pointer ptr;
		rb_tree tree;
	};

	RBTree() = default;
	~RBTree()
	{
		if (root)
			destroy();
		root = nullptr;
	}

	RBTree(const std::initializer_list<Hash>& list)
	{
		for (auto iterator = list.begin(); iterator != list.end(); iterator++)
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

	bool count(const Ty_Key& key) const
	{
		assert(root != nullptr);
		RBTreeNode* cur = root;
		while (cur != nullptr)
		{
			if (Pred()(key, cur->data.key))
				break;
			else if (Compare()(key, cur->data.key))
				cur = cur->left;
			else
				cur = cur->right;
		}

		if (cur != nullptr)
			return true;
		else
			return false;
	}

	Ty_Val& operator[](const Ty_Key& key)
	{
		assert(root != nullptr);
		RBTreeNode* cur = root;
		while (cur != nullptr)
		{
			if (Pred()(key, cur->data.key))
				break;
			else if (Compare()(key, cur->data.key))
				cur = cur->left;
			else
				cur = cur->right;
		}

		if (cur != nullptr)
			return cur->data.value;
		else
		{
			insert(key, Ty_Val());
			return operator[](key);
		}
	}

	const Ty_Val& operator[](const Ty_Key& key) const
	{
		assert(root != nullptr);
		RBTreeNode* cur = root;
		while (cur != nullptr)
		{
			if (Pred()(key, cur->data.key))
				break;
			else if (Compare()(key, cur->data.key))
				cur = cur->left;
			else
				cur = cur->right;
		}

		if (cur != nullptr)
			return cur->data.value;
		else
		{
			insert(key, Ty_Val());
			return operator[](key);
		}
	}

	Iterator find(const Ty_Key& key) const
	{
		assert(root != nullptr);
		RBTreeNode* cur = root;
		while (cur != nullptr)
		{
			if (Pred()(key, cur->data.key))
				break;
			else if (Compare()(key, cur->data.key))
				cur = cur->left;
			else
				cur = cur->right;
		}

		if (cur != nullptr)
			return Iterator(cur, this);
		else
			return end();
	}

	Iterator begin()
	{
		assert(root != nullptr);
		RBTreeNode* cur = root;
		while (cur->left != nullptr)
			cur = cur->left;
		return Iterator(cur, this);
	}

	Iterator begin() const
	{
		assert(root != nullptr);
		RBTreeNode* cur = root;
		while (cur->left != nullptr)
			cur = cur->left;
		return Iterator(cur, this);
	}

	Iterator end()
	{
		assert(root != nullptr);
		return Iterator(root->parent, this);
	}

	Iterator end() const
	{
		assert(root != nullptr);
		return Iterator(root->parent, this);
	}

	void destroy()
	{
		Queue<RBTreeNode*> queue;
		if (root)
		{
			delete root->parent;
			queue.enqueue(root);
		}
		while (!queue.empty())
		{
			RBTreeNode* front = queue.front();
			if (front->left)
				queue.enqueue(front->left);
			if (front->right)
				queue.enqueue(front->right);
			queue.dequeue();
			delete front;
		}
	}

	void erase(const Ty_Key& key)
	{
		assert(root != nullptr);
		//with stack
		RBTreeNode* cur = root;
		while (cur != nullptr)
		{
			if (Pred()(key, cur->data.key))
			{
				erase(cur);
				return;
			}
			else if (Compare()(key, cur->data.key))
				cur = cur->left;
			else
				cur = cur->right;
		}
	}

	int height(RBTreeNode* node)
	{
		if (node == nullptr)
			return 0;
		int left = height(node->left);
		int right = height(node->right);
		return std::max(left, right) + 1;
	}

private:
	void erase(RBTreeNode* cur)
	{
		//three conditions
		if (cur->left == nullptr && cur->right == nullptr)
		{
			if (cur->color == Color::red)
			{
				if (cur == cur->parent->left)
					cur->parent->left = nullptr;
				else
					cur->parent->right = nullptr;
			}
			else
			{
				RBTreeNode* cur_parent = cur->parent;
				if (cur == root)
					root = nullptr;
				else if (cur == cur_parent->left)
				{
					RBTreeNode* cur_parent_right = cur_parent->right;
					if (cur_parent_right->color == Color::black)
					{
						if (cur_parent_right->right == nullptr || cur_parent_right->right->color == Color::black)
						{
							if (cur_parent_right->left == nullptr || cur_parent_right->left->color == Color::black)
							{
								cur_parent_right->color = Color::red;
								cur_parent->left = nullptr;
								while (cur_parent->color != Color::red)
									cur_parent = cur_parent->parent;
								cur_parent->color = Color::black;
							}
							else
							{
								Color tmp = cur_parent_right->color;
								cur_parent_right->color = cur_parent_right->left->color;
								cur_parent_right->left->color = tmp;
								right_rotation(cur_parent_right);
								//transfer to another condition
								cur_parent_right = cur_parent->right;
								tmp = cur_parent->color;
								cur_parent->color = cur_parent_right->color;
								cur_parent_right->color = tmp;
								cur_parent_right->right->color = Color::black;
								left_rotation(cur_parent);
								cur_parent->left = nullptr;
							}
						}
						else
						{
							Color tmp = cur_parent->color;
							cur_parent->color = cur_parent_right->color;
							cur_parent_right->color = tmp;
							cur_parent_right->right->color = Color::black;
							left_rotation(cur_parent);
							cur_parent->left = nullptr;
						}
					}
					else
					{
						Color tmp = cur_parent->color;
						cur_parent->color = cur_parent_right->color;
						cur_parent_right->color = tmp;
						erase(cur);
					}
				}
				else if (cur == cur_parent->right)
				{
					RBTreeNode* cur_parent_left = cur_parent->left;
					if (cur_parent_left->color == Color::black)
					{
						if (cur_parent_left->left == nullptr || cur_parent_left->left->color == Color::black)
						{
							if (cur_parent_left->right == nullptr || cur_parent_left->right->color == Color::black)
							{
								cur_parent_left->color = Color::red;
								cur_parent->right = nullptr;
								while (cur_parent->color != Color::red)
									cur_parent = cur_parent->parent;
								cur_parent->color = Color::black;
							}
							else
							{
								Color tmp = cur_parent_left->color;
								cur_parent_left->color = cur_parent_left->right->color;
								cur_parent_left->right->color = tmp;
								left_rotation(cur_parent_left);
								//transfer to another condition
								cur_parent_left = cur_parent->left;
								tmp = cur_parent->color;
								cur_parent->color = cur_parent_left->color;
								cur_parent_left->color = tmp;
								cur_parent_left->left->color = Color::black;
								right_rotation(cur_parent);
								cur_parent->right = nullptr;
							}
						}
						else
						{
							Color tmp = cur_parent->color;
							cur_parent->color = cur_parent_left->color;
							cur_parent_left->color = tmp;
							cur_parent_left->left->color = Color::black;
							right_rotation(cur_parent);
							cur_parent->right = nullptr;
						}
					}
					else
					{
						Color tmp = cur_parent->color;
						cur_parent->color = cur_parent_left->color;
						cur_parent_left->color = tmp;
						erase(cur);
					}
				}
			}
			delete cur;
		}
		else if (cur->left != nullptr && cur->right != nullptr)
		{
			//use the second way to delete cur node
			//exchange deleted node with its direct prev or direct next in order
			RBTreeNode* cur_right_leftest = cur->right;
			while (cur_right_leftest->left != nullptr)
				cur_right_leftest = cur_right_leftest->left;

			//exchange data and invoke erase again
			Hash tmp = cur->data;
			cur->data = cur_right_leftest->data;
			cur_right_leftest->data = tmp;
			erase(cur_right_leftest);
		}
		else if (cur->left != nullptr)
		{
			if (cur == root)
			{
				cur->left->parent = cur->parent;
				cur->left->color = Color::black;
				root = cur->left;
			}
			else
			{
				RBTreeNode* cur_parent = cur->parent;
				if (cur == cur_parent->left)
					cur_parent->left = cur->left;
				else
					cur_parent->right = cur->left;
				cur->left->parent = cur_parent;
				cur->left->color = Color::black;
			}
			delete cur;
		}
		else if (cur->right != nullptr)
		{
			if (cur == root)
			{
				cur->right->parent = cur->parent;
				cur->right->color = Color::black;
				root = cur->right;
			}
			else
			{
				RBTreeNode* cur_parent = cur->parent;
				if (cur == cur_parent->left)
					cur_parent->left = cur->right;
				else
					cur_parent->right = cur->right;
				cur->right->parent = cur_parent;
				cur->right->color = Color::black;
			}
			delete cur;
		}
	}

	//two rotation
	void left_rotation(RBTreeNode* cur)
	{
		RBTreeNode* cur_right = cur->right;
		RBTreeNode* cur_parent = cur->parent;
		bool is_left = cur == cur_parent->left;
		bool is_root = cur == root;

		if (cur_right->left != nullptr)
			cur_right->left->parent = cur;
		cur->right = cur_right->left;
		cur->parent = cur_right;
		cur_right->left = cur;

		if (is_root)
		{
			cur_right->parent = cur_parent;
			root = cur_right;
		}
		else
		{
			cur_right->parent = cur_parent;
			if (is_left)
				cur_parent->left = cur_right;
			else
				cur_parent->right = cur_right;
		}
	}

	void right_rotation(RBTreeNode* cur)
	{
		RBTreeNode* cur_left = cur->left;
		RBTreeNode* cur_parent = cur->parent;
		bool is_left = cur == cur_parent->left;
		bool is_root = cur == root;

		if (cur_left->right != nullptr)
			cur_left->right->parent = cur;
		cur->left = cur_left->right;
		cur->parent = cur_left;
		cur_left->right = cur;

		if (is_root)
		{
			cur_left->parent = cur_parent;
			root = cur_left;
		}
		else
		{
			cur_left->parent = cur_parent;
			if (is_left)
				cur_parent->left = cur_left;
			else
				cur_parent->right = cur_left;
		}
	}

	void insert(const Hash& element, RBTreeNode*& node, RBTreeNode* prev)
	{
		if (node == nullptr)
		{
			node = new RBTreeNode(element);
			if (node == root)
			{
				node->color = Color::black;
				node->parent = new RBTreeNode;
			}
			else if (prev->color == Color::red)
			{
				node->parent = prev;
				RBTreeNode* node_parent = node->parent;
				RBTreeNode* node_grand = node_parent->parent;
				if (node == prev->left)
				{
					if (node_parent == node_grand->left)
					{
						right_rotation(node_grand);
						node_grand->color = Color::red;
						node_parent->color = Color::black;
					}
					else
					{
						right_rotation(node_parent);
						left_rotation(node_grand);
						node_grand->color = Color::red;
						node_parent->parent->color = Color::black;
					}
				}
				else
				{
					if (node_parent == node_grand->left)
					{
						left_rotation(node_parent);
						right_rotation(node_grand);
						node_grand->color = Color::red;
						node_parent->parent->color = Color::black;
					}
					else
					{
						left_rotation(node_grand);
						node_grand->color = Color::red;
						node_parent->color = Color::black;
					}
				}
			}
			else
			{
				node->color = Color::red;
				node->parent = prev;
			}
		}
		else if (Pred()(element.key, node->data.key))
			node->data.value = element.value;
		else if (Compare()(element.key, node->data.key))
		{
			//adjust to avoid complicated condition when two subtree are both red
			if (node->left != nullptr && node->right != nullptr &&
				node->left->color == Color::red && node->right->color == Color::red)
			{
				node->left->color = Color::black;
				node->right->color = Color::black;
				if (node != root)
				{
					node->color = Color::red;
					if (node->parent->color == Color::red)
					{
						RBTreeNode* node_parent = node->parent;
						RBTreeNode* node_grand = node_parent->parent;
						if (node_parent == node_grand->left)
						{
							right_rotation(node_grand);
							node_grand->color = Color::red;
							node_parent->color = Color::black;
						}
						else
						{
							right_rotation(node_parent);
							left_rotation(node_grand);
							node_grand->color = Color::red;
							node->color = Color::black;
							insert(element, node_parent->right->left, node_parent->right);
							return;
						}
					}
				}
			}
			insert(element, node->left, node);
		}
		else
		{
			//adjust to avoid complicated condition when two subtree are both red
			if (node->left != nullptr && node->right != nullptr &&
				node->left->color == Color::red && node->right->color == Color::red)
			{
				node->left->color = Color::black;
				node->right->color = Color::black;
				if (node != root)
				{
					node->color = Color::red;
					if (node->parent->color == Color::red)
					{
						RBTreeNode* node_parent = node->parent;
						RBTreeNode* node_grand = node_parent->parent;
						if (node_parent == node_grand->left)
						{
							left_rotation(node_parent);
							right_rotation(node_grand);
							node_grand->color = Color::red;
							node->color = Color::black;
							insert(element, node_grand->left->right, node_grand->left);
							return;
						}
						else
						{
							left_rotation(node_grand);
							node_grand->color = Color::red;
							node_parent->color = Color::black;
						}
					}
				}
			}
			insert(element, node->right, node);
		}
	}
};


//B tree node
template<typename T, uint32_t Num = 3, typename Compare = CompareLess<T>, typename Pred = Equal_To<T>>
struct BTNode
{
	uint32_t num;
	BTNode* parent;
	Vector<T> data;
	Vector<BTNode*> children;
	BTNode() : num(0), parent(nullptr) {}
	~BTNode()
	{
		if (!data.empty())
			destroy();
	}

public:
	void insert(const T& element)
	{
		if (data.empty())
		{
			num = 1;
			data.push_back(element);
		}
		else
		{
			uint32_t ret = find(element);
			if (Pred()(element, data[ret]))
				data[ret] = element;
			else
			{
				data.insert(ret, element);
				num += 1;
			}
		}
	}

	void insert(const uint32_t& pos, const T& element)
	{
		data.insert(pos, element);
		num += 1;
	}

	void erase(const uint32_t& pos)
	{
		data.erase(pos);
		num -= 1;
	}

	uint32_t find(const T& element) const
	{
		uint32_t left = 0, right = data.size() - 1;
		while (left <= right && right < data.size())
		{
			uint32_t middle = (left + right) >> 1;
			if (Compare()(element, data[middle]))
				right = middle - 1;
			else if (Pred()(element, data[middle]))
				return middle;
			else
				left = middle + 1;
		}
		//the right position of the target
		return left;
	}

	void destroy()
	{
		if (!data.empty())
			data.destroy();
		if (!children.empty())
			children.destroy();
	}
};

template<typename Hash, typename Compare>
struct CompareHash
{
	bool operator()(const Hash& one, const Hash& the_other) const
	{
		return Compare()(one.key, the_other.key);
	}
};

template<typename Hash, typename Pred>
struct EqualHash
{
	bool operator()(const Hash& one, const Hash& the_other) const
	{
		return Pred()(one.key, the_other.key);
	}
};

template<typename Ty_Key, typename Ty_Val, uint32_t Num = 3, typename Compare = CompareLess<Ty_Key>, typename Pred = Equal_To<Ty_Key>>
struct BTree
{
	using Hash = HashNode<Ty_Key, Ty_Val>;
	using BTNode = BTNode<Hash, Num, CompareHash<Hash, Compare>, EqualHash<Hash, Pred>>;
public:
	BTNode* root;
public:
	BTree() : root(nullptr) { assert(Num > 2); }

	void insert(const Hash& element)
	{
		if (root == nullptr)
		{
			root = new BTNode;
			root->insert(element);
		}
		else
		{
			BTNode* cur = root;
			uint32_t ret = 0;
			while (true)
			{
				ret = cur->find(element);
				if (ret >= cur->data.size() || !Pred()(element.key, cur->data[ret].key))
				{
					if (cur->children.empty())
						break;
					else
						cur = cur->children[ret];
				}
				else
				{
					cur->data[ret].value = element.value;
					return;
				}
			}

			cur->insert(ret, element);
			if (cur->num >= Num)
				insert_adjust(cur);
		}
	}

	void insert(const Ty_Key& key, const Ty_Val& val)
	{
		Hash element(key, val);
		insert(element);
	}

	void erase(const Ty_Key& key)
	{
		//exchange and delete leaf node
		BTNode* cur = root;
		BTNode* correct = nullptr;

		Hash hash(key, Ty_Val());
		uint32_t ret = 0;
		uint32_t index = 0;
		while (true)
		{
			ret = cur->find(hash);
			if (ret >= cur->data.size() || !Pred()(key, cur->data[ret].key))
			{
				if (cur->children.empty())
					break;
				else
					cur = cur->children[ret];
			}
			else
			{
				index = ret;
				correct = cur;
				if (cur->children.empty())
					break;
				else
					cur = cur->children[ret + 1];;
			}
		}

		if (correct != nullptr)
		{
			correct->data[index] = cur->data[ret];
			if (cur->num >= (Num + 1) >> 1 || cur == root)
				cur->erase(ret);
			else
				merge_adjust(cur, ret);
		}
		else if (Pred()(key, cur->data[ret].key))
			cur->erase(ret);
	}

	Ty_Val& find(const Ty_Key& key) const
	{
		assert(root != nullptr);
		BTNode* cur = root;
		Hash hash(key, Ty_Val());
		while (cur != nullptr)
		{
			uint32_t ret = cur->find(hash);
			if (ret >= cur->data.size() || !Pred()(key, cur->data[ret].key))
				cur = cur->children[ret];
			else
				return cur->data[ret].value;
		}
		return hash.value;
	}

private:
	void merge_adjust(BTNode* cur, const uint32_t& ret)
	{
		BTNode* cur_parent = cur->parent;
		uint32_t parent_index = cur_parent->find(cur->data[ret]);
		if (parent_index != cur_parent->num && cur_parent->children[parent_index + 1]->num >= (Num + 1) >> 1)
			left_adjust(cur, parent_index);
		else if (parent_index != 0 && cur_parent->children[parent_index - 1]->num >= (Num + 1) >> 1)
			right_adjust(cur, parent_index - 1);
		else if (parent_index != 0)
		{
			//select one of the brother node to merge, here we choose left
			Hash parent_element = cur_parent->data[parent_index - 1];
			BTNode* left = cur_parent->children[parent_index - 1];
			left->insert(left->num, parent_element);

			cur_parent->erase(parent_index - 1);
			cur_parent->children.erase(parent_index);

			uint32_t length = cur->num;
			for (uint32_t i = 0; i < length; i++)
			{
				if (i != ret)
					left->insert(left->num, cur->data[0]);
				cur->erase(0);
			}

			if (cur_parent == root && cur_parent->num == 0)
				root = left;
			else if (cur_parent != root && cur_parent->num < ((Num + 1) >> 1) - 1)
				merge_adjust(cur_parent, cur_parent->num);

			cur->destroy();
			delete cur;
			cur = nullptr;
		}
		else
		{
			Hash parent_element = cur_parent->data[parent_index];
			BTNode* right = cur_parent->children[parent_index + 1];
			right->insert(0, parent_element);

			cur_parent->erase(parent_index);
			cur_parent->children.erase(parent_index);

			uint32_t length = cur->num;
			for (uint32_t i = 0; i < length; i++)
			{
				if (i != length - ret - 1)
					right->insert(0, cur->data[cur->num - 1]);
				cur->erase(cur->num - 1);
			}

			if (cur_parent == root && cur_parent->num == 0)
				root = right;
			else if (cur_parent != root && cur_parent->num < ((Num + 1) >> 1) - 1)
				merge_adjust(cur_parent, cur_parent->num);

			cur->destroy();
			delete cur;
			cur = nullptr;
		}

		if (cur != nullptr)
			cur->erase(ret);
	}

	void right_adjust(BTNode* cur, const uint32_t& left_pos)
	{
		BTNode* cur_parent = cur->parent;
		BTNode* left = cur_parent->children[left_pos];
		Hash parent_data = cur_parent->data[left_pos];
		Hash left_data = left->data[left->num - 1];

		cur_parent->data[left_pos] = left_data;
		cur->insert(0, parent_data);
		left->erase(left->num - 1);
	}

	void left_adjust(BTNode* cur, const uint32_t& right_pos)
	{
		BTNode* cur_parent = cur->parent;
		BTNode* right = cur_parent->children[right_pos + 1];
		Hash parent_data = cur_parent->data[right_pos];
		Hash right_data = right->data[0];

		cur_parent->data[right_pos] = right_data;
		cur->insert(cur->num, parent_data);
		right->erase(0);
	}

	void insert_adjust(BTNode* cur)
	{
		auto new_node = new BTNode;
		BTNode* cur_parent = cur->parent;
		//upscale
		uint32_t mid = (Num + 1) >> 1;
		uint32_t mid_pos = mid - 1;
		uint32_t left_size = mid_pos;
		uint32_t right_size = Num >> 1;

		new_node->num = right_size;
		new_node->parent = cur_parent;
		new_node->data.resize(right_size);
		memmove(new_node->data.statistics(), cur->data.statistics() + mid, sizeof(Hash) * right_size);

		if (cur_parent != nullptr)
		{
			uint32_t ret = cur_parent->find(cur->data[mid_pos]);
			cur_parent->insert(ret, cur->data[mid_pos]);
			//insert in ret + 1
			cur_parent->children.insert(ret + 1, new_node);

			if (cur_parent->num >= Num)
				insert_adjust(cur_parent);
		}
		else
		{
			root = new BTNode;
			root->insert(cur->data[mid_pos]);
			root->children.push_back(cur);
			root->children.push_back(new_node);

			cur->parent = root;
			new_node->parent = root;
		}

		cur->num = left_size;
		cur->data.resize(left_size);
	}
};