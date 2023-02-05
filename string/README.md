# String
| Type of restrictions | Specification |
| ------------- | ------------- |
| Time limit | 1 second |
| Memory limit | 64Mb |
| Input | Standard input or input.txt |
| Output | standard output or output.txt |

The String class is a simplified analogue of the library std::string. It is necessary to support the following functionality:

- Constructor from the C-style string (const char*).
- Constructor from two parameters: the number n and the symbol c - creates a string of n copies of the symbol c.
- Default constructor, copying constructor and assignment operator.
- The == operator, which allows checking strings for a match.
- Square brackets that work correctly for both constant and non-constant strings.
- The length() method, which returns the size of the string.
- push_back(char) and pop_back() methods. The running time should be O(1).
- Methods front() and back(), which return a reference to the initial and last characters of the string, respectively.
- The += operator, which allows you to add a character or another string to a string. Adding a symbol should work for amortized O(1).
- The + operator, which allows you to add strings with symbols, characters with strings and strings with strings.
- The find(substring) and rfind(substring) methods, which allow you to find the leftmost and rightmost occurrence of the substring substring in this string.
  Returns the index of the occurrence found. If no occurrences are found, you need to return length() (dummy index).
- The substr(start, count) method, which returns a substring starting from the start index and consisting of count characters.
- The empty() method, which checks whether the string is empty. The clear() method, which allows you to clear a string.
- Operators of output to the stream " and input from the stream ".
