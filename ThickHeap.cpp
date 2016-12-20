#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

const int INF = 1e9 + 100;

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

void runManualTesting()
{
	ThickHeap heap;

	cout << "Welcome to Thick Heap data structure testing application.\n";
	cout << "The next commands are supported:\n";
	cout << "\"insert A\" - Insert number A to the Thick Heap. (A must be an integer between -10^9 and 10^9 inclusively).\n";
	cout << "\"getMin\" - Print the minimum number being in the Thick Heap.\n";
	cout << "\"deleteMin\" - Print and delete the minimum number being in the Thick Heap.\n";
	cout << "\"size\" - Print the number of elements being in the Thick Heap.\n";
	cout << "\"exit\" - Finish testing and close this application.\n";

	string command;
	while (cin >> command)
	{
		if (command == "insert")
		{
			string number;
			cin >> number;
			int a;
			int maximum = 1000000000;
			try
			{
				size_t bad;
				a = stoi(number, &bad);
				if (bad != number.size() || a > maximum || a < -maximum)
				{
					cout << "Incorrect argument. \"A\" must be an integer between -10^9 and 10^9 inclusively.\n";
					continue;
				}
			}
			catch (exception e)
			{
				cout << "Incorrect argument. \"A\" must be an integer between -10^9 and 10^9 inclusively.\n";
				continue;
			}

			try
			{
				heap.insert(a);
				cout << "Element " << a << " has been successfully inserted to the heap.\n";
			}
			catch (char* error)
			{
				cout << error << "\n";
			}
		}
		else if (command == "getMin")
		{
			if (heap.getSize() == 0)
			{
				cout << "Error. Heap is empty.\n";
				continue;
			}
			int minimum;
			try
			{
				minimum = heap.getMin();
				cout << "The minimum element in the heap: " << minimum << "\n";
			}
			catch (char* error)
			{
				cout << error << "\n";
			}
		}
		else if (command == "deleteMin")
		{
			if (heap.getSize() == 0)
			{
				cout << "Error. Heap is empty.\n";
				continue;
			}
			int deleted;
			try
			{
				deleted = heap.deleteMin();
				cout << "Element " << deleted << " has been successfully deleted from the heap.\n";
			}
			catch (char* error)
			{
				cout << error << "\n";
			}
		}
		else if (command == "size")
		{
			cout << "The number of elements in the heap: " << heap.getSize() << "\n";
		}
		else if (command == "exit")
		{
			cout << "Bye!";
			return;
		}
		else
		{
			cout << "Unknown command. Try again.\n";
		}
	}
}

void runAutomaticTesting()
{
	ThickHeap heap;
	string inputfilename, outputfilename;
	cout << "Enter name of the input file: ";
	cin >> inputfilename;
	cout << "Enter name of the output file: ";
	cin >> outputfilename;
	cout << endl;
	ifstream fin(inputfilename);
	ofstream fout(outputfilename);
	if (!fin.is_open())
	{
		cout << "Error while opening file. Note that input file must be in the same directory as this application.\n";
		system("pause");
		return;
	}

	// Считываем количество запросов
	int qNumber;
	string qnum;
	fin >> qnum;
	int maxQNum = 1000000;
	try
	{
		size_t bad;
		qNumber = stoi(qnum, &bad);
		if (bad != qnum.size() || qNumber > maxQNum || qNumber < 1)
		{
			cout << "Error in line 1: ";
			cout << "Queries number must be an integer between 1 and 10^7 inclusively.\n";
			fin.close();
			fout.close();
			system("pause");
			return;
		}
	}
	catch (exception e)
	{
		cout << "Error in line 1: ";
		cout << "Queries number must be an integer between 1 and 10^7 inclusively.\n";
		fin.close();
		fout.close();
		system("pause");
		return;
	}

	string command;
	for (int i = 0; i < qNumber; ++i)
	{
		if (fin.eof())
		{
			cout << "Error in line " << i + 2 << ": command not found.\n";
			fin.close();
			fout.close();
			system("pause");
			return;
		}
		fin >> command;
		if (command == "insert")
		{
			string number;
			fin >> number;
			int a;
			int maximum = 1000000000;
			try
			{
				size_t bad;
				a = stoi(number, &bad);
				if (bad != number.size() || a > maximum || a < -maximum)
				{
					cout << "Error in line " << i + 2 << ": ";
					cout << "Incorrect argument. \"A\" must be an integer between -10^9 and 10^9 inclusively.\n";
					fin.close();
					fout.close();
					system("pause");
					return;
				}
			}
			catch (exception e)
			{
				cout << "Error in line " << i + 2 << ": ";
				cout << "Incorrect argument. \"A\" must be an integer between -10^9 and 10^9 inclusively.\n";
				fin.close();
				fout.close();
				system("pause");
				return;
			}

			try
			{
				heap.insert(a);
			}
			catch (char* error)
			{
				cout << "Error in line " << i + 2 << ": ";
				cout << error << "\n";
				fin.close();
				fout.close();
				system("pause");
				return;
			}
		}
		else if (command == "getMin")
		{
			if (heap.getSize() == 0)
			{
				cout << "Error in line " << i + 2 << ": ";
				cout << "Heap is empty.\n";
				fin.close();
				fout.close();
				system("pause");
				return;
			}
			int minimum;
			try
			{
				minimum = heap.getMin();
				fout << minimum << "\n";
			}
			catch (char* error)
			{
				cout << "Error in line " << i + 2 << ": ";
				cout << error << "\n";
				fin.close();
				fout.close();
				system("pause");
				return;
			}
		}
		else if (command == "deleteMin")
		{
			if (heap.getSize() == 0)
			{
				cout << "Error in line " << i + 2 << ": ";
				cout << "Heap is empty.\n";
				fin.close();
				fout.close();
				system("pause");
				return;
			}
			int deleted;
			try
			{
				deleted = heap.deleteMin();
				fout << deleted << "\n";
			}
			catch (char* error)
			{
				cout << "Error in line " << i + 2 << ": ";
				cout << error << "\n";
				fin.close();
				fout.close();
				system("pause");
				return;
			}
		}
		else if (command == "size")
		{
			fout << heap.getSize() << "\n";
		}
		else
		{
			cout << "Error in line " << i + 2 << ": ";
			cout << "Unknown command. \n";
			fin.close();
			fout.close();
			system("pause");
			return;
		}
	}
	fin.close();
	fout.close();
	cout << "The testing successfully finished. Answer has been written in output file.\n";
	system("pause");
	return;
}

int main()
{
	//runManualTesting();
	//runAutomaticTesting();
	return 0;
}