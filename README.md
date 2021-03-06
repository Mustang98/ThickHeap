# ThickHeap
На данный момент реализована алгоритмическая часть проекта на С++. <br>
Позже планируется добавить визуализацию алгоритма, используя JavaScript + HTML + CSS.

<h2>Описание файлов проекта:</h2>
Исходный код алгоритма находится в файле <b>ThickHeap.cpp</b><br>
Для тестирования правильности работы алгоритма в репозитории присутствуют 2 файла:
<ul>
<li><b>ThickHeapManualTestingApp.exe</b> - Консольная утилита для ручного тестирования при помощи ввода с клавиатуры.</li>
<li><b>ThickHeapAutomaticTestingApp.exe</b> - Консольная утилита для автоматического тестирования с помощью файлов (для больших входных данных).</li>
</ul>
В папке <b>test</b> находятся 3 файла, которые могут быть использованы для тестирования (они описаны ниже).

<h2>Формат входных и выходных данных и ограничения:</h2>
Формат и доступные команды для ручного тестирования подробно описаны в самой утилите.<br>
<br>
Для автоматического тестирования в папке с утилитой необходимо разместить файл следующего формата:<br>
В первой строке файла должно быть число N (0 <= N <= 1000000) - количество команд.<br>
В следующих N строках должны находиться команды:<br>
<code>insert A</code> - добавить число А в кучу. (А должно быть целым числом от -10^9 до 10^9 включительно).<br>
<code>getMin</code> - вывести в выходной файл минимальный элемент, находящийся в куче. (Куча не должна быть пустой!)<br>
<code>deleteMin</code> - вывести в выходной файл и удалить из кучи минимальный элемент. (Куча не должна быть пустой!)<br>
<code>size</code> - вывести в выходной файл количество элементов в куче.<br>
В случае корректных запросов во входном файле программа выведет необходимые данные в выходной файл. В противном случае в консоль выведется ошибка и номер строки входного файла, в котором она возникла.<br>
<br>
В папке <b>test</b> находятся 3 файла с тестами для автоматического тестирования:
<ul>
<li><b>test1.txt</b> - Добавление 333333 элементов в кучу с параллельным выводом её размера. После все элементы удаляются.</li>
<li><b>test2.txt</b> - Чередование операций добавления, вывода размера и удаления элемента 300000 раз.</li>
<li><b>test3.txt</b> - Добавление 100000 случайных элементов в кучу с последующим удалением минимальных. В результате добавленные элементы должны быть выведены в отсортированном порядке.</li>
</ul>
