#include"hash.hpp"

//adjacency list
template<typename T>
struct AdjArcNode
{
    uint32_t vertexIndex;
    AdjArcNode* nextArc = nullptr;
    T information;
    explicit AdjArcNode(const uint32_t& vertex, const T& info = 0) : vertexIndex(vertex), information(info) {}
};

template<typename T, typename U>
struct AdjVertexNode
{
    T data;
    AdjArcNode<U>* firstArc;
    AdjVertexNode() : data(T()), firstArc(nullptr) { }
    explicit AdjVertexNode(const T& element, AdjArcNode<U>* arc = nullptr) : data(element), firstArc(arc) {}
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
    OrthVertexNode() : data(T()), firstIn(nullptr), firstOut(nullptr) { }
    explicit OrthVertexNode(const T& element, OrthArcNode<U>* arcIn = nullptr, OrthArcNode<U>* arcOut = nullptr)
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
    AdjMultVertexNode() : data(T()), firstArc(nullptr) { }
    explicit AdjMultVertexNode(const T& element, AdjMultArcNode<U>* arc = nullptr) : data(element), firstArc(arc) {}
};


template<typename T>
struct Arc
{
    uint32_t headVertex;
    uint32_t tailVertex;
    T information;
    Arc() : headVertex(0), tailVertex(0), information(T()) { }
    Arc(const uint32_t& head, const uint32_t& tail, const T& info = 0)
      : headVertex(head), tailVertex(tail), information(info) { }
};

typedef Arc<uint32_t> ArcWeight;

template<typename T, typename U = uint32_t>
struct Graph
{
public:
    enum class Type : uint8_t
    {
        adjacency_list,
        reverse_adjacency_list,
        orthogonal_list,
        //only for undigraph
        adjacency_multlist
    };

    enum class Mode : bool
    {
        depth_first_search,
        broadth_first_search
    };

    Graph(Type type_, Vector<T>& vertices, Vector<Arc<U>>& arcs) : type(type_)
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
            case Type::adjacency_multlist:
                createAdjMultList(vertices, arcs);
                break;
            default:
                break;
        }
    }

    void access(Mode mode, void(*visit)(T), uint32_t node = 0)
    {
        switch (mode)
        {
            case Mode::depth_first_search:
                switch (type)
                {
                    case Type::adjacency_list:
                        AdjListDFS(visit, node);
                        break;
                    case Type::reverse_adjacency_list:
                        ReAdjListDFS(visit, node);
                        break;
                    case Type::orthogonal_list:
                        OrthListDFS(visit, node);
                        break;
                    case Type::adjacency_multlist:
                        AdjMultListDFS(visit, node);
                        break;
                    default:
                        break;
                }
                break;
            case Mode::broadth_first_search:
                switch (type)
                {
                    case Type::adjacency_list:
                        AdjListBFS(visit, node);
                        break;
                    case Type::reverse_adjacency_list:
                        ReAdjListBFS(visit, node);
                        break;
                    case Type::orthogonal_list:
                        OrthListBFS(visit, node);
                        break;
                    case Type::adjacency_multlist:
                        AdjMultListBFS(visit, node);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    void reset(Type type_, Vector<T>& vertices, Vector<Arc<U>>& arcs)
    {
        switch (this->type)
        {
            case Type::adjacency_list:
                destroyAdjList();
                break;
            case Type::reverse_adjacency_list:
                destroyReAdjList();
                break;
            case Type::orthogonal_list:
                destroyOrthList();
                break;
            case Type::adjacency_multlist:
                destroyAdjMultList();
                break;
            default:
                break;
        }

        this->type = type_;
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
            case Type::adjacency_multlist:
                createAdjMultList(vertices, arcs);
                break;
            default:
                break;
        }
    }

    ~Graph()
    {
        switch (type)
        {
            case Type::adjacency_list:
                destroyAdjList();
                break;
            case Type::reverse_adjacency_list:
                destroyReAdjList();
                break;
            case Type::orthogonal_list:
                destroyOrthList();
                break;
            case Type::adjacency_multlist:
                destroyAdjMultList();
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
    Type type;

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
            auto arc_node = new AdjArcNode<U>(arcs[i].headVertex, arcs[i].information);
            if (adjVertices[vertexIndex].firstArc == nullptr)
            {
                adjVertices[vertexIndex].firstArc = arc_node;
                storage[vertexIndex] = adjVertices[vertexIndex].firstArc;
            }
            else
            {
                storage[vertexIndex]->nextArc = arc_node;
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
                prev = nullptr;
            }
        }
        adjVertices.destroy();
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
            auto arc_node = new AdjArcNode<U>(arcs[i].headVertex, arcs[i].information);
            if (reAdjVertices[vertexIndex].firstArc == nullptr)
            {
                reAdjVertices[vertexIndex].firstArc = arc_node;
                storage[vertexIndex] = reAdjVertices[vertexIndex].firstArc;
            }
            else
            {
                storage[vertexIndex]->nextArc = arc_node;
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
                prev = nullptr;
            }
        }
        reAdjVertices.destroy();
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
            auto arcNode = new OrthArcNode<U>(headIndex, tailIndex, arcs[i].information);
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
                prev = nullptr;
            }
        }
        orthVertices.destroy();
    }

    //arcs with two vertices will be treated as an edge, rather than an arc
    void createAdjMultList(Vector<T>& vertices, Vector<Arc<U>>& edges)
    {
        for (uint32_t i = 0; i < vertices.size(); i++)
        {
            AdjMultVertexNode<T, U> vertex(vertices[i]);
            adjMultVertices.push_back(vertex);
        }

        Vector<AdjMultArcNode<U>*> storage(vertices.size());
        for (uint32_t i = 0; i < edges.size(); i++)
        {
            uint32_t headIndex = edges[i].headVertex;
            uint32_t tailIndex = edges[i].tailVertex;
            auto arcNode = new AdjMultArcNode<U>(headIndex, tailIndex, edges[i].information);
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

    void destroyAdjMultList()
    {
        for(uint32_t i = 0; i < adjMultVertices.size(); i++)
        {
            AdjMultArcNode<U>* arcNode = adjMultVertices[i].firstArc;
            while(arcNode)
            {
                AdjMultArcNode<U>* prev = arcNode;
                arcNode = arcNode->headLink;
                delete prev;
                prev = nullptr;
            }
            adjMultVertices.destroy();
        }
    }

    void AdjListRecursive(Vector<uint32_t>& judgement, Vector<AdjArcNode<U>*>& storage, AdjArcNode<U>*& cur, uint32_t order, uint32_t& index)
    {
        if(order < 1)
            return;
        uint32_t next_index = 0;
        do
        {
            next_index = cur->vertexIndex;
            cur = cur->nextArc;
        }while(judgement[next_index] != 0 && cur != nullptr);

        if(judgement[next_index] == 0)
        {
            if(cur != nullptr)
                storage[index] = cur;
            index = next_index;
        }
        else
        {
            next_index = judgement.find(order - 1);
            cur = storage[next_index];
            AdjListRecursive(judgement, storage, cur, order - 1, index);
        }
    }

    void AdjListDFS(void(*visit)(T), uint32_t node = 0)
    {
        uint32_t count = adjVertices.size();
        Vector<uint32_t> judgement(0, count);

        AdjVertexNode<T, U>& access_node = adjVertices[node];
        AdjArcNode<U>* cur = access_node.firstArc;

        Vector<AdjArcNode<U>*> storage(count);
        for(uint32_t i = 0; i < count; i++)
            storage[i] = adjVertices[i].firstArc;

        uint32_t order = 1;
        uint32_t index = node;
        while(order <= count)
        {
            visit(access_node.data);
            judgement[index] = order;

            uint32_t new_index = index;
            AdjListRecursive(judgement, storage, cur, order, new_index);

            if(new_index == index)
                break;
            else
                index = new_index;

            access_node = adjVertices[index];
            cur = storage[index];
            order++;
        }
    }

    void AdjListBFS(void(*visit)(T), uint32_t node = 0) const
    {

    }

    void ReAdjListDFS(void(*visit)(T), uint32_t node = 0) const
    {

    }

    void ReAdjListBFS(void(*visit)(T), uint32_t node = 0) const
    {

    }

    void OrthListDFS(void(*visit)(T), uint32_t node = 0) const
    {

    }

    void OrthListBFS(void(*visit)(T), uint32_t node = 0) const
    {

    }

    void AdjMultListDFS(void(*visit)(T), uint32_t node = 0) const
    {

    }

    void AdjMultListBFS(void(*visit)(T), uint32_t node = 0) const
    {

    }
};