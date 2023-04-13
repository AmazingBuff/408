#pragma once

#include"vector.hpp"
#include"list.hpp"

template<typename Ty_Key, typename Ty_Val>
struct HashNode
{
	Ty_Key key;
	Ty_Val value;
	HashNode() { memset(this, 0, sizeof(HashNode<Ty_Key, Ty_Val>)); }
	HashNode(const Ty_Key& key, const Ty_Val val) : key(key), value(val) {}
    HashNode(const HashNode& other)
    {
        key = other.key;
        value = other.value;
    }
	bool operator==(const HashNode& other) const
	{
		return key == other.key && value == other.value;
	}

	bool operator!=(const HashNode& other) const
	{
		return key != other.key || value != other.value;
	}
};

template<typename Ty_Key>
struct Hash_Func
{
	uint32_t operator()(const Ty_Key& value) const
	{
		return static_cast<uint32_t>(value);
	}
};

template<typename Ty_Key>
struct Equal_To
{
	bool operator()(const Ty_Key& a, const Ty_Key& b) const
	{
		return a == b;
	}
};

template<typename Ty_Key, typename Ty_Val, typename Hash = Hash_Func<Ty_Key>, typename Pred = Equal_To<Ty_Key>>
struct HashMap
{
public:
	struct Iterator
	{
		using pointer = HashNode<Ty_Key, Ty_Val>*;
		using hash_table = HashMap<Ty_Key, Ty_Val, Hash, Pred>*;
	public:
		Iterator(pointer ptr, hash_table hash) : ptr(ptr), hash(hash) { }
		Iterator() : ptr(nullptr), hash(nullptr) { }
		Iterator(const Iterator& other)
		{
			ptr = other.ptr;
			hash = other.hash;
		}

		Iterator& operator=(const Iterator& other)
		{
			ptr = other.ptr;
			hash = other.hash;
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
			uint32_t index = hash->hashFunction(ptr->key);
			List<HashNode<Ty_Key, Ty_Val>*>* node = hash->buckets[index].begin();
			while (node != hash->buckets[index].end())
			{
				if (Pred()(node->data->key, ptr->key))
					break;
				node = node->next;
			}

			if (node->next != hash->buckets[index].end())
				ptr = node->next->data;
			else
			{
				for (uint32_t i = index + 1; i < hash->capacity; i++)
				{
					if (!hash->buckets[i].empty())
					{
						ptr = hash->buckets[i].begin()->data;
						break;
					}
				}
			}
			return *this;
		}

		explicit operator bool() const
		{
			return ptr != nullptr && hash != nullptr;
		}

		Iterator operator++(int)&
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator!=(const Iterator& other) const
		{
			return ptr != other.ptr || hash != other.hash;
		}

		bool operator==(const Iterator& other) const
		{
			return ptr == other.ptr && hash == other.hash;
		}

	private:
		pointer ptr;
		hash_table hash;
	};

	explicit HashMap(const uint32_t& capacity = 100) : capacity(capacity), length(0)
	{
		buckets.resize(capacity);
	}

	~HashMap()
	{
		if (!buckets.empty())
			buckets.destroy();
	}

	void insert(const Ty_Key& key, const Ty_Val& value)
	{
		operator[](key) = value;
	}

	void erase(const Ty_Key& key)
	{
		assert(length > 0);
		uint32_t index = hashFunction(key);
		List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
		uint32_t pos = 0;
		while (node)
		{
			if (Pred()(node->data->key, key))
			{
				buckets[index].pop(pos);
				length--;
				return;
			}
			node = node->next;
			pos++;
		}
	}

	Iterator find(const Ty_Key& key)
	{
		uint32_t index = hashFunction(key);
		List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
		while (node != buckets[index].end())
		{
			if (Pred()(node->data->key, key))
				return Iterator(node->data, this);
			node = node->next;
		}

		return end();
	}

	bool count(const Ty_Key& key)
	{
		uint32_t index = hashFunction(key);
		List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
		while (node != buckets[index].end())
		{
			if (Pred()(node->data->key, key))
				return true;
			node = node->next;
		}

		return false;
	}

	const Iterator cend() const
	{
		return Iterator(buckets[capacity - 1].end()->data, this);
	}

	Iterator end()
	{
		return Iterator(buckets[capacity - 1].end()->data, this);
	}

	const Iterator cbegin() const
	{
		for (uint32_t i = 0; i < capacity; i++)
		{
			if (!buckets[i].empty())
				return Iterator(buckets[i].begin()->data, this);
		}

		return Iterator(buckets[0].begin()->data, this);
	}

	Iterator begin()
	{
		for (uint32_t i = 0; i < capacity; i++)
		{
			if (!buckets[i].empty())
				return Iterator(buckets[i].begin()->data, this);
		}

		return Iterator(buckets[0].begin()->data, this);
	}

	const Ty_Val& operator[](const Ty_Key& key) const
	{
		uint32_t index = hashFunction(key);
		List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
		while (node != buckets[index].end())
		{
			if (Pred()(node->data->key, key))
				return node->data->value;
			node = node->next;
		}

		auto hash_node = new HashNode<Ty_Key, Ty_Val>(key, Ty_Val());
		buckets[index].push_back(hash_node);
		length++;
		return hash_node->value;
	}

	Ty_Val& operator[](const Ty_Key& key)
	{
		uint32_t index = hashFunction(key);
		List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
		while (node != buckets[index].end())
		{
			if (Pred()(node->data->key, key))
				return node->data->value;
			node = node->next;
		}

		auto hash_node = new HashNode<Ty_Key, Ty_Val>(key, Ty_Val());
		buckets[index].push_back(hash_node);
		length++;
		return hash_node->value;
	}

	void destroy()
	{
		for (uint32_t i = 0; i < capacity; i++)
		{
			if (!buckets[i].empty())
				buckets[i].destroy();
		}
		length = 0;
	}

	uint32_t size() const
	{
		return length;
	}

	bool empty() const
	{
		return length == 0;
	}
private:
	uint32_t capacity;
	mutable uint32_t length;
	Vector<LinkedList<HashNode<Ty_Key, Ty_Val>*>> buckets;

	uint32_t hashFunction(const Ty_Key& key) const
	{
		return Hash()(key) % capacity;
	}
};
