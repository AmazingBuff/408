#include<iostream>
#include"graph.hpp"
#include"search.hpp"
#include"sort.hpp"
#include<chrono>
#include<map>

template<typename T>
void visit(T val)
{
	std::cout << val << std::endl;
}

void visit(char val)
{
	std::cout << val;
}

int main()
{

	Vector<char> vertices = { 'a', 'b', 'c', 'd', 'e', 'f'};
	Vector<Arc<uint8_t>> arcs = { {0, 1, 6}, {0, 2, 1}, {0, 3, 5},
                               {1, 0, 6}, {1, 2, 5}, {1, 4, 3},
                               {2, 0, 1}, {2, 1, 5}, {2, 3, 5}, {2, 4, 6}, {2, 5, 4},
                               {3, 0, 5}, {3, 2, 5}, {3, 5, 2},
                               {4, 1, 3}, {4, 2, 6}, {4, 5, 6},
                               {5, 2, 4}, {5, 3, 2}, {5, 4, 6}};
	Graph<char, uint8_t> graph(Graph<char, uint8_t>::Type::adjacency_list, vertices, arcs);

    //graph.reset(Graph<char, uint8_t>::Type::orthogonal_list);

    LinkedList<HashNode<uint32_t, char>> ret =  graph.TopologicalSort();

    SpanningTreeNode<char, uint8_t>* nde = graph.Prim(0);

	Vector<Arc<uint8_t>> single_arcs = { {0, 1, 6}, {0, 2, 1}, {0, 3, 5},
                                         {1, 2, 5}, {1, 4, 3},
                                         {2, 3, 5}, {2, 4, 6}, {2, 5, 4},
                                         {3, 5, 2},
                                         {4, 5, 6} };

    Vector<Arc<uint8_t>> topological = {{0, 1, 1}, {0, 2, 1}, {0, 3, 1},
                                        {2, 1, 1}, {2, 5, 1},
                                        {3, 5, 1},
                                        {4, 3, 1}, {4, 5, 1},};

	auto start_time = std::chrono::high_resolution_clock::now();
	graph.access(Graph<char, uint8_t>::Mode::depth_first_search, visit, 1);
	auto end_time = std::chrono::high_resolution_clock::now();
	std::cout << std::endl;
	graph.access(Graph<char, uint8_t>::Mode::broadth_first_search, visit, 1);
	std::cout << std::endl;
	Graph<char, uint8_t> graph2(Graph<char, uint8_t>::Type::orthogonal_list, vertices, arcs);
	graph2.access(Graph<char, uint8_t>::Mode::depth_first_search, visit, 1);
	std::cout << std::endl;
	graph2.access(Graph<char, uint8_t>::Mode::broadth_first_search, visit, 1);
	std::cout << std::endl;

	Graph<char, uint8_t> graph3(Graph<char, uint8_t>::Type::orthogonal_list, vertices, topological);

    LinkedList<HashNode<uint32_t, char>> ret3 =  graph3.TopologicalSort();

	graph3.access(Graph<char, uint8_t>::Mode::depth_first_search, visit, 1);
	std::cout << std::endl;
	graph3.access(Graph<char, uint8_t>::Mode::broadth_first_search, visit, 1);
	std::cout << std::endl;

	std::cout << (end_time - start_time).count() << std::endl;

	HashMap<uint32_t, char> hash;
	hash.insert(100, 'a');
	hash.insert(100, 'e');
	hash.insert(110, 'b');
	hash.insert(10, 'c');
	hash.insert(200, 'd');

	BinarySortTree<uint32_t, char> bst = { {3, 'a'}, {1, '3'}, {4, '4'}, {5, 'f'} };

	bst.erase(4);

	bst.insert(77, 'd');
	bst.insert(77, 'e');

	AVLTreeNoParent<uint32_t, char> avlTree;
	avlTree.insert(77, 'd');
	avlTree.insert(707, 'q');
	avlTree.insert(702, 'f');
	avlTree.insert(802, 'c');
	avlTree.insert(22, 'l');
	avlTree.insert(1000, 'e');

	avlTree.erase(22);
	avlTree.erase(77);

	start_time = std::chrono::high_resolution_clock::now();
	RBTree<uint32_t, char> rbtree;
	rbtree.insert(77, 'd');
	rbtree.insert(707, 'q');
	rbtree.insert(702, 'f');
	rbtree.insert(802, 'c');
	rbtree.insert(22, 'l');
	rbtree.insert(1000, 'e');
	rbtree.insert(900, 'e');
	rbtree.insert(1100, 'e');
	rbtree.insert(1300, 'e');
	rbtree.insert(850, 'e');
	rbtree.insert(950, 'e');
	rbtree.insert(975, 'e');
	rbtree.insert(976, 'e');
	rbtree.insert(978, 'e');
	end_time = std::chrono::high_resolution_clock::now();
	std::cout << (end_time - start_time).count() << std::endl;

	Vector<uint32_t> akkk;
	akkk.push_back(1);
	akkk.push_back(2);
	akkk.push_back(3);
	akkk.push_back(4);
	akkk.push_back(5);
	memmove(akkk.statistics() + 1, akkk.statistics(), (akkk.size() - 1) * sizeof(uint32_t));
	std::cout << akkk[1] << akkk[2] << akkk[3] << akkk[4] << akkk[0] << std::endl;

	start_time = std::chrono::high_resolution_clock::now();
	std::map<uint32_t, char> map;
	map.insert(std::make_pair(77, 'd'));
	map.insert(std::make_pair(707, 'q'));
	map.insert(std::make_pair(702, 'f'));
	map.insert(std::make_pair(802, 'c'));
	map.insert(std::make_pair(22, 'l'));
	map.insert(std::make_pair(1000, 'e'));
	map.insert(std::make_pair(900, 'e'));
	map.insert(std::make_pair(1100, 'e'));
	map.insert(std::make_pair(1300, 'e'));
	map.insert(std::make_pair(850, 'e'));
	map.insert(std::make_pair(950, 'e'));
	map.insert(std::make_pair(975, 'e'));
	map.insert(std::make_pair(976, 'e'));
	map.insert(std::make_pair(978, 'e'));
	end_time = std::chrono::high_resolution_clock::now();
	std::cout << (end_time - start_time).count() << std::endl;

	RBTree<uint32_t, char>::Iterator ity = rbtree.begin();
	ity++;
	++ity;

	BTree<uint32_t, char, 5> btree;
	btree.insert(4, 'a');
	btree.insert(5, 'b');
	btree.insert(7, 'c');
	btree.insert(6, 'd');

	btree.insert(3, 'e');
	btree.insert(22, 'f');
	btree.insert(31, 'g');
	btree.insert(34, 'h');
	btree.insert(21, 'i');

	btree.erase(4);
	btree.erase(5);

	for (uint32_t i = 0; i < btree.root->data.size(); i++)
	{
		std::cout << btree.root->data[i].key << std::endl;
	}

	for (uint32_t i = 0; i < btree.root->children.size(); i++)
	{
		for (uint32_t j = 0; j < btree.root->children[i]->data.size(); j++)
		{
			std::cout << btree.root->children[i]->data[j].key << std::endl;
		}
	}

	HashNode<uint32_t, char> ha = *ity;

	rbtree[802] = '4';

	start_time = std::chrono::high_resolution_clock::now();
	rbtree.erase(850);
	rbtree.erase(975);
	rbtree.erase(978);
	rbtree.erase(1100);
	end_time = std::chrono::high_resolution_clock::now();
	std::cout << (end_time - start_time).count() << std::endl;

	start_time = std::chrono::high_resolution_clock::now();
	map.erase(850);
	map.erase(975);
	map.erase(978);
	map.erase(1100);
	end_time = std::chrono::high_resolution_clock::now();
	std::cout << (end_time - start_time).count() << std::endl;

	HashMap<uint32_t, char>::Iterator it = hash.find(100);
	++it;
	it++;
	HashNode<uint32_t, char> node = *it;

	std::cout << "hello" << it->key << it->value << std::endl;


    Vector<uint32_t> arrr = {4,6,1,4,6,8,11,10,45,24,3,4,1,4,66,1,3,5,56,22};
    Vector<uint32_t> arrr2 = {4,6,1,4,6,8,11,10,45,24,3,4,1,4,66,1,3,5,56,22};
    Vector<uint32_t> arrr3 = {4,6,1,4,6,8,11,10,45,24,3,4,1,4,66,1,3,5,56,22};
    Vector<uint32_t> arrr4 = {4,6,1,4,6,8,11,10,45,24,3,4,1,4,66,1,3,5,56,22};
    Vector<uint32_t> arrr5 = {4,6,1,4,6,8,11,10,45,24,3,4,1,4,66,1,3,5,56,22};
    Vector<uint32_t> arrr6 = {4,6,1,4,6,8,11,10,45,24,3,4,1,4,66,1,3,5,56,22};
    Vector<uint32_t> arrr7 = {4,6,1,4,6,8,11,10,45,24,3,4,1,4,66,1,3,5,56,22};
    Vector<uint32_t> arrr8 = {4,6,1,4,6,8,11,10,45,24,3,4,1,4,66,1,3,5,56,22};
    start_time = std::chrono::high_resolution_clock::now();
    directlyInsertSort(arrr);
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << (end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    binaryInsertSort(arrr2);
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << (end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    twoWayInsertSort(arrr3);
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << (end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    shellSort(arrr4, {5, 3, 1});
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << (end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    bubbleSort(arrr5);
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << (end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    quickSort(arrr6, 0, static_cast<int>(arrr6.size()) - 1);
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << (end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    selectSort(arrr7);
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << (end_time - start_time).count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    heapSort(arrr8);
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << (end_time - start_time).count() << std::endl;

    for(auto& a : arrr)
        std::cout << a << ' ';
    std::cout << std::endl;
    for(auto& a : arrr2)
        std::cout << a << ' ';
    std::cout << std::endl;
    for(auto& a : arrr3)
        std::cout << a << ' ';
    std::cout << std::endl;
    for(auto& a : arrr4)
        std::cout << a << ' ';
    std::cout << std::endl;
    for(auto& a : arrr5)
        std::cout << a << ' ';
    std::cout << std::endl;
    for(auto& a : arrr6)
        std::cout << a << ' ';
    std::cout << std::endl;
    for(auto& a : arrr7)
        std::cout << a << ' ';
    std::cout << std::endl;
    for(auto& a : arrr8)
        std::cout << a << ' ';

	return 0;
}