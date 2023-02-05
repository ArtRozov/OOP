# C. BigInteger+Rational
| Тип ограничений  | Конкретизация |
| ------------- | ------------- |
| Ограничение времени | 5 секунд  |
| Ограничение памяти  | 64Mb  |
| Ввод  | 	стандартный ввод или input.txt  |
| Вывод  | 	стандартный вывод или output.txt  |

This is a BigInteger class for working with long integers. Supported operations:

- addition, subtraction, multiplication, division, modulo remainder, working the same way as for int;
- compound assignment with these operations. Division should work no longer than in O(n2).
- unary minus, prefix and postfix increment and decrement. Prefix increment and decrement should work for O(1) on average.
- comparison operators == != < > <= >=
- output to the stream and input from the stream
- the toString() method, which returns a string representation of a number
- construction from int (including implicit conversion when needed)
- implicit conversion to bool when needed (should work in conditional expressions)
- optional - literal suffix bi for writing literals of the BigInteger type, see help here https://en.cppreference.com/w/cpp/language/user_literal

Using the BigInteger class, write a Rational class to work with rational numbers of arbitrarily high precision.
Rational numbers should be represented as irreducible ordinary fractions, where the numerator and denominator are arbitrarily long integers.

Operations must be supported:
- constructor from BigInteger, from int
- addition, subtraction, multiplication, division, compound assignment with these operations
- unary minus
- comparison operators == != < > <= >=
- the toString() method, which returns a string representation of a number (of the form [minus]numerator/denominator), where the numerator and denominator are mutually prime numbers; if the number is actually an integer, then the denominator does not need to be output
- the asDecimal(sizet precision=0) method, which returns a string representation of a number in the form of a decimal fraction with precision decimal places
- the operator of conversion to double
