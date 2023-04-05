#include"hash.h"

//adjacency list
template<typename T>
struct AdjArcNode
{
    uint32_t vertexIndex;
    AdjArcNode* nextArc = nullptr;
    T information;
    AdjArcNode(const uint32_t& vertex, const T& info = 0) : vertexIndex(vertex), information(info) {}
};

template<typename T, typename U>
struct AdjVertexNode
{
    T data;
    AdjArcNode<U>* firstArc;
    AdjVertexNode(const T& element, AdjArcNode<U>*& arc = nullptr) : data(element), firstArc(arc) {}
};


//orthogonal list
template<typename T>
struct OrthArcNode
{
    uint32_t arcHead;
    uint32_t arcTail;
    OrthArcNode* headLink = nullptr;
    OrthArcNode* tailLink = nullptr;
    T information;
    OrthArcNode(const uint32_t& headVertex, const uint32_t& tailVertex, const T& info = 0) : arcHead(headVertex), arcTail(tailVertex), information(info) {}
};

template<typename T, typename U>
struct OrthVertexNode
{
    T data;
    OrthArcNode<U>* firstIn;
    OrthArcNode<U>* firstOut;
    OrthVertexNode(const T& element) : data(element) {}
    OrthVertexNode(const T& element, OrthArcNode<U>*& arcIn = nullptr, OrthArcNode<U>*& arcOut = nullptr) 
    : data(element), firstIn(arcIn), firstOut(arcOut) {}
};


//adjacency multilist
template<typename T>
struct AdjMultArcNode
{
    bool flag = false;
    uint32_t arcHead;
    uint32_t arcTail;
    AdjMultArcNode* headLink = nullptr;
    AdjMultArcNode* tailLink = nullptr;
    T information;
    AdjMultArcNode(const uint32_t& headVertex, const uint32_t& tailVertex, const T& info = 0) 
    : arcHead(headVertex), arcTail(tailVertex), information(info) {}
};

template<typename T, typename U>
struct AdjMultVertexNode
{
    T data;
    AdjMultArcNode<U>* firstArc;
    AdjMultVertexNode(const T& element, AdjMultArcNode<U>*& arc = nullptr) : data(element), firstArc(arc) {}
};


template<typename T>
struct Arc
{
    uint32_t headVertex;
    uint32_t tailVertex;
    T information;
    Arc(const uint32_t& head, const uint32_t& tail, const T& info = 0)
      : headVertex(head), tailVertex(tail), information(info) { }
};

template<typename T, typename U>
struct Graph
{
public:
    enum class Type : uint8_t
    {
        adjacency_list,
        reverse_adjacency_list,
        orthogonal_list,
        adjacency_multilist
    };

    Graph(Type type, Vector<T>& vertices, Vector<Arc<U>>& arcs)
    {
        switch (type)
        {
            case Type::adjacency_list:
                createAdjList(vertices, arcs);
                break;
            case Type::reverse_adjacency_list:
                createReAdjList(vertices, arcs);
                break;
            case Type::orthogonal_list:
                createOrthList(vertices, arcs);
                break;
            case Type::adjacency_multilist:
                createAdjMultList(vertices, arcs);
                break;
            default:
                break;
        }
    }
private:
    Vector<AdjVertexNode<T, U>> adjVertices;
    Vector<AdjVertexNode<T, U>> reAdjVertices;
    Vector<OrthVertexNode<T, U>> orthVertices;
    Vector<AdjMultVertexNode<T, U>> adjMultVertices;

    void createAdjList(Vector<T>& vertices, Vector<Arc<U>>& arcs)
    {
        for (uint32_t i = 0; i < vertices.size(); i++)
        {
            AdjVertexNode<T, U> vertex(vertices[i]);
            adjVertices.push_back(vertex);
        }

        Vector<AdjArcNode<U>*> storage(vertices.size());
        for (uint32_t i = 0; i < arcs.size(); i++)
        {
            uint32_t vertexIndex = arcs[i].tailVertex;
            if (adjVertices[vertexIndex].firstArc == nullptr)
            {
                adjVertices[vertexIndex].firstArc = new AdjArcNode<U>(arcs[i].headVertex, arcs[i].information);
                storage[vertexIndex] = adjVertices[vertexIndex].firstArc;
            }
            else
            {
                storage[vertexIndex]->nextArc = new AdjArcNode<U>(arcs[i].headVertex, arcs[i].information);
                storage[vertexIndex] = storage[vertexIndex]->nextArc;
            }
        }
    }

    void destroyAdjList()
    {
        for (uint32_t i = 0; i < adjVertices.size(); i++)
        {
            AdjArcNode<U>* firstArc = adjVertices[i].firstArc;
            while (firstArc)
            {
                AdjArcNode<U>* prev = firstArc;
                firstArc = firstArc->nextArc;
                delete prev;
            }
        }
        adjVertices.destory();
    }

    void createReAdjList(Vector<T>& vertices, Vector<Arc<U>>& arcs)
    {
        for (uint32_t i = 0; i < vertices.size(); i++)
        {
            AdjVertexNode<T, U> vertex(vertices[i]);
            reAdjVertices.push_back(vertex);
        }

        Vector<AdjArcNode<U>*> storage(vertices.size());
        for (uint32_t i = 0; i < arcs.size(); i++)
        {
            uint32_t vertexIndex = arcs[i].headVertex;
            if (reAdjVertices[vertexIndex].firstArc == nullptr)
            {
                reAdjVertices[vertexIndex].firstArc = new AdjArcNode<U>(arcs[i].tailVertex, arcs[i].information);
                storage[vertexIndex] = reAdjVertices[vertexIndex].firstArc;
            }
            else
            {
                storage[vertexIndex]->nextArc = new AdjArcNode<U>(arcs[i].tailVertex, arcs[i].information);
                storage[vertexIndex] = storage[vertexIndex]->nextArc;
            }
        }
    }

    void destroyReAdjList()
    {
        for (uint32_t i = 0; i < reAdjVertices.size(); i++)
        {
            AdjArcNode<U>* firstArc = reAdjVertices[i].firstArc;
            while (firstArc)
            {
                AdjArcNode<U>* prev = firstArc;
                firstArc = firstArc->nextArc;
                delete prev;
            }
        }
        reAdjVertices.destory();
    }

    void createOrthList(Vector<T>& vertices, Vector<Arc<U>>& arcs)
    {
        for (uint32_t i = 0; i < vertices.size(); i++)
        {
            OrthVertexNode<T, U> vertex(vertices[i]);
            orthVertices.push_back(vertex);
        }

        Vector<OrthArcNode<U>*> headStorage(vertices.size());
        Vector<OrthArcNode<U>*> tailStorage(vertices.size());
        for (uint32_t i = 0; i < arcs.size(); i++)
        {
            uint32_t headIndex = arcs[i].headVertex;
            uint32_t tailIndex = arcs[i].tailVertex;
            OrthArcNode<U>* arcNode = new OrthArcNode<U>(headIndex, tailIndex, arcs[i].information);
            if (orthVertices[headIndex].firstIn == nullptr)
            {
                orthVertices[headIndex].firstIn = arcNode;
                headStorage[headIndex] = orthVertices[headIndex].firstIn;
            }
            else
            {
                headStorage[headIndex]->headLink = arcNode;
                headStorage[headIndex] = headStorage[headIndex]->headLink;
            }

            if (orthVertices[tailIndex].firstOut == nullptr)
            {
                orthVertices[tailIndex].firstOut = arcNode;
                tailStorage[tailIndex] = orthVertices[tailIndex].firstOut;
            }
            else
            {
                tailStorage[tailIndex]->tailLink = arcNode;
                tailStorage[tailIndex] = tailStorage[tailIndex]->tailLink;
            }
        }
    }

    void destroyOrthList()
    {
        //preform delete operation along one direction
        for (uint32_t i = 0; i < orthVertices.size(); i++)
        {
            OrthArcNode<U>* firstIn = orthVertices[i].firstIn;
            while (firstIn)
            {
                OrthArcNode<U>* prev = firstIn;
                firstIn = firstIn->headLink;
                delete prev;
            }
        }
        orthVertices.destory();
    }

    void createAdjMultList(Vector<T>& vertices, Vector<Arc<U>>& arcs)
    {
        for (uint32_t i = 0; i < vertices.size(); i++)
        {
            AdjMultVertexNode<T, U> vertex(vertices[i]);
            adjMultVertices.push_back(vertex);
        }

        Vector<AdjMultArcNode<U>*> storage(vertices.size());
        for (uint32_t i = 0; i < arcs.size(); i++)
        {
            uint32_t headIndex = arcs[i].headVertex;
            uint32_t tailIndex = arcs[i].tailVertex;
            AdjMultArcNode<U>* arcNode = new AdjMultArcNode<U>(headIndex, tailIndex, arcs[i].information);
            if (adjMultVertices[headIndex].firstArc == nullptr)
            {
                adjMultVertices[headIndex].firstArc = arcNode;
                storage[headIndex] = adjMultVertices[headIndex].firstArc;
            }
            else
            {
                storage[headIndex]->headLink = arcNode;
                storage[headIndex] = storage[headIndex]->headLink;
            }

            if (adjMultVertices[tailIndex].firstArc == nullptr)
            {
                adjMultVertices[tailIndex].firstArc = arcNode;
                storage[tailIndex] = adjMultVertices[tailIndex].firstArc;
            }
            else
            {
                storage[tailIndex]->tailLink = arcNode;
                storage[tailIndex] = storage[tailIndex]->tailLink;
            }
        }
    }

    void destoryAdjMultList()
    {

    }
};