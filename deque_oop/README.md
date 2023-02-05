# Deque
| Тип ограничений  | Конкретизация |
| ------------- | ------------- |
| Ограничение времени | 2 секунды  |
| Ограничение памяти  | 64Mb  |
| Ввод  | 	стандартный ввод или input.txt  |
| Вывод  | 	стандартный вывод или output.txt  |

Deque has the following functionality:

- Constructors: by default, copy constructor, constructor from int, constructor from (int, const T&). The deque assignment operator.
- The size() method, which returns the current size of the container.
- Address by index: square brackets (without checks for going abroad) and at (throwing std::out_of_range). It should work for a guaranteed O(1).
- push_back, pop_back, push_front, pop_front methods. Must work for amortized O(1).
- There must be an internal iterator type (with a small letter, in the best traditions of STL). This type, in addition to the obvious, should support:
- Increment, decrement
- Addition with integers
- Comparison < > <= >= == !=
- Taking the difference of two iterators
- Dereference (unary asterisk), the result is T&
- Operator ->, the result is T*
- There must also be a constant iterator const_iterator. 
  Its difference from the usual iterator is that it does not allow you to change the element lying under it.
  Conversion (including implicit) of a non-constant iterator to a constant one is acceptable, but not back.
  It is necessary to implement a constant iterator so that its code is not a copy paste of the code of a regular iterator.
- The push_back, push_front, pop_back and pop_front operations should not invalidate pointers and references to the rest of the deck elements.
  The pop_back and pop_front operations, in addition, should also not invalidate iterators.
- Methods begin, cbegin, end and cend, which return non-constant and constant iterators to the beginning and to the “element after the end" of the container, respectively.
  If the container itself is constant, then the begin and end methods also return constant iterators.
  The end decrement should give an iterator on the last element, the subtraction of integers from the end should also work correctly and give iterators on the corresponding elements.
- reverse-iterators, as well as methods rbegin, rend, crbegin, crend.
- The insert(iterator, const T&) method, which inserts into the container by iterator.
  All elements on the right are shifted one to the right, the insertion works linear time.
- The erase(iterator) method, which removes an element from the container by iterator. 
  All elements on the right are shifted one to the left, the deletion works linear time.
  
All methods of your Deque are strictly safe with respect to exceptions.
