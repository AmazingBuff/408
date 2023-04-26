#pragma once

#include<initializer_list>

template<typename T>
struct List
{
	T data;
	List<T>* next = nullptr;
	explicit List(const T& element) : data(element) { }
    List() { memset(this, 0, sizeof(List));}
};

template<typename T>
struct LinkedList
{
private:
	List<T>* head = nullptr;
	List<T>* tail = nullptr;
	uint32_t length = 0;
public:
	LinkedList()
	{
		head = new List<T>;
		tail = new List<T>;
		head->next = tail;
		tail = head;
	}

	LinkedList(const std::initializer_list<T>& list)
	{
		head = new List<T>;
		tail = new List<T>;
		head->next = tail;
		tail = head;
		length = list.size();
		auto iterator = list.begin();
		for (uint32_t i = 0; i < length; i++)
			push_back(*iterator++);
	}

	explicit LinkedList(List<T>* list, bool hasHead = false)
	{
		if (hasHead)
			head = list;
		else
		{
			head = new List<T>;
			head->next = list;
		}
		List<T>* cur = head;
		while (cur->next)
		{
			cur = cur->next;
			length++;
		}
		tail = cur;
		tail->next = new List<T>;
	}

	~LinkedList()
	{
		if (length)
			destroy();
		delete tail->next;
		delete head;
		tail = nullptr;
		head = nullptr;
	}

	void push_back(const T& element)
	{
		length++;
		tail->next->data = element;
		tail = tail->next;
		auto node = new List<T>;
		assert(node != nullptr);
		tail->next = node;
	}

	void pop_back()
	{
		assert(length > 0);
		length--;
		delete tail->next;
		List<T>* cur = head;
		for (uint32_t i = 0; i < length; i++)
			cur = cur->next;
		tail = cur;
	}

	void push_front(const T& element)
	{
		length++;
		auto node = new List<T>(element);
		assert(node != nullptr);
		node->next = head->next;
		head->next = node;
		if (tail == head)
			tail = node;
	}

	void pop_front()
	{
		assert(length > 0);
		length--;
		List<T>* node = head->next;
		head->next = node->next;
		if (tail == node)
			tail = head;
		delete node;
	}

	void push(const T& element, uint32_t pos)
	{
		assert(pos <= length);
		if (pos == 0)
			push_front(element);
		else if (pos == length)
			push_back(element);
		else
		{
			length++;
			List<T>* cur = head;
			while (pos)
			{
				cur = cur->next;
				pos--;
			}
			auto node = new List<T>(element);
			assert(node != nullptr);
			node->next = cur->next;
			cur->next = node;
		}
	}

	void pop(uint32_t pos)
	{
		assert(pos < length);
		if (pos == 0)
			pop_front();
		else if (pos == length - 1)
			pop_back();
		else
		{
			length--;
			List<T>* cur = head;
			while (pos)
			{
				cur = cur->next;
				pos--;
			}
			List<T>* node = cur->next;
			cur->next = cur->next->next;
			delete node;
		}
	}

	uint32_t find(const T& value) const
	{
		List<T>* cur = head->next;
		uint32_t index = 0;
		while (cur != tail->next)
		{
			if (cur->data == value)
				return index;
			else
			{
				cur = cur->next;
				index++;
			}
		}
		return length;
	}

	bool count(const T& value) const
	{
		List<T>* cur = head->next;
		while (cur != tail->next)
		{
			if (cur->data == value)
				return true;
			else
				cur = cur->next;
		}
		return false;
	}

	uint32_t findMin(bool(*compare_lesser)(T, T)) const
	{
		return findMin(compare_lesser, length);
	}

	uint32_t findMin(bool(*compare_lesser)(T, T), uint32_t expectedIndex) const
	{
		List<T>* cur = head->next;
		uint32_t index = 0;
		T min = cur->data;
		for (uint32_t i = 1; i < length; i++)
		{
			cur = cur->next;
			if (compare_lesser(cur->data, min) && i != expectedIndex)
			{
				min = cur->data;
				index = i;
			}
		}
		return index;
	}

	uint32_t findMax(bool(*compare_more)(T, T)) const
	{
		return findMax(compare_more, length);
	}

	uint32_t findMax(bool(*compare_more)(T, T), uint32_t expectedIndex) const
	{
		List<T>* cur = head->next;
		uint32_t index = 0;
		T max = cur->data;
		for (uint32_t i = 1; i < length; i++)
		{
			cur = cur->next;
			if (compare_more(cur->data, max) && i != expectedIndex)
			{
				max = cur->data;
				index = i;
			}
		}
		return index;
	}

	T& operator[](uint32_t index)
	{
		assert(index < length);
		List<T>* cur = head->next;
		while (index--)
			cur = cur->next;
		return cur->data;
	}

	const T& operator[](uint32_t index) const
	{
		assert(index < length);
		List<T>* cur = head->next;
		while (index--)
			cur = cur->next;
		return cur->data;
	}

	uint32_t size() const
	{
		return length;
	}

	bool empty() const
	{
		return length == 0;
	}

	void destroy()
	{
		while (length)
			pop_back();
	}

	List<T>* begin()
	{
		return head->next;
	}

	const List<T>* cbegin() const
	{
		return head->next;
	}

	List<T>* end()
	{
		return tail->next;
	}

	const List<T>* cend() const
	{
		return tail->next;
	}
};
