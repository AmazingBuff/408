#include"vector.h"

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
    AdjArcNode<U>* firstArc = nullptr;
    AdjVertexNode(const T& element) : data(element) {}
    AdjVertexNode(const T& element, AdjArcNode<U>*& arc) : data(element), firstArc(arc) {}
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
    OrthArcNode<U>* firstIn = nullptr;
    OrthArcNode<U>* firstOut = nullptr;
    OrthVertexNode(const T& element) : data(element) {}
    OrthVertexNode(const T& element, OrthArcNode<U>*& arcIn, OrthArcNode<U>*& arcOut) : data(element), firstIn(arcIn), firstOut(arcOut) {}
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
    AdjMultArcNode(const uint32_t& headVertex, const uint32_t& tailVertex, const T& info = 0) : arcHead(headVertex), arcTail(tailVertex), information(info) {}
};

template<typename T, typename U>
struct AdjMultVertexNode
{
    T data;
    AdjMultArcNode<U>* firstArc = nullptr;
    AdjMultVertexNode(const T& element) : data(element) {}
    AdjMultVertexNode(const T& element, AdjMultArcNode<U>*& arc) : data(element), firstArc(arc) {}
};


