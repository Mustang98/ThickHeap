#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

const int INF = 1e9 + 100, maximumValue = 1000000000, maximumOperationsCount = 1000000;

struct Node
{
	int value;
	Node* parent = nullptr;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* leftChild = nullptr;
	int rank = 0;
	Node(int value)
	{
		this->value = value;
		this->rank = 0;
	}
};

struct RootCounterNode
{
	int value = 0;
	Node* listPointer = nullptr;
};

class ThickHeap
{
	int size = 0;
	int maxRank;
	Node* minNode = nullptr;
	vector<RootCounterNode> rootCounter;

	// Вставляет дерево ранга k в список деревьев k-го элемента корневого счетчика
	void insertTreeInList(Node* tree)
	{
		if (tree->rank > maxRank || rootCounter[tree->rank].value == 3)
		{
			throw "insertTreeInList error";
		}

		tree->right = rootCounter[tree->rank].listPointer;
		rootCounter[tree->rank].listPointer = tree;
	}

	// Удаляет дерево ранга k из списка деревьев k-го элемента корневого счетчика
	void deleteTreeFromList(Node* tree)
	{
		// Гарантируется, что такое дерево существует в списке
		int rank = tree->rank;
		if (rank > maxRank)
		{
			throw "deleteTreeFromList error";
		}
		Node* currentPointer = rootCounter[rank].listPointer;
		if (currentPointer == tree)
		{
			rootCounter[rank].listPointer = currentPointer->right;
			tree->right = nullptr;
			return;
		}
		while (currentPointer->right != tree)
		{
			currentPointer = currentPointer->right;
		}
		currentPointer->right = tree->right;
		tree->right = nullptr;
	}

	// Возвращает указатель на минималный узел списочной части корневого счетчика
	Node* getMinFromList(Node* listPointer)
	{
		Node* curp = listPointer;
		Node* minp = listPointer;
		while (curp != nullptr)
		{
			if (curp->value < minp->value)
			{
				minp = curp;
			}
			curp = curp->right;
		}
		return minp;
	}

	// Связывает три толстых дерева ранга k в одно дерево ранга k + 1
	Node* fastening(Node* p1, Node* p2, Node* p3)
	{
		Node* minp;
		// Если узел явлеятся minNode, он должен быть в корне
		bool foundmin = false;
		if (p1->rank == minNode->rank)
		{
			if (p1 == minNode)
			{
				minp = p1;
				p1 = p3;
				foundmin = true;
			}
			else if (p2 == minNode)
			{
				minp = p2;
				p2 = p3;
				foundmin = true;
			}
			else if (p3 == minNode)
			{
				minp = p3;
				foundmin = true;
			}
		}

		if (!foundmin)
		{
			if (p1->value <= p2->value && p1->value <= p3->value)
			{
				minp = p1;
				p1 = p3;
			}
			else if (p2->value <= p3->value && p2->value <= p1->value)
			{
				minp = p2;
				p2 = p3;
			}
			else if (p3->value <= p2->value && p3->value <= p1->value)
			{
				minp = p3;
			}
		}

		p1->left = nullptr;
		p1->right = p2;
		p1->parent = minp;
		p2->left = p1;
		p2->right = minp->leftChild;
		p2->parent = minp;
		if (minp->leftChild != nullptr)
		{
			minp->leftChild->left = p2;
		}
		minp->leftChild = p1;
		minp->left = nullptr;
		minp->right = nullptr;
		minp->parent = nullptr;
		minp->rank++;
		return minp;
	}

	// Связывает 3 дерева ранга k в одно дерево ранга k + 1, поддерживая корневой счетчик [операция фиксации]
	void fixRootNode(int rank)
	{
		if (rank > maxRank || rootCounter[rank].value != 3)
		{
			throw "fixRootNode error";
		}

		rootCounter[rank].value = 0;
		Node* p1 = rootCounter[rank].listPointer;
		Node* p2 = p1->right;
		Node* p3 = p2->right;
		p1->right = p2->right = nullptr;
		Node* newp = fastening(p1, p2, p3);
		rootCounter[rank].listPointer = nullptr;
		insertTree(newp);
	}

	// Вставка дерева в кучу
	void insertTree(Node* tree)
	{
		int rank = tree->rank;
		if (rank > maxRank + 1)
		{
			throw "insertTree error";
		}

		if (rank == maxRank + 1)
		{
			increaseMaxRank();
		}

		// Если вставить не можем, освобождаем место
		if (rootCounter[rank].value == 3)
		{
			fixRootNode(rank);
		}

		insertTreeInList(tree);
		rootCounter[rank].value++;
	}

	// Удаление дерева из кучи
	void deleteTree(Node* tree)
	{
		if (tree->rank > maxRank || rootCounter[tree->rank].value == 0)
		{
			throw "deleteTree error";
		}

		deleteTreeFromList(tree);
		rootCounter[tree->rank].value--;
	}

	// Возвращает указатель на минимальный узел среди корневых узлов
	Node* minNodeInRoot()
	{
		Node* minp = nullptr;
		for (int i = 0; i <= maxRank; ++i)
		{
			Node* p1 = getMinFromList(rootCounter[i].listPointer);
			if (getKey(p1) < getKey(minp))
			{
				minp = p1;
			}
		}
		return minp;
	}

	// Увеличивает значение maxRank, добавляя узлы в соотв. счетчики
	void increaseMaxRank()
	{
		maxRank++;
		rootCounter.push_back(RootCounterNode());
	}

	// Возвращает значение узла, либо +inf, если узел null
	int getKey(Node* p)
	{
		if (p == nullptr)
		{
			return INF;
		}
		else
		{
			return p->value;
		}
	}

public:
	ThickHeap()
	{
		maxRank = -1;
	}

	void insert(int value)
	{
		size++;
		Node* newNode = new Node(value);
		if (getKey(newNode) < getKey(minNode))
		{
			minNode = newNode;
		}
		this->insertTree(newNode);
	}

	int getMin()
	{
		if (minNode == nullptr)
		{
			throw "Error. Heap is empty.";
		}
		return minNode->value;
	}

	int deleteMin()
	{
		if (minNode == nullptr)
		{
			throw "Error. Heap is empty.";
		}
		size--;
		deleteTree(minNode);
		Node* cur = minNode->leftChild;
		while (cur != nullptr)
		{
			Node* temp = cur;
			cur = cur->right;
			temp->left = nullptr;
			temp->right = nullptr;
			temp->parent = nullptr;
			insertTree(temp);
		}
		int minValue = minNode->value;
		minNode = minNodeInRoot();
		return minValue;
	}

	int getSize()
	{
		return size;
	}

	bool isEmpty()
	{
		return this->size == 0;
	}
};

bool parseInteger(string argument, int minValue, int maxValue, int &value)
{
    try
    {
        size_t bad;
        value = stoi(argument, &bad);
        if (bad != argument.size() || value > maxValue || value < minValue)
        {
            return false;
        }
    }
    catch (exception e)
    {
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    std::ofstream lout("heapLogs.txt", std::ios::app);

    int currentTest = atoi(argv[3]);
    ThickHeap heap;
    int operationsCount;
    int number;
    string operation;
    string argument;

    fin >> argument; 
    if (parseInteger(argument, 0, maximumOperationsCount, operationsCount) == false)
    {
        fout << "Error: Incorrect operation count\n";
        lout << "CRITICAL error in testcase " << currentTest << " on line 1: ";
        lout << "Incorrect operation count\n";
        fin.close();
        fout.close();
        lout.close();
        return 0;
    }

    for (int index = 0; index < operationsCount; ++index)
    {
        if (fin.eof())
        {
            fout << "Error: Operation expected\n";
            lout << "WARNING error in testcase " << currentTest << " on line ";
            lout << index + 2 << ": Operation expected\n";
            break;
        }

        fin >> operation;

        if (operation == "insert")
        {
            fin >> argument;
            if (parseInteger(argument, -maximumValue, maximumValue, number) == false)
            {
                fout << "Error: Incorrect argument\n";
                lout << "WARNING error in testcase " << currentTest << " on line ";
                lout << index + 2 << ": Incorrect argument\n";
                continue;
            }
            heap.insert(number);
        }
        else if (operation == "getMin")
        {
            if (heap.isEmpty())
            {
                fout << "Error: Heap is empty\n";
                lout << "WARNING error in testcase " << currentTest << " on line ";
                lout << index + 2 << ": Heap is empty\n";
                continue; 
            }
            fout << heap.getMin() << "\n";
        }
        else if (operation == "deleteMin")
        {
            if (heap.isEmpty())
            {
                fout << "Error: Heap is empty\n";
                lout << "WARNING error in testcase " << currentTest << " on line ";
                lout << index + 2 << ": Heap is empty\n";
                continue;
            }
            fout << heap.deleteMin() << "\n";
        }
        else if (operation == "size")
        {
            fout << heap.getSize() << "\n";
        }
        else
        {
            fout << "Error: Incorrect operation\n";
            lout << "WARNING error in testcase " << currentTest << " on line ";
            lout << index + 2 << ": Incorrect operation\n";
            continue;
        }
    }

    fin.close();
    fout.close();
    lout.close();

    return 0;
}