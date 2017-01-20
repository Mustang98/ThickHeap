#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

/*
ИСПОЛЬЗОВАННЫЕ СРЕДСТВА STL:
1. Функция std::min(int, int) - возвращает минимальное из двух полученных аргументов;
2. Контейнер std::vector<int> - динамический массив целочисленных значений;
3. Метод std::vector<int>::push_back(int) - метод добавления числа в конец массива (справа);
4. Метод std::vector<int>::size() - метод получения количества чисел в динамическом массиве;
5. Тип std::vector<int>::iterator - указатель (итератор) на конкретный элемент дин. массива;
6. Метод std::vector<int>::begin() - метод получения указателя (итератора) на 1й элемент дин. массива;
7. Метод std::vector<int>::iterator::operator+ (int number) - метод перемещения укзателя (итератора)
   на number позиций вправо;
8. Метод std::vector<int>::erase(std::vector<int>::iterator it) - метод удаления из контейнера
   элемента, на который указывает указатель it с последующим сдвигом элементов справа на 1 ячейку
   влево.
9. Функция std::stoi(std::string str, size_t &bad) - функция парсинга целого числа из строки. Число
   парсится пока не будет достигнут конец строки или встречен некорректный символ (его индекс будет
   записан в переменную bad). Может выбрасывать исключения (например если число больше INT_MAX).
*/

const int maximumValue = 1000000000, maximumOperationsCount = 1000000;

// Функция парсинга целого числа с указанием интервала допустимых значений
// Если аргумент корректен, возвращается true, а значение записывается в out-переменную value,
// иначе возвращается false.
bool parseInteger(std::string argument, int minValue, int maxValue, int &value)
{
    try
    {
        size_t bad; // Индекс первого некорректного символа
        value = stoi(argument, &bad); // Пытаемся получить число
        // Если встретился некорректный символ или полученное значение выходит за границы
        if (bad != argument.size() || value > maxValue || value < minValue)
        {
            return false;
        }
    }
    // Исключение могло быть выброшено функцией stoi() при некорректном аргументе
    catch (std::exception e) 
    {
        return false;
    }
    return true;
}

// Первые 2 аргумента коммандной строки - имена входного и выходного файлов
int main(int argc, char* argv[])
{
    std::ifstream fin(argv[1]); // Открываем входной файл
    std::ofstream fout(argv[2]); // Открываем (или создаем) выходной файл
    
    std::vector<int> container; // Динамический массив, поддерживающий нужные элементы

    int operationsCount; // Количество операций
    int number; // Добавляемое в контейнер число
    std::string operation; // Название текущей операции
    std::string argument; // Аргумент операции (для последующего парсинаг с проверкой)

    fin >> argument; // Считываем количество операций
    // Пытаемся распарсить полученное значение. Если аргумент некорректный:
    if (parseInteger(argument, 0, maximumOperationsCount, operationsCount) == false)
    {
        fout << "Error: Incorrect operation count\n"; // То выводим сообщение об ошибке
        fin.close(); // Закрываем входной файл
        fout.close(); // Закрываем выходной файл
        return 0; // Завершаем работу программы
    }

    // Обрабатываем каждую операцию по очереди
    for (int index = 0; index < operationsCount; ++index)
    {
        if (fin.eof()) // Если операции закончились
        {
            fout << "Error: Operation expected\n"; // То выводим сообщение об ошибке
            break; // Завершаем раоту программы
        }

        fin >> operation; // Считываем очередную операцию

        if (operation == "insert") // Операция добавления числа в контейнер
        {
            fin >> argument; // Считываем аргумент
            // Пытаемся распарсить добавляемое значение. Если аргумент некорректный:
            if (parseInteger(argument, -maximumValue, maximumValue, number) == false) 
            {
                fout << "Error: Incorrect argument\n"; // То выводим сообщение об ошибке
                break; // Завершаем работу программы
            }
            container.push_back(number); // Добавляем число в контейнер (справа)
        }
        else if (operation == "getMin") // Операция получения минимального числа в контейнере
        {
            if (container.size() == 0) // Если в контейнере нет элементов, то:
            { 
                fout << "Error: Heap is empty\n"; // То выводим сообщение об ошибке
                break; // Завершаем работу программы
            }
            // Находим минимальное число в контейнере, а именно:
            int minNumber = container[0]; // Изначально полагаем, что искомое число на 0й позиции
            for (int currentNumber : container) // Потом просматриваем остальные числа
            {
                // И если рассматриваемое число меньше минимального, обновляем минимальное
                minNumber = std::min(minNumber, currentNumber);
            }
            fout << minNumber << "\n"; // Выводим найденное число
        }
        else if (operation == "deleteMin") // Операция удаления минимального элемента из контейнера
        {
            if (container.size() == 0) // Если в контейнере нет элементов, то:
            {
                fout << "Error: Heap is empty\n"; // То выводим сообщение об ошибке
                break; // Завершаем работу программы
            }
            // Находим индекс минимального числа, а именно
            int minIndex = 0; // Изначально полагаем, что искомое число на 0й позиции
            // Потом просматриваем остальные индексы
            for (int currentIndex = 1; currentIndex < container.size(); ++currentIndex)
            {
                // И если число по текущему индексу меньше минимального
                if (container[currentIndex] < container[minIndex])
                {
                    minIndex = currentIndex; // То изменяем индекс минимального числа на текущий
                }
            }
            fout << container[minIndex] << "\n"; // Выводим число по найденному индексу
            // Получаем указатель на минимальное число путем сдвига указателя на 0е число на
            // minIndex ячеек вправо
            std::vector<int>::iterator it = container.begin() + minIndex;
            container.erase(it); // Удаляем число по полученному указателю
        }
        else if (operation == "size") // Операция получения количества элементов в контейнере
        {
            // Выводим количество элементов в контейнере
            fout << container.size() << "\n";
        }
        else // Встретили некорректное название операции
        {
            fout << "Error: Incorrect operation\n"; // Выводим сообщение об ошибке
            break; // Завершаем работу программы
        }
    }

    fin.close(); // Закрываем входной файл
    fout.close(); // Закрываем выходной файл

    return 0;
}

