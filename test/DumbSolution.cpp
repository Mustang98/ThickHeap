#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

/*
�������������� �������� STL:
1. ������� std::min(int, int) - ���������� ����������� �� ���� ���������� ����������;
2. ��������� std::vector<int> - ������������ ������ ������������� ��������;
3. ����� std::vector<int>::push_back(int) - ����� ���������� ����� � ����� ������� (������);
4. ����� std::vector<int>::size() - ����� ��������� ���������� ����� � ������������ �������;
5. ��� std::vector<int>::iterator - ��������� (��������) �� ���������� ������� ���. �������;
6. ����� std::vector<int>::begin() - ����� ��������� ��������� (���������) �� 1� ������� ���. �������;
7. ����� std::vector<int>::iterator::operator+ (int number) - ����� ����������� �������� (���������)
   �� number ������� ������;
8. ����� std::vector<int>::erase(std::vector<int>::iterator it) - ����� �������� �� ����������
   ��������, �� ������� ��������� ��������� it � ����������� ������� ��������� ������ �� 1 ������
   �����.
9. ������� std::stoi(std::string str, size_t &bad) - ������� �������� ������ ����� �� ������. �����
   �������� ���� �� ����� ��������� ����� ������ ��� �������� ������������ ������ (��� ������ �����
   ������� � ���������� bad). ����� ����������� ���������� (�������� ���� ����� ������ INT_MAX).
*/

const int maximumValue = 1000000000, maximumOperationsCount = 1000000;

// ������� �������� ������ ����� � ��������� ��������� ���������� ��������
// ���� �������� ���������, ������������ true, � �������� ������������ � out-���������� value,
// ����� ������������ false.
bool parseInteger(std::string argument, int minValue, int maxValue, int &value)
{
    try
    {
        size_t bad; // ������ ������� ������������� �������
        value = stoi(argument, &bad); // �������� �������� �����
        // ���� ���������� ������������ ������ ��� ���������� �������� ������� �� �������
        if (bad != argument.size() || value > maxValue || value < minValue)
        {
            return false;
        }
    }
    // ���������� ����� ���� ��������� �������� stoi() ��� ������������ ���������
    catch (std::exception e) 
    {
        return false;
    }
    return true;
}

// ������ 2 ��������� ���������� ������ - ����� �������� � ��������� ������, 3� - ����� �����
int main(int argc, char* argv[])
{
    std::ifstream fin(argv[1]); // ��������� ������� ����
    std::ofstream fout(argv[2]); // ��������� (��� �������) �������� ����
    std::ofstream lout("dumbLogs.txt", std::ios::app); // ��������� ���� �����������
    
    std::vector<int> container; // ������������ ������, �������������� ������ ��������

    int currentTest = atoi(argv[3]); // �������� ����� �������� �����
    int operationsCount; // ���������� ��������
    int number; // ����������� � ��������� �����
    std::string operation; // �������� ������� ��������
    std::string argument; // �������� �������� (��� ������������ �������� � ���������)

    fin >> argument; // ��������� ���������� ��������
    // �������� ���������� ���������� ��������. ���� �������� ������������:
    if (parseInteger(argument, 0, maximumOperationsCount, operationsCount) == false)
    {
        fout << "Error: Incorrect operation count\n"; // �� ������� ��������� �� ������
        lout << "CRITICAL error in testcase " << currentTest << " on line 1: ";
        lout << "Incorrect operation count\n";
        fin.close(); // ��������� ������� ����
        fout.close(); // ��������� �������� ����
        lout.close(); // ��������� ���� �����
        return 0; // ��������� ������ ���������
    }

    // ������������ ������ �������� �� �������
    for (int index = 0; index < operationsCount; ++index)
    {
        if (fin.eof()) // ���� �������� �����������
        {
            fout << "Error: Operation expected\n"; // �� ������� ��������� �� ������
            lout << "WARNING error in testcase " << currentTest << " on line ";
            lout << index + 2 << ": Operation expected\n";
            break; // ��������� ������ ���������
        }

        fin >> operation; // ��������� ��������� ��������

        if (operation == "insert") // �������� ���������� ����� � ���������
        {
            fin >> argument; // ��������� ��������
            // �������� ���������� ����������� ��������. ���� �������� ������������:
            if (parseInteger(argument, -maximumValue, maximumValue, number) == false) 
            {
                fout << "Error: Incorrect argument\n"; // �� ������� ��������� �� ������
                lout << "WARNING error in testcase " << currentTest << " on line ";
                lout << index + 2 << ": Incorrect argument\n";
                continue; // ��������� � ��������� ��������
            }
            container.push_back(number); // ��������� ����� � ��������� (������)
        }
        else if (operation == "getMin") // �������� ��������� ������������ ����� � ����������
        {
            if (container.size() == 0) // ���� � ���������� ��� ���������, ��:
            { 
                fout << "Error: Heap is empty\n"; // �� ������� ��������� �� ������
                lout << "WARNING error in testcase " << currentTest << " on line ";
                lout << index + 2 << ": Heap is empty\n";
                continue; // ��������� � ��������� ��������
            }
            // ������� ����������� ����� � ����������, � ������:
            int minNumber = container[0]; // ���������� ��������, ��� ������� ����� �� 0� �������
            for (int currentNumber : container) // ����� ������������� ��������� �����
            {
                // � ���� ��������������� ����� ������ ������������, ��������� �����������
                minNumber = std::min(minNumber, currentNumber);
            }
            fout << minNumber << "\n"; // ������� ��������� �����
        }
        else if (operation == "deleteMin") // �������� �������� ������������ �������� �� ����������
        {
            if (container.size() == 0) // ���� � ���������� ��� ���������, ��:
            {
                fout << "Error: Heap is empty\n"; // �� ������� ��������� �� ������
                lout << "WARNING error in testcase " << currentTest << " on line ";
                lout << index + 2 << ": Heap is empty\n";
                continue; // ��������� � ��������� ��������
            }
            // ������� ������ ������������ �����, � ������
            int minIndex = 0; // ���������� ��������, ��� ������� ����� �� 0� �������
            // ����� ������������� ��������� �������
            for (int currentIndex = 1; currentIndex < container.size(); ++currentIndex)
            {
                // � ���� ����� �� �������� ������� ������ ������������
                if (container[currentIndex] < container[minIndex])
                {
                    minIndex = currentIndex; // �� �������� ������ ������������ ����� �� �������
                }
            }
            fout << container[minIndex] << "\n"; // ������� ����� �� ���������� �������
            // �������� ��������� �� ����������� ����� ����� ������ ��������� �� 0� ����� ��
            // minIndex ����� ������
            std::vector<int>::iterator it = container.begin() + minIndex;
            container.erase(it); // ������� ����� �� ����������� ���������
        }
        else if (operation == "size") // �������� ��������� ���������� ��������� � ����������
        {
            // ������� ���������� ��������� � ����������
            fout << container.size() << "\n";
        }
        else // ��������� ������������ �������� ��������
        {
            fout << "Error: Incorrect operation\n"; // ������� ��������� �� ������
            lout << "WARNING error in testcase " << currentTest << " on line ";
            lout << index + 2 << ": Incorrect operation\n";
            continue; // ��������� � ��������� ��������
        }
    }

    fin.close(); // ��������� ������� ����
    fout.close(); // ��������� �������� ����
    lout.close(); // ��������� ���� �����
    return 0;
}

