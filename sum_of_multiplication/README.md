# A. Сумма произведений

| Тип ограничений  | Конкретизация |
| ------------- | ------------- |
| Ограничение времени | 10 секунд  |
| Ограничение памяти  | 64Mb  |
| Ввод  | 	стандартный ввод или input.txt  |
| Вывод  | 	стандартный вывод или output.txt  |

Цель этой задачи — научиться работать с аргументами командной строки, а также с указателями и динамическим выделением памяти вручную.

В этой задаче нельзя пользоваться никакими классами и функциями из стандартной библиотеки, кроме ввода/вывода в поток (cin, cout).
Все выделения памяти надо делать вручную, использовать std::vector и ему подобные контейнеры нельзя.

Даны k массивов целых чисел, чьи размеры равны соответственно n[1], n[2],… , n[k]. Обозначим данные массивы за a[1],… , a[k].
Ваша задача — посчитать и вывести сумму произведений вида a1[i1]⋅…⋅ a[ik] по всевозможным кортежам попарно различных индексов (i[1],… , i[k]),
где каждый индекс лежит в соответствующих пределах.

Ваша программа должна использовать O(k+n[1]+… +n[k]) памяти. Выделять память «с запасом, чтоб точно хватило» нельзя.

## Формат ввода
Числа n[1],… , n[k] вводятся в виде параметров командной строки. После чего через стандартный поток ввода вводятся элементы массивов один за другим, то есть сначала все элементы первого массива, затем все элементы второго, и т.д.

Например, ваша программа может быть вызвана так: ./a.out 7 2 4 8 9

Это будет означать, что k = 5, а размеры массивов равны соответственно a[1] = 7,… , a[5] = 9.

## Формат вывода
Выведите единственно число — ответ на задачу.

## Примечания
Гарантируется, что все элементы массивов влезают в int, а ответ влезает в long long.\
Наличие в условии задачи 1-индексации не означает, что в вашей программе индексация должна идти с 1 :)\
Не пытайтесь придумать какой-то умный алгоритм, избежав перебора. Это NP-трудная задача (даже хуже, чем просто NP-трудная, но забейте).\
Цель — аккуратно и правильно написать перебор, корректно выделив и освободив все динамические массивы.