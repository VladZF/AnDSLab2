# Лабораторная работа №2 по курсу "Алгоритмы и структуры данных 2"

## Введение
Данная лабораторная работа направлена на решение определенной задачи тремя разными алгоритмами. При этом необходимо измерить время каждого алгоритма: отдельно препроцессинг, если он есть, и ответ. Выяснить, какой алгоритм эффективнее на разных объемах входных данных.
> [!IMPORTANT]
> Если алгоритм включает в себя стадию препроцессинга, то требуется вычислить время как подготовки, так и получения ответа отдельно.

## Постановка задачи
Даны прямоугольники на плоскости с углами в целочисленных координатах. Требуется как можно быстрее выдавать ответ на вопрос «Скольким прямоугольникам принадлежит точка (x,y)?» И подготовка данных должна занимать мало времени.

## Подробнее про алгоритмы
> [!NOTE]
> n - количество прямоугольников.

### Наивный алгоритм
Не требующий препроцессинга и легкий в реализации алгоритм грубой силы. Для каждого прямоугольника устанавливаем факт владения или не владения каждой точкой. При фиксированном количестве точек алгоритм работает за ```O(n)```.

### Алгоритм сжатия координат и построение карты.
>[!NOTE]
>В следующих двух алгоритмах будут использоваться массивы xValues и yValues - уникальные значения x и y соответственно в порядке возрастания, а также хэш-мапы xIndex и yIndex, которые будут сопоставлять значения x и y с их индексами в массивах xValues и yValues.

```c++
std::set<int> xSet, ySet;  
std::unordered_map<int, int> xIndex, yIndex;  
for (auto rectangle: rectangles) {  
    xSet.insert(rectangle.start.x);  
    xSet.insert(rectangle.end.x);  
    ySet.insert(rectangle.start.y);  
    ySet.insert(rectangle.end.y);  
}  
xValues = std::vector<int>(xSet.begin(), xSet.end());  
yValues = std::vector<int>(ySet.begin(), ySet.end());  
auto map = std::vector<std::vector<int>>(yValues.size(), std::vector<int>(xValues.size()));  
for (int i = 0; i < xValues.size(); ++i)  
    xIndex[xValues[i]] = i;  
for (int i = 0; i < yValues.size(); ++i)  
    yIndex[yValues[i]] = i;
```

Более сложный в исполнении и уже требующий подготовки данных. Сначала нужно получить уникальные значения координат прямоугольников в отсортированном порядке (отдельно для x и y). Далее для каждого прямоугольника по значениям координат находим индексы на карте и заполняем по площади, прибавляя по единице.
```c++
for (auto rectangle: rectangles) {  
    int sx = xIndex[rectangle.start.x];  
    int sy = yIndex[rectangle.start.y];  
    int ex = xIndex[rectangle.end.x];  
    int ey = yIndex[rectangle.end.y];  
    for (int i = sy; i < ey; ++i)  
        for (int j = sx; j < ex; ++j)  
            map[i][j]++;
```
> [!IMPORTANT]
> Нельзя взаимодействовать с теми ячейками карты, которые в себе содержат хотя бы одно значение координаты конца прямоугольника, так как по условию задачи если точка лежит на верхней или правой границе прямоугольника, то она не пренадлежит ему.

В итоге алгоритм в себе имеет три основных цикла, соответственно, имеет сложность ```O(n^3)```

При поиске ответа мы ищем бинарным поиском индексы на карте и по ним получаем значение для точки. Время работы составит ```O(log(n))```, так как всю работу по поиску делает бинарный поиск.

### Персистентное дерево отрезков
Самый сложный алгоритм, но самый эффективный. При подготовки данных требуется построить персистентное дерево отрезков. Для этого использована специальная структура Event, которая является имитацией сканирующей прямой.
```c++
struct Event {  
    int x;  
    int startY;  
    int endY;  
    int state;  
  
    Event() = default;  
  
    Event(int x, int sy, int ey, int s) : x(x), startY(sy), endY(ey), state(s) {}  
};
```
- x - координата по x, которая имитирует положение сканирующей прямой, идущей параллельно оси абсцисс.
- startY и endY - координаты по y, которые указывают на отрезок изменения Д.О.
- state - 1: начало прямоугольника; -1: конец прямоугольника

Алгоритм построения дерева занимает ```O(n*log(n))``` времени.

Поиск ответа осуществляется через доступ к определенной версии дерева через координату x, и поиск нужного пути, в дереве, используя координату y. Время поиска также занимает ```O(log(n))``` времени.

## Время работы алгоритмов
> [!NOTE]
> Количество прямоугольников в тестах равняется ```2^i```, где 1 <= i <= 10.
> Количество точек во всех тестах равняется 1000.

### Таблица с временами

|                 |                  |                          |        |             |                               |        |             |
| --------------- | ---------------- | ------------------------ | ------ | ----------- | ----------------------------- | ------ | ----------- |
| Кол-во прямоуг. | Наивный алгоритм | Сжатие координат и карта |        |             | Персистентное дерево отрезков |        |             |
|                 |                  | Препроцессинг            | Ответ  | Общее время | Препроцессинг                 | Ответ  | Общее время |
| 1               | 14301            | 5344                     | 37398  | 42742       | 7169                          | 41083  | 48252       |
| 2               | 21892            | 7479                     | 50187  | 57666       | 10820                         | 59299  | 70119       |
| 4               | 35195            | 13378                    | 63780  | 77158       | 18539                         | 79805  | 98344       |
| 8               | 63189            | 29660                    | 77577  | 107237      | 44360                         | 95314  | 139674      |
| 16              | 117459           | 57666                    | 79745  | 137411      | 94049                         | 122879 | 216928      |
| 32              | 226426           | 142689                   | 90055  | 232744      | 154658                        | 119858 | 274516      |
| 64              | 444993           | 406024                   | 121622 | 527646      | 322030                        | 138671 | 460701      |
| 128             | 885461           | 1849105                  | 155201 | 2004306     | 634639                        | 190720 | 825359      |
| 256             | 1723484          | 14902282                 | 162841 | 15065123    | 1303606                       | 207902 | 1511508     |
| 512             | 3406417          | 116527087                | 174736 | 116701823   | 3379426                       | 226879 | 3606305     |
| 1024            | 6826610          | 1227469087               | 194466 | 1227663553  | 5567250                       | 258441 | 5825691     |

## Общее время работы алгоритмов

![all_times](https://github.com/VladZF/AnDSLab2/blob/master/Graphs/all_times.png)

Наихудший вариант показывает алгоритм на карте из-за затратного по времени препроцессинга. Лучше всего на маленьких данных показывает себя наивный алгоритм, но это с учетом отсутствия препроцессинга, на больших данных он начинает идти чуть хуже персистентного Д.О.

## Время выдачи ответов трех алгоритмов

![answer_times](https://github.com/VladZF/AnDSLab2/blob/master/Graphs/answer_times.png)

Как и в предыдущем случае, лучше себя показывает на маленьких данных наивный алгоритм. Но потом начинает существенно проигрывать Д.О и карте. Причем, время ответа у карты немного лучше, чем у Д.О. Все потому, что при поиске карта использует 2 раза бинарный поиск и потом обращается к ячейке матрицы за ```O(1)```, тогда как в Д.О необходимо пройтись еще по самому дереву, что увеличивает константу у сложности (которая не учитывается в асимптотике).

## Время препроцессинга карты и Д.О.

![prepare_times](https://github.com/VladZF/AnDSLab2/blob/master/Graphs/prepare_times.png)

Карта на маленьких данных совсем немного опережает построение Д.О. Это происходит из-за большего числа структур и выделения памяти. Но из-за большой разницы в сложности, построение карты на большом объеме данных начинает занимать колоссально больше времени чем препроцессинг алгоритма на дереве.

## Выводы
При оценивании работы алгоритмов можно выделить следующие особенности
### Наивный алгоритм
- Прост в исполнении
- На маленьких данных показывает себя лучше всех остальных алгоритмов
- На больших данных очень затратный по выдаче ответа из-за отсутствия препроцессинга
### Сжатие координат и построение карты
- Из всех алгоритмов самый быстрый в выдаче ответа
- Среднее количество выделяемой памяти
- Затратный препроцессинг
### Персистентное Д.О.
- Эффективные по времени препроцессинг и выдача ответа
- Довольно ресурсозатратный алгоритм из-за большого выделения памяти и использования самого большого количества структур
- По времени работы и на маленьких и на больших данных работает эффективно, что делает его самым эффективным алгоритмом из представленных
- Довольно сложен в реализации, особенно на C/C++ из-за большого количества указателей.

### Итог
Все алгоритмы имеют свои преимущества и недостатки, если расположить в их в порядке увеличения эффективности и сложности реализации, то список будет такой:
```Наивный алгоритм -> Построение карты и сжатие координат -> персистентное дерево отрезков```
