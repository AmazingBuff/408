#pragma once

#include"hash.hpp"
#include"queue.hpp"
#include"list.hpp"

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
struct SpanningTreeNode
{
	uint32_t vertexIndex = 0;
	T data;
	SpanningTreeNode* parent = nullptr;
	Vector<SpanningTreeNode*> children;
	Vector<U> arcsWeight; //point out from current node, match with children node
	SpanningTreeNode(const uint32_t& index, const T& value) : vertexIndex(index), data(value) {}
};



template<typename T, typename U = uint32_t>
struct Graph
{
public:
	enum class Type : uint8_t
	{
		adjacency_list,
		reverse_adjacency_list,
		//only for digraph
		orthogonal_list,
		//only for un-digraph
		adjacency_multilist
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
		case Type::adjacency_multilist:
			createAdjMultiList(vertices, arcs);
			break;
		default:
			break;
		}
	}

	void access(const Mode mode, void(*visit)(T), uint32_t node = 0)
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
			case Type::adjacency_multilist:
				AdjMultiListDFS(visit, node);
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
			case Type::adjacency_multilist:
				AdjMultiListBFS(visit, node);
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
		case Type::adjacency_multilist:
			destroyAdjMultiList();
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
		case Type::adjacency_multilist:
			createAdjMultiList(vertices, arcs);
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
		case Type::adjacency_multilist:
			destroyAdjMultiList();
			break;
		default:
			break;
		}
	}

	SpanningTreeNode<T, U>* Prim(uint32_t node) const
	{
		switch (type)
		{
		case Type::adjacency_list:
			return PrimAdjList(node);
		case Type::reverse_adjacency_list:
			return PrimReAdjList(node);
		case Type::adjacency_multilist:
			return PrimAdjMultiList(node);
		default:
			perror("don't support digraph");
			return nullptr;
		}
	}

	LinkedList<HashNode<uint32_t, T>> TopologicalSort() const
	{
		switch (type)
		{
		case Type::adjacency_list:
			return TopologicalSortAdjList();
		case Type::reverse_adjacency_list:
			return TopologicalSortReAdjList();
		case Type::orthogonal_list:
			return TopologicalSortOrthList();
		default:
			perror("don't support un-digraph");
			return LinkedList<HashNode<uint32_t, T>>();
		}
	}
private:
	Vector<AdjVertexNode<T, U>> adjVertices;
	Vector<AdjVertexNode<T, U>> reAdjVertices;
	Vector<OrthVertexNode<T, U>> orthVertices;
	Vector<AdjMultVertexNode<T, U>> adjMultiVertices;
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
	void createAdjMultiList(Vector<T>& vertices, Vector<Arc<U>>& edges)
	{
		for (uint32_t i = 0; i < vertices.size(); i++)
		{
			AdjMultVertexNode<T, U> vertex(vertices[i]);
			adjMultiVertices.push_back(vertex);
		}

		Vector<AdjMultArcNode<U>*> storage(vertices.size());
		for (uint32_t i = 0; i < edges.size(); i++)
		{
			uint32_t headIndex = edges[i].headVertex;
			uint32_t tailIndex = edges[i].tailVertex;
			auto arcNode = new AdjMultArcNode<U>(headIndex, tailIndex, edges[i].information);

			if (adjMultiVertices[headIndex].firstArc == nullptr)
			{
				adjMultiVertices[headIndex].firstArc = arcNode;
				storage[headIndex] = adjMultiVertices[headIndex].firstArc;
			}
			else
			{
				if (storage[headIndex]->arcHead == headIndex)
				{
					storage[headIndex]->headLink = arcNode;
					storage[headIndex] = storage[headIndex]->headLink;
				}
				else if (storage[headIndex]->arcTail == headIndex)
				{
					storage[headIndex]->tailLink = arcNode;
					storage[headIndex] = storage[headIndex]->tailLink;
				}
			}


			if (adjMultiVertices[tailIndex].firstArc == nullptr)
			{
				adjMultiVertices[tailIndex].firstArc = arcNode;
				storage[tailIndex] = adjMultiVertices[tailIndex].firstArc;
			}
			else
			{
				if (storage[tailIndex]->arcHead == tailIndex)
				{
					storage[tailIndex]->headLink = arcNode;
					storage[tailIndex] = storage[tailIndex]->headLink;
				}
				else if (storage[tailIndex]->arcTail == tailIndex)
				{
					storage[tailIndex]->tailLink = arcNode;
					storage[tailIndex] = storage[tailIndex]->tailLink;
				}
			}
		}
	}

	void destroyAdjMultiList()
	{
		for (uint32_t i = 0; i < adjMultiVertices.size(); i++)
		{
			AdjMultArcNode<U>* arcNode = adjMultiVertices[i].firstArc;
			while (arcNode)
			{
				AdjMultArcNode<U>* prev = arcNode;
				arcNode = arcNode->headLink;
				delete prev;
				prev = nullptr;
			}
			adjMultiVertices.destroy();
		}
	}

	void AdjDFS(const Vector<AdjVertexNode<T, U>>& adj, void(*visit)(T), uint32_t node) const
	{
		uint32_t count = adj.size();
		assert(node < count);

		Vector<bool> judgement(false, count);
		Stack<AdjArcNode<U>*> stack;

		uint32_t access_count = 1;
		uint32_t index = node;
		while (access_count <= count)
		{
			const AdjVertexNode<T, U>& access_node = adj[index];
			visit(access_node.data);
			access_count++;
			judgement[index] = true;
			if (access_node.firstArc)
				stack.push(access_node.firstArc);

			AdjArcNode<U>* top = nullptr;
			while (true)
			{
				if (stack.empty())
				{
					index = judgement.find(false);
					break;
				}
				top = stack.top();
				stack.pop();
				while (top != nullptr)
				{
					if (judgement[top->vertexIndex] == false)
					{
						index = top->vertexIndex;
						break;
					}
					top = top->nextArc;
				}

				if (top != nullptr)
				{
					if (top->nextArc != nullptr)
						stack.push(top->nextArc);
					break;
				}
			}
		}
	}

	void AdjBFS(const Vector<AdjVertexNode<T, U>>& adj, void(*visit)(T), uint32_t node) const
	{
		uint32_t count = adj.size();
		assert(node < count);

		Vector<bool> judgement(false, count);
		Queue<uint32_t> queue;
		queue.enqueue(node);

		while (!queue.empty())
		{
			uint32_t index = queue.front();
			queue.dequeue();
			if (!judgement[index])
			{
				const AdjVertexNode<T, U>& access_node = adj[index];
				visit(access_node.data);
				judgement[index] = true;

				AdjArcNode<U>* arc = access_node.firstArc;
				while (arc != nullptr)
				{
					queue.enqueue(arc->vertexIndex);
					arc = arc->nextArc;
				}
			}
		}
	}

	void AdjListDFS(void(*visit)(T), uint32_t node) const
	{
		AdjDFS(adjVertices, visit, node);
	}

	void AdjListBFS(void(*visit)(T), uint32_t node) const
	{
		AdjBFS(adjVertices, visit, node);
	}

	void ReAdjListDFS(void(*visit)(T), uint32_t node) const
	{
		AdjDFS(reAdjVertices, visit, node);
	}

	void ReAdjListBFS(void(*visit)(T), uint32_t node) const
	{
		AdjBFS(reAdjVertices, visit, node);
	}

	void OrthListDFS(void(*visit)(T), uint32_t node) const
	{
		uint32_t count = orthVertices.size();
		assert(node < count);

		Vector<bool> judgement(false, count);
		Stack<OrthArcNode<U>*> stack;

		uint32_t access_count = 1;
		uint32_t index = node;
		while (access_count <= count)
		{
			const OrthVertexNode<T, U>& access_node = orthVertices[index];
			visit(access_node.data);
			access_count++;
			judgement[index] = true;
			if (access_node.firstOut)
				stack.push(access_node.firstOut);

			OrthArcNode<U>* top = nullptr;
			while (true)
			{
				if (stack.empty())
				{
					index = judgement.find(false);
					break;
				}
				top = stack.top();
				stack.pop();
				while (top != nullptr)
				{
					if (judgement[top->arcHead] == false)
					{
						index = top->arcHead;
						break;
					}
					top = top->tailLink;
				}

				if (top != nullptr)
				{
					if (top->tailLink != nullptr)
						stack.push(top->tailLink);
					break;
				}
			}
		}
	}

	void OrthListBFS(void(*visit)(T), uint32_t node) const
	{
		uint32_t count = orthVertices.size();
		assert(node < count);

		Vector<bool> judgement(false, count);
		Queue<uint32_t> queue;
		queue.enqueue(node);

		while (!queue.empty())
		{
			uint32_t index = queue.front();
			queue.dequeue();
			if (!judgement[index])
			{
				const OrthVertexNode<T, U>& access_node = orthVertices[index];
				visit(access_node.data);
				judgement[index] = true;

				OrthArcNode<U>* arc = access_node.firstOut;
				while (arc != nullptr)
				{
					queue.enqueue(arc->arcHead);
					arc = arc->tailLink;
				}
			}
		}
	}

	void AdjMultiListDFS(void(*visit)(T), uint32_t node) const
	{
		uint32_t count = adjMultiVertices.size();
		assert(node < count);

		Vector<bool> judgement(false, count);
		Stack<AdjMultArcNode<U>*> stack;

		uint32_t access_count = 1;
		uint32_t index = node;
		while (access_count <= count)
		{
			const AdjMultVertexNode<T, U>& access_node = adjMultiVertices[index];
			visit(access_node.data);
			access_count++;
			judgement[index] = true;
			if (access_node.firstArc)
				stack.push(access_node.firstArc);

			AdjMultArcNode<U>* top = nullptr;
			uint32_t new_index = 0;
			while (true)
			{
				if (stack.empty())
				{
					index = judgement.find(false);
					break;
				}
				top = stack.top();
				stack.pop();

				while (top != nullptr)
				{
					//judge if it is identity, true for arcHead == index and false for arcTail == index
					bool identity = top->arcHead == index;
					new_index = identity ? top->arcTail : top->arcHead;
					AdjMultArcNode<U>* new_node = identity ? top->headLink : top->tailLink;
					if (!judgement[new_index])
					{
						uint32_t tmp = index;
						index = new_index;
						new_index = tmp;
						break;
					}
					top = new_node;
				}

				if (top != nullptr)
				{
					bool identity = top->arcHead == new_index;
					AdjMultArcNode<U>* new_node = identity ? top->headLink : top->tailLink;
					if (new_node != nullptr)
						stack.push(new_node);
					break;
				}
			}
		}
	}

	void AdjMultiListBFS(void(*visit)(T), uint32_t node) const
	{
		uint32_t count = adjMultiVertices.size();
		assert(node < count);

		Vector<bool> judgement(false, count);
		Queue<uint32_t> queue;
		queue.enqueue(node);

		while (!queue.empty())
		{
			uint32_t index = queue.front();
			queue.dequeue();
			if (!judgement[index])
			{
				const AdjMultVertexNode<T, U>& access_node = adjMultiVertices[index];
				visit(access_node.data);
				judgement[index] = true;

				AdjMultArcNode<U>* arc = access_node.firstArc;
				while (arc != nullptr)
				{
					//judge if it is identity, true for arcHead == index and false for arcTail == index
					bool identity = arc->arcHead == index;
					uint32_t new_index = identity ? arc->arcTail : arc->arcHead;
					AdjMultArcNode<U>* new_node = identity ? arc->headLink : arc->tailLink;
					queue.enqueue(new_index);
					arc = new_node;
				}
			}
		}
	}

	struct CloseEdge
	{
		uint32_t nodeIndex;
		U weight;
		CloseEdge() : nodeIndex(UINT32_MAX), weight(U(0)) {}
		CloseEdge(const uint32_t& index, const U& weight) : nodeIndex(index), weight(weight) {}
	};

	//minimum cost spanning tree of connected net(undigraph)
	SpanningTreeNode<T, U>* PrimAdj(const Vector<AdjVertexNode<T, U>>& adj, uint32_t origin) const
	{
		uint32_t count = adj.size();
		assert(origin < count);

		auto root = new SpanningTreeNode<T, U>(origin, adj[origin].data);
		SpanningTreeNode<T, U>* tree = root;

		//for quickly getting the tree node
		Vector<SpanningTreeNode<T, U>*> map;
		map.resize(count);
		map[origin] = tree;

		HashMap<uint32_t, CloseEdge> closeArcTable;
		uint32_t node = origin;
		while (--count)
		{
			AdjVertexNode<T, U> cur = adj[node];
			AdjArcNode<U>* arc = cur.firstArc;
			//refresh close edge element
			while (arc != nullptr)
			{
				uint32_t index = arc->vertexIndex;
				if (closeArcTable.count(index))
				{
					if (closeArcTable[index].weight > arc->information)
						closeArcTable[index] = CloseEdge(node, arc->information);
				}
				else if (index != origin)
					closeArcTable.insert(index, CloseEdge(node, arc->information));
				arc = arc->nextArc;
			}
			//find the node of the least weight
			uint32_t index = closeArcTable.begin()->key;
			U min_weight;
			memset(&min_weight, 0xffffffff, sizeof(U));
			for (auto it : closeArcTable)
			{
				if (it.value.weight < min_weight && it.value.weight != U(0))
				{
					min_weight = it.value.weight;
					index = it.key;
				}
			}
			//refresh minimum spinning tree
			tree = map[closeArcTable[index].nodeIndex];
			tree->arcsWeight.push_back(closeArcTable[index].weight);
			closeArcTable[index].nodeIndex = UINT32_MAX;
			closeArcTable[index].weight = U(0);

			auto tree_node = new SpanningTreeNode<T, U>(index, adj[index].data);
			tree_node->parent = tree;
			tree->children.push_back(tree_node);
			map[index] = tree_node;

			//refresh loop data
			node = index;
		}
		return root;
	}

	SpanningTreeNode<T, U>* PrimAdjList(uint32_t node) const
	{
		return PrimAdj(adjVertices, node);
	}

	SpanningTreeNode<T, U>* PrimReAdjList(uint32_t node) const
	{
		return PrimAdj(reAdjVertices, node);
	}

	SpanningTreeNode<T, U>* PrimAdjMultiList(uint32_t node) const
	{
		uint32_t count = adjMultiVertices.size();
		assert(node < count);

		auto root = new SpanningTreeNode<T, U>(node, adjMultiVertices[node].data);
		SpanningTreeNode<T, U>* tree = root;

		//for quickly getting the tree node
		Vector<SpanningTreeNode<T, U>*> map;
		map.resize(count);
		map[node] = tree;

		HashMap<uint32_t, CloseEdge> closeArcTable;
		uint32_t cur_index = node;
		while (--count)
		{
			AdjMultVertexNode<T, U> cur = adjMultiVertices[cur_index];
			AdjMultArcNode<U>* arc = cur.firstArc;
			//refresh close edge element
			while (arc != nullptr)
			{
				//traverse along one direction
				uint32_t index = arc->arcHead == cur_index ? arc->arcTail : arc->arcHead;
				if (closeArcTable.count(index))
				{
					if (closeArcTable[index].weight > arc->information)
						closeArcTable[index] = CloseEdge(cur_index, arc->information);
				}
				else if (index != node)
					closeArcTable.insert(index, CloseEdge(cur_index, arc->information));
				arc = arc->arcHead == cur_index ? arc->headLink : arc->tailLink;
			}
			//find the node of the least weight
			uint32_t index = closeArcTable.begin()->key;
			U min_weight = U(0);
			memset(&min_weight, 0xffffffff, sizeof(U));
			for (auto it : closeArcTable)
			{
				if (it.value.weight < min_weight && it.value.weight != U(0))
				{
					min_weight = it.value.weight;
					index = it.key;
				}
			}
			//refresh minimum spinning tree
			tree = map[closeArcTable[index].nodeIndex];
			tree->arcsWeight.push_back(closeArcTable[index].weight);
			closeArcTable[index].nodeIndex = UINT32_MAX;
			closeArcTable[index].weight = U(0);

			auto tree_node = new SpanningTreeNode<T, U>(index, adjMultiVertices[index].data);
			tree_node->parent = tree;
			tree->children.push_back(tree_node);
			map[index] = tree_node;

			//refresh loop data
			cur_index = index;
		}
		return root;
	}

	LinkedList<HashNode<uint32_t, T>> TopologicalSortAdjList() const
	{

	}

	LinkedList<HashNode<uint32_t, T>> TopologicalSortReAdjList() const
	{
        Vector<bool> access_storage(reAdjVertices.size());
        uint32_t index = UINT32_MAX;
		for(uint32_t i = 0; i < reAdjVertices.size(); i++)
        {
            if(reAdjVertices[i].firstArc == nullptr)
            {
                index = i;
                break;
            }
        }
        if(index == UINT32_MAX)
			
	}

	LinkedList<HashNode<uint32_t, T>> TopologicalSortOrthList() const
	{

	}
};