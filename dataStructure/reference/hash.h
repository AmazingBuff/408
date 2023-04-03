#include"vector.h"
#include"list.h"

template<typename Ty_Key, typename Ty_Val>
struct HashNode
{
    Ty_Key key;
    Ty_Val value;
    HashNode() { memset(this, 0, sizeof(HashNode<Ty_Key, Ty_Val>)); }
    HashNode(const Ty_Val& key, const Ty_Val val) : key(key), value(val) {}
};


template<typename Ty_Key, typename Ty_Val>
struct HashTable
{
public:
    HashTable(const uint32_t& size = 100) : size(size)
    {
        buckets.resize(size);
    }
private:
    uint32_t size;
    Vector<LinkedList<HashNode<Ty_Key, Ty_Val>*>> buckets;
    uint32_t hashIndex() const
    {

    }
};

