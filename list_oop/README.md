# List and StackAllocator
| Type of restrictions | Specification |
| ------------- | ------------- |
| Time limit | 10 seconds |
| Memory limit | 512Mb |
| Input | Standard input or input.txt |
| Output | standard output or output.txt |

## Part 1.

The StackAllocator<typename T, size_t N> class was written, which allows you to create standard containers on the stack, without a single access to dynamic memory.\
For this, the StackStorage<size_t N> class was written, which stores a large array on the stack.

The StackAllocator class meets the allocator requirements described on the page https://en.cppreference.com/w/cpp/named_req/Allocator \
It is STL-compatible, that is, it allows use as an allocator for standard containers.\
In particular, defined:

- Default constructor, copy constructor, destructor, assignment operator;
- Allocate, deallocate methods;
- Internal value_type type;
- The select_on_container_copy_construction method, if the logic of your allocator requires it.

An example of how StackAllocator can be used:
```c++
int main() {
  StackStorage<100'000> storage;
  StackAllocator<int, 100'000> alloc(storage);
  std::vector<int, StackAllocator<int, 100'000» v(alloc);
  // ... useful stuff ...
}
```
During execution, there may be multiple stackstorages with the same N.\
Of course, allocators built on different stackstorages should be considered unequal.

StackAllocator takes care of the alignment of objects.\
In particular, it does not put variables of type int at addresses that are not multiples of 4; variables of type double - at addresses that are not multiples of 8, etc..

Using any container with StackAllocator instead of std::allocator leads to the fact that there are no calls to dynamic memory in the program at all.

## Part 2.
The List class is a doubly linked list with the correct use of the allocator (meets the requirements https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer )\
List has two template parameters: T is the type of elements in the sheet, Allocator is the type of allocator used (by default - std::allocator<T>).

Supported:
- Constructors: without parameters; from one number; from a number and const T&; from one allocator; from a number and an allocator; from a number, const T& and an allocator.
- The get_allocator() method, which returns the allocator object currently used in the sheet;
- Copy constructor, destructor, copying assignment operator;
- Size() method, working for O(1);
- push_back, push_front, pop_back, pop_front methods;
- Bidirectional iterators that meet the requirements https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator .
Also support constant and reverse iterators;
- Methods begin, end, cbegin, cend, rbegin, rend, crbegin, crend;
- Insert(iterator, const T&) and erase(iterator) methods - for deleting and adding single elements to the list.

All methods of the sheet are strictly safe with respect to exceptions.\
This means that when an exception is thrown from any method of class T during an arbitrary operation X on a sheet, the sheet returns to the state it was before X started, preventing UB and memory leaks, and throws the exception up to the calling function.
We believe that constructors and assignment operators for allocators never throw exceptions (this is part of the requirements for Allocator).
