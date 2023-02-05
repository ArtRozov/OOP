#include <iostream>
#include <string>
#include <vector>
#include <cassert>

class BigInteger {
 private:
  std::vector<long long> big_int = {};
  bool neg_sign = 0;

  bool bigger(const BigInteger& number1, const BigInteger& number2, long long index) const { //function for operator/=
    for (int i = index + number2.big_int.size() - 1; i >= index; --i) {
      if (number2.big_int[i - index] > number1.big_int[i]) { return true; }
      else if (number2.big_int[i - index] < number1.big_int[i]) { return false; }
    }
    return false;
  }

  void nulls_in_the_end() {
    while ((big_int.size() > 1) && (big_int[big_int.size() - 1] == 0)) { big_int.pop_back(); }
  }
 public:
  const long long base = 1000000000;
  //for Rational

  int get_sign() const { return neg_sign; }

  long long num_size() const { return big_int.size(); }

  void change_sign() {
    neg_sign = !neg_sign;
    null_sign();
  }

  void null_sign() {
    if ((big_int.size() == 1) and (big_int[0] == 0)) { neg_sign = 0; }
  }

  bool is_null() const {
    return (big_int.size() == 1) && (big_int[0] == 0);
  }

  void decrease(BigInteger& number1, const BigInteger& number2, size_t index) { //void for operator/=
    for (size_t i = index; i < index + number2.big_int.size(); ++i) {
      number1.big_int[i] -= number2.big_int[i - index];
      if (number1.big_int[i] < 0) {
        number1.big_int[i] += base;
        number1.big_int[i + 1] -= 1;
      }
    }
  }

  BigInteger() {
    big_int = {0};
    neg_sign = 0;
  }

  BigInteger(long long number) {
    big_int = {};
    if (number < 0) {
      neg_sign = 1;
      number *= (-1);
    }
    while (number > 0) {
      big_int.push_back(number % base);
      number /= base;
    }
    if (big_int.empty()) { big_int = {0}; }
    null_sign();
  }

  BigInteger(const BigInteger& number) {
    big_int = number.big_int;
    neg_sign = number.neg_sign;
  }

  BigInteger(std::string number) {
    long long index = 0;
    big_int = {};
    if (number[0] == '-') {
      neg_sign = 1;
      index = 1;
    }
    big_int.resize((number.length() - index) / 9 + 1);
    long long pow10 = 1;
    for (long long i = number.length() - 1; i >= index; --i) {
      if ((number.length() - i - 1) % 9 == 0) { pow10 = 1; }
      big_int[(number.length() - i - 1) / 9] += pow10 * (int(number[i]) - int('0'));
      pow10 *= 10;
    }
    nulls_in_the_end();
    null_sign();
  }

  explicit operator bool() const { return !((big_int.size() == 1) && (big_int[0] == 0)); }

  BigInteger& operator=(const BigInteger& number) {
    big_int = number.big_int;
    neg_sign = number.neg_sign;
    return *this;
  }

  BigInteger operator-() const {
    BigInteger result = *this;
    result.change_sign();
    result.null_sign();
    return result;
  }

  bool operator==(const BigInteger& number) {
    if ((big_int.size() != number.big_int.size()) || (neg_sign != number.neg_sign)) {
      return false;
    } else {
      for (size_t i = 0; i < big_int.size(); ++i) {
        if (big_int[i] != number.big_int[i]) { return false; }
      }
      return true;
    }
  }

  bool operator>(const BigInteger& number) {
    if (neg_sign != number.neg_sign) { return number.neg_sign; }
    if (neg_sign == 0) {
      if (big_int.size() > number.big_int.size()) { return true; }
      else if (big_int.size() < number.big_int.size()) { return false; }
      else {
        for (int i = big_int.size() - 1; i >= 0; --i) {
          if (big_int[i] > number.big_int[i]) {
            return true;
          } else if (big_int[i] < number.big_int[i]) {
            return false;
          }
        }
      }
    } else {
      if (big_int.size() < number.big_int.size()) { return true; }
      else if (big_int.size() > number.big_int.size()) { return false; }
      else {
        for (int i = big_int.size() - 1; i >= 0; --i) {
          if (big_int[i] < number.big_int[i]) { return true; }
          else if (big_int[i] > number.big_int[i]) { return false; }
        }
      }
    }
    return false;
  }

  bool operator<(const BigInteger& number) {
    if (*this > number) {
      return false;
    } else if (*this == number) {
      return false;
    }
    return true;
  }

  bool operator>=(const BigInteger& number) {
    return (*this > number) || (*this == number);
  }

  bool operator<=(const BigInteger& number) {
    return (*this < number) || (*this == number);
  }

  bool operator!=(const BigInteger& number) {
    return !(*this == number);
  }

  BigInteger& operator++() {
    if (neg_sign) {
      --big_int[0];
      if ((big_int[0] == 0) && (big_int.size() == 1)) {
        neg_sign = 0;
      }
      size_t i = 0;
      while ((i + 1 < big_int.size()) && (big_int[i] < 0)) {
        big_int[i + 1] -= 1;
        big_int[i] += base;
        ++i;
      }
      nulls_in_the_end();
    } else {
      ++big_int[0];
      size_t i = 0;
      while ((i + 1 < big_int.size()) && (big_int[i] >= base)) {
        big_int[i + 1] += 1;
        big_int[i] %= base;
        ++i;
      }
      if (big_int[big_int.size() - 1] >= base) {
        big_int[big_int.size() - 1] %= base;
        big_int.push_back(1);
      }
    }
    null_sign();
    return *this;
  }

  BigInteger& operator--() {
    change_sign();
    ++(*this);
    change_sign();
    null_sign();
    return *this;
  }

  BigInteger operator++(int) {
    BigInteger copy = *this;
    ++(*this);
    return copy;
  }

  BigInteger operator--(int) {
    BigInteger copy = *this;
    --(*this);
    return copy;
  }

  BigInteger& operator+=(const BigInteger& number) {
    if (big_int.size() < number.big_int.size()) {
      big_int.resize(number.big_int.size());
    }
    if (neg_sign == number.neg_sign) {
      for (int i = number.big_int.size() - 1; i >= 0; --i) {
        big_int[i] += number.big_int[i];
      }
      for (size_t i = 0; i < big_int.size(); ++i) {
        if (big_int[i] >= base) {
          if ((i + 1) >= big_int.size()) {
            big_int.push_back(0);
          }
          big_int[i + 1] += big_int[i] / base;
          big_int[i] %= base;
        }
      }
    } else {
      for (int i = number.big_int.size() - 1; i >= 0; --i) {
        big_int[i] -= number.big_int[i];
      }
      for (int i = big_int.size() - 1; i >= 0; --i) {
        if (big_int[i] < 0) {
          for (size_t j = 0; j < big_int.size(); ++j) {
            big_int[j] *= -1;
          }
          neg_sign = 1 - neg_sign;
          break;
        } else if (big_int[i] > 0) {
          break;
        }
      }
      for (size_t i = 0; i < big_int.size() - 1; ++i) {
        if (big_int[i] < 0) {
          big_int[i] += base;
          big_int[i + 1] -= 1;
        }
      }
      nulls_in_the_end();
    }
    null_sign();
    return *this;
  }

  BigInteger& operator-=(const BigInteger& number) {
    neg_sign = !neg_sign;
    *this += number;
    neg_sign = !neg_sign;
    null_sign();
    return *this;
  }

  BigInteger& operator*=(const BigInteger& number) {
    if ((number.big_int.size() == 1) && (number.big_int[0] == 0)) {
      big_int.resize(1);
      big_int[0] = 0;
      return *this;
    }
    if (neg_sign == number.neg_sign) { neg_sign = 0; }
    else { neg_sign = 1; }
    if (number.big_int.size() > 1) {
      BigInteger copy = *this;
      big_int = std::vector<long long>(big_int.size() + number.big_int.size() + 1);
      for (int i = number.big_int.size() - 1; i >= 0; --i) {
        for (size_t j = 0; j < copy.big_int.size(); ++j) {
          big_int[i + j] += copy.big_int[j] * number.big_int[i];
        }
        for (size_t j = 0; j < big_int.size(); ++j) {
          if (big_int[j] >= base) {
            big_int[j + 1] += big_int[j] / base;
            big_int[j] %= base;
          }
        }
      }
    } else {
      big_int.resize(big_int.size() + 1);
      for (size_t i = 0; i < big_int.size() - 1; ++i) {
        big_int[i] *= number.big_int[0];
      }
      for (size_t i = 0; i < big_int.size() - 1; ++i) {
        if (big_int[i] >= base) {
          big_int[i + 1] += big_int[i] / base;
          big_int[i] %= base;
        }
      }
    }
    nulls_in_the_end();
    null_sign();
    return *this;
  }


  BigInteger& operator/=(const BigInteger& number) {
    BigInteger ans;
    if (neg_sign == number.neg_sign) {
      ans.neg_sign = 0;
    } else {
      ans.neg_sign = 1;
    }
    neg_sign = 0;
    if (number.big_int.size() > big_int.size()) {
      ans.big_int = {0};
      ans.neg_sign = 0;
      *this = ans;
      return *this;
    }
    long long index_of_last = big_int.size() - 1;
    size_t s = big_int.size() - number.big_int.size();
    for (long long i = s; i >= 0; --i) {
      int left = 0;
      int right = base;
      BigInteger copy;
      int median = (right + left) / 2;
      while (right - left > 1) {
        median = (right + left) / 2;
        copy = number;
        copy *= median;
        if (copy.big_int.size() > number.big_int.size()) {
          copy.big_int[copy.big_int.size() - 2] += copy.big_int[copy.big_int.size() - 1] * base;
          copy.big_int.pop_back();
        }
        if ((copy.big_int.size() > big_int.size() - i) || bigger(*this, copy, i)) { //copy > this
          right = median;
        } else { left = median; }
      }
      if (left != 0) {
        copy = number;
        copy *= left;
        if (copy.big_int.size() > number.big_int.size()) {
          copy.big_int[copy.big_int.size() - 2] += copy.big_int[copy.big_int.size() - 1] * base;
          copy.big_int.pop_back();
        }
        decrease(*this, copy, i);
      }
      ans.big_int.push_back(left);
      if (index_of_last > 0) {
        big_int[index_of_last - 1] += base * big_int[index_of_last];
        big_int.pop_back();
        index_of_last = big_int.size() - 1;
      }
    }
    for (size_t i = 0; i < ans.big_int.size() / 2; ++i) {
      std::swap(ans.big_int[i], ans.big_int[ans.big_int.size() - 1 - i]);
    }
    ans.nulls_in_the_end();
    ans.null_sign();
    *this = ans;
    return *this;
  }

  BigInteger& operator%=(const BigInteger& number) {
    BigInteger copy = *this;
    copy /= number;
    copy *= number;
    *this -= copy;
    null_sign();
    return *this;
  }

  std::string toString() const {
    std::string s = "";
    BigInteger copy = *this;
    if (neg_sign == 1) { s += "-"; }
    long long pow10 = 1;
    while (pow10 <= big_int[big_int.size() - 1]) {
      pow10 *= 10;
    }
    pow10 /= 10;
    while (pow10 > 0) {
      s += char((copy.big_int[copy.big_int.size() - 1] / pow10) % 10 + int('0'));
      pow10 /= 10;
    }
    for (int i = copy.big_int.size() - 2; i >= 0; --i) {
      pow10 = base;
      pow10 /= 10;
      while (pow10 > 0) {
        s += char((copy.big_int[i] / pow10) % 10 + int('0'));
        pow10 /= 10;
      }
    }
    if (s == "") { s = '0'; }
    return s;
  }

  friend std::istream& operator>>(std::istream&, BigInteger&);

  friend std::ostream& operator<<(std::ostream&, const BigInteger&);

  friend BigInteger& NOD(BigInteger& number1, BigInteger& number2);
};

BigInteger operator+(const BigInteger& num1, const BigInteger& num2) {
  BigInteger copy = num1;
  copy += num2;
  return copy;
}

BigInteger operator-(const BigInteger& num1, const BigInteger& num2) {
  BigInteger copy = num1;
  copy -= num2;
  return copy;
}

BigInteger operator*(const BigInteger& num1, const BigInteger& num2) {
  BigInteger copy = num1;
  copy *= num2;
  return copy;
}

BigInteger operator/(const BigInteger& num1, const BigInteger& num2) {
  BigInteger copy = num1;
  copy /= num2;
  return copy;
}

BigInteger operator%(const BigInteger& num1, const BigInteger& num2) {
  BigInteger copy = num1;
  copy %= num2;
  return copy;
}

std::istream& operator>>(std::istream& in, BigInteger& number) {
  std::string s;
  in >> s;
  number = BigInteger(s);
  return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& ans) {
  std::string s = ans.toString();
  out << s;
  return out;
}

BigInteger& NOD(BigInteger& number1, BigInteger& number2) {
  while (!((number2.big_int.size() == 1) && (number2.big_int[0] == 0))) {
    number1 %= number2;
    BigInteger copy = number2;
    number2 = number1;
    number1 = copy;
    number2.nulls_in_the_end();
  }
  return number1;
}

class Rational {
 private:
  BigInteger numerator;
  BigInteger denominator;
  bool neg_sign = 0;
 public:

  void reduction_of_fractions() {
    if (!numerator.is_null()) {
      BigInteger copy1 = numerator;
      BigInteger copy2 = denominator;
      BigInteger nod = NOD(copy1, copy2);
      numerator /= nod;
      denominator /= nod;
    }
  }

  Rational() {
    numerator = BigInteger(0);
    denominator = BigInteger(1);
    neg_sign = 0;
  }

  Rational(const long long number) {
    numerator = BigInteger(number);
    denominator = BigInteger(1);
    if (numerator.get_sign() == 1) {
      neg_sign = !neg_sign;
      numerator.change_sign();
    }
  }

  Rational(const long long number, const long long number1) {
    numerator = BigInteger(number);
    denominator = BigInteger(number1);
    if (numerator.get_sign() == 1) {
      neg_sign = !neg_sign;
      numerator.change_sign();
    }
    if (denominator.get_sign() == 1) {
      neg_sign = !neg_sign;
      denominator.change_sign();
    }
    reduction_of_fractions();
  }

  Rational(const BigInteger& number) {
    numerator = number;
    if (numerator.get_sign() == 1) {
      neg_sign = !neg_sign;
      numerator.change_sign();
    }
    denominator = BigInteger(1);
  }

  Rational(const BigInteger& number, const BigInteger& number1) {
    numerator = number;
    if (numerator.get_sign() == 1) {
      neg_sign = !neg_sign;
      numerator.change_sign();
    }
    denominator = number1;
    if (denominator.get_sign() == 1) {
      neg_sign = !neg_sign;
      denominator.change_sign();
    }
    reduction_of_fractions();
  }

  bool operator==(const Rational& number) {
    return (numerator * number.denominator == denominator * number.numerator) &&
           (neg_sign == number.neg_sign);
  }

  bool operator!=(const Rational& number) { return !(*this == number); }

  bool operator<(const Rational& number) {
    return (neg_sign > number.neg_sign) ||
           (numerator * number.denominator < denominator * number.numerator);
  }

  bool operator<=(const Rational& number) { return *this < number || *this == number; }

  bool operator>(const Rational& number) { return !(*this <= number); }

  bool operator>=(const Rational& number) { return !(*this < number); }

  Rational operator-() const {
    Rational copy = *this;
    copy.neg_sign = !neg_sign;
    return copy;
  }

  Rational& operator+=(const Rational& number) {
    Rational copy = number;
    copy.numerator *= denominator;
    numerator *= copy.denominator;
    if (neg_sign == copy.neg_sign) { numerator += copy.numerator; }
    else { numerator -= copy.numerator; }
    if (numerator.get_sign() == 1) {
      numerator.change_sign();
      neg_sign = !neg_sign;
    }
    denominator *= copy.denominator;
    if (numerator.is_null()) { neg_sign = 0; }
    return *this;
  }

  Rational& operator-=(const Rational& number) {
    Rational copy = number;
    copy.numerator *= denominator;
    numerator *= number.denominator;
    denominator *= number.denominator;
    if (neg_sign == copy.neg_sign) { numerator -= copy.numerator; }
    else { numerator += copy.numerator; }
    if (numerator.get_sign() == 1) {
      numerator.change_sign();
      neg_sign = !neg_sign;
    }
    if (numerator.is_null()) { neg_sign = 0; }
    return *this;
  }

  Rational& operator*=(const Rational& number) {
    numerator *= number.numerator;
    denominator *= number.denominator;
    if (neg_sign == number.neg_sign) { neg_sign = 0; }
    else { neg_sign = 1; }
    if (numerator.is_null()) { neg_sign = 0; }
    return *this;
  }

  Rational& operator/=(const Rational& number) {
    numerator *= number.denominator;
    denominator *= number.numerator;
    if (neg_sign == number.neg_sign) { neg_sign = 0; }
    else { neg_sign = 1; }
    if (numerator.is_null()) { neg_sign = 0; }
    return *this;
  }

  std::string toString() {
    std::string s;
    reduction_of_fractions();
    if (numerator.is_null()) { neg_sign = 0; }
    if (neg_sign == 1) { s += '-'; }
    s += numerator.toString();
    if (!((denominator == 1) || (numerator == 0))) {
      s += '/' + denominator.toString();
    }
    return s;
  }

  std::string asDecimal(long long precision) {
    std::string s;
    reduction_of_fractions();
    if (numerator.is_null()) { neg_sign = 0; }
    if (neg_sign == 1) { s += '-'; }
    BigInteger result = numerator / denominator;
    BigInteger remainder_of_division = numerator % denominator;
    s += result.toString() + '.';
    for (long long i = 0; i < precision; ++i) {
      if (remainder_of_division != 0) {
        remainder_of_division *= 10;
        result = remainder_of_division % denominator;
        remainder_of_division /= denominator;
      }
      s += remainder_of_division.toString();
      remainder_of_division = result;
    }
    return s;
  }

  explicit operator double() { return atof(asDecimal(20).c_str()); }

};

Rational operator+(const Rational& number1, const Rational& number2) {
  Rational result = number1;
  result += number2;
  return result;
}

Rational operator-(const Rational& number1, const Rational& number2) {
  Rational result = number1;
  result -= number2;
  return result;
}

Rational operator*(const Rational& number1, const Rational& number2) {
  Rational result = number1;
  result *= number2;
  return result;
}

Rational operator/(const Rational& number1, const Rational& number2) {
  Rational result = number1;
  result /= number2;
  return result;
}