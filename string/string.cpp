#include <iostream>
#include <cstring>

class String {
 private:
  char* str = nullptr;
  size_t len_str = 0;
  size_t buffer = 0;
 public:
  String() = default;

  String(char c) : str(new char[1]), len_str(1), buffer(1) { *str = c; }

  String(const String& s) : str(new char[s.buffer]), len_str(s.len_str), buffer(s.buffer) {
    memcpy(str, s.str, len_str);
  }

  String(const char* s) : str(new char[strlen(s)]), len_str(strlen(s)), buffer(strlen(s)) {
    memcpy(str, s, len_str);
  }

  String(size_t n, char c) : str(new char[n]), len_str(n), buffer(n) {
    memset(str, c, n);
  }

  ~String() { delete[] str; }

  char& front() { return *str; }

  char& back() { return *(str + len_str - 1); }

  const char& front() const { return *str; }

  const char& back() const { return *(str + len_str - 1); }

  bool empty() const { return len_str == 0; }

  void clear() {
    if (len_str == 0) return;
    delete[] str;
    str = nullptr;
    len_str = 0;
    buffer = 0;
  }

  size_t length() const { return len_str; }

  void swap(String& s) {
    std::swap(str, s.str);
    std::swap(len_str, s.len_str);
    std::swap(buffer, s.buffer);
  }

  bool operator==(const String& s) {
    if (len_str != s.len_str) return false;
    for (size_t i = 0; i < len_str; ++i) {
      if (*(str + i) != *(s.str + i)) { return false; }
    }
    return true;
  }

  String& operator=(const String& s) {
    if (*this == s) return *this;
    if (len_str != 0) { clear(); }
    str = new char[s.len_str];
    len_str = s.len_str;
    buffer = len_str;
    memcpy(str, s.str, s.len_str);
    return *this;
  }

  char& operator[](size_t ind) { return *(str + ind); }

  const char& operator[](size_t ind) const { return *(str + ind); }

  void push_back(char c) {
    if (len_str == buffer) {
      if (buffer == 0) { buffer = 1; }
      else { buffer *= 2; }
      String copy;
      copy.str = new char[buffer];
      copy.len_str = len_str;
      copy.buffer = buffer;
      memcpy(copy.str, str, len_str);
      swap(copy);
    }
    len_str += 1;
    str[len_str - 1] = c;
  }

  void pop_back() {
    len_str -= 1;
  }

  String& operator+=(const char& c) {
    if (len_str == buffer) {
      if (buffer == 0) { buffer = 1; }
      else { buffer *= 2; }
      String copy;
      copy.str = new char[buffer];
      copy.len_str = len_str;
      copy.buffer = buffer;
      memcpy(copy.str, str, len_str);
      swap(copy);
    }
    len_str += 1;
    str[len_str - 1] = c;
    return *this;
  }

  String& operator+=(const String& s) {
    String copy = *this;
    str = new char[copy.len_str + s.len_str];
    len_str = copy.len_str + s.len_str;
    buffer = len_str;
    memcpy(str, copy.str, copy.len_str);
    memcpy(str + copy.len_str, s.str, s.len_str);
    return *this;
  }

  size_t find(const String& s) const {
    size_t index = 0;
    size_t index_s = 0;
    while (index < len_str and index_s < s.len_str) {
      if (str[index] == s.str[index_s]) { index_s += 1; }
      else { index_s = 0; }
      index += 1;
    }
    if (index_s == s.len_str) { return index - index_s; }
    else { return len_str; }
  }

  size_t rfind(const String& s) const {
    size_t index = len_str - 1;
    size_t index_s = s.len_str - 1;
    while (index > 0 and (index_s > 0 or str[index] != s.str[index_s])) {
      if (str[index] == s.str[index_s]) { index_s -= 1; }
      else { index_s = s.len_str - 1; }
      index -= 1;
    }
    if (index_s == 0 and str[index] == s.str[index_s]) { return index; }
    else { return len_str; }
  }

  String substr(size_t begin, size_t amount) {
    String copy;
    if (!(begin + amount - 1 > len_str)) {
      copy.str = new char[amount];
      copy.len_str = amount;
      copy.buffer = amount;
      memcpy(copy.str, str + begin, amount);
    }
    return copy;
  }

  String substr(size_t begin, size_t amount) const {
    String copy;
    if (begin + amount - 1 > len_str) { return copy; }
    copy.str = new char[amount];
    copy.len_str = amount;
    copy.buffer = amount;
    memcpy(copy.str, str + begin, amount);
    return copy;
  }

};

std::istream& operator>>(std::istream& in, String& s) {
  char c;
  c = in.get();
  s = String();
  while (std::isspace(c) != 0) c = in.get();
  while ((std::isspace(c) == 0) && !in.eof()) {
    s.push_back(c);
    c = in.get();
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const String& ans) {
  for (size_t i = 0; i < ans.length(); ++i) {
    out << ans[i];
  }
  return out;
}

String operator+(const String& s1, const String& s2) {
  String copy = s1;
  return copy += s2;
}

String operator+(const String& s, const char& c) {
  String copy = s;
  return copy += c;
}

String operator+(const char& c, const String& s) {
  String copy = c;
  return copy += s;
}