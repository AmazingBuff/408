#include"vector.hpp"
#include"list.hpp"

template<typename Ty_Key, typename Ty_Val>
struct HashNode
{
    Ty_Key key;
    Ty_Val value;
    HashNode() { memset(this, 0, sizeof(HashNode<Ty_Key, Ty_Val>)); }
    HashNode(const Ty_Val& key, const Ty_Val val) : key(key), value(val) {}
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
    public:
        Iterator(const Ty_Key& key) : key(key) { }
        Iterator(const Iterator& other)
        {
            key = other.key;
        }

        Iterator operator=(const Iterator& other)
        {
            key = other.key;
            return *this;
        }

        Iterator& operator++()
        {
            uint32_t index = hashFunction(key);
            List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
            while (node)
            {
                if (Pred(node->data->key, key))
                    break;
                node = node->next;
            }

            if (node->next)
                key = node->next->data->key;
            else
            {
                for (uint32_t i = index + 1; i < size; i++)
                {
                    if (!buckets[i].empty())
                    {
                        key = buckets[i].begin()->data->key;
                        break;
                    }
                }
            }
            return *this;
        }

        Iterator& operator++(uint32_t)
        {
            ++(*this);
            return *this;
        }

    private:
        Ty_Key key;
    };

    HashMap(const uint32_t& size = 100) : size(size)
    {
        buckets.resize(size);
    }

    void insert(const Ty_Key& key, const Ty_Val& value)
    {
        operator[](key) = value;
    }

    void erase(const Ty_Key& key)
    {
        uint32_t index = hashFunction(key);
        List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
        uint32_t pos = 0;
        while (node)
        {
            if (Pred()(node->data->key, key))
            {
                buckets[index].pop(pos);
                return;
            }
            node = node->next;
            pos++;
        }
        return;
    }

    Ty_Val& find(const Ty_Key& key)
    {
        uint32_t index = hashFunction(key);
        List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
        while (node)
        {
            if (Pred()(node->data->key, key))
                return Iterator(node->data->key);
            node = node->next;
        }

        return Ty_Val();
    }

    const Iterator cend() const
    {
        for (uint32_t i = size - 1; i < size; i--)
        {
             if (!buckets[i].empty())
                 return Iterator(buckets[i].end()->data->key);
        }
    }

    Iterator end()
    {
        for (uint32_t i = size - 1; i < size; i--)
        {
            if (!buckets[i].empty())
                return Iterator(buckets[i].end()->data->key);
        }

        return Iterator(Ty_Key());
    }

    const Iterator cbegin() const
    {
        for (uint32_t i = 0; i < size; i++)
        {
            if (!buckets[i].empty())
                return Iterator(buckets[i].begin()->data->key);
        }
    }

    Iterator begin()
    {
        for (uint32_t i = 0; i < size; i++)
        {
            if (!buckets[i].empty())
                return Iterator(buckets[i].begin()->data->key);
        }

        return Iterator(Ty_Key());
    }

    const Ty_Val& operator[](const Ty_Key& key) const
    {
        uint32_t index = hashFunction(key);
        List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
        while (node)
        {
            if (Pred()(node->data->key, key))
                return node->data->value;
            node = node->next;
        }
        return Ty_Val();
    }

    Ty_Val& operator[](const Ty_Key& key)
    {
        uint32_t index = hashFunction(key);
        List<HashNode<Ty_Key, Ty_Val>*>* node = buckets[index].begin();
        while (node)
        {
            if (Pred()(node->data->key, key))
                return node->data->value;
            node = node->next;
        }
        HashNode<Ty_Key, Ty_Val>* hash_node = new HashNode<Ty_Key, Ty_Val>(key, Ty_Val());
        buckets[index].push_back(hash_node);
        return hash_node->value;
    }

private:
    uint32_t size;
    Vector<LinkedList<HashNode<Ty_Key, Ty_Val>*>> buckets;
    uint32_t hashFunction(const Ty_Key& key) const
    {
        return Hash()(key) % size;
    }
};
