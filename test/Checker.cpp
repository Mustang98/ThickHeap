#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

// Возвращает затраченное время в мс
int runSolution(std::string filePath, std::string input, std::string output)
{
    std::string fullPath = filePath + " " + input + " " + output;
    clock_t start = clock();
    system(fullPath.c_str());
    clock_t time = clock() - start;
    float sectime = (float)time / CLOCKS_PER_SEC;
    int mstime = (int)(sectime * (float)1000);
    return mstime;
}

bool compareFiles(std::string heapFileName, std::string dumbFileName, int &errorLine)
{
    std::ifstream heapStream(heapFileName);
    std::ifstream dumbStream(dumbFileName);
    int line = 1;
    std::string heapString, dumbString;
    while (!dumbStream.eof())
    {
        if (heapStream.eof())
        {
            errorLine = line;
            return false;
        }
        std::getline(dumbStream, dumbString);
        std::getline(heapStream, heapString);
        if (dumbString != heapString)
        {
            errorLine = line;
            return false;
        }
        line++;
    }

    if (!heapStream.eof())
    {
        errorLine = line;
        return false;
    }

    return true;
}

int main()
{
    int testCount, msTime, errorLine;
    bool compResult;
    std::cout << "Insert test count: ";
    std::cin >> testCount;
    std::string dumbPath = "DumbSolution.exe";
    std::string heapPath = "HeapSolution.exe";
    for (int test = 1; test <= testCount; ++test)
    {
        std::string testString = std::to_string(test);
        std::string input("input" + testString + ".txt");
        std::string heapOutput("heapOutput" + testString + ".txt");
        std::string dumbOutput("dumbOutput" + testString + ".txt");
        std::cout << "========== TEST #" << test << "==========\n";
        std::cout << "Solving via HeapSolution: ";
        msTime = runSolution(heapPath, input, heapOutput);
        std::cout << "OK " << msTime << "ms\n";
        std::cout << "Solving via DumbSolution: ";
        msTime = runSolution(dumbPath, input, dumbOutput);
        std::cout << "OK " << msTime << "ms\n";
        std::cout << "Comparing result: ";
        compResult = compareFiles(heapOutput, dumbOutput, errorLine);
        if (compResult)
        {
            std::cout << "OK output files are equal\n\n";
        }
        else
        {
            std::cout << "WA at line " << errorLine << "\n\n";
        }
    }
    system("pause");
    return 0;
}

