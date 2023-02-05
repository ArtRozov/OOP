#include <iostream>
#include <cstring>
#include <math.h>

int to_integer(char* input) {
  int number = 0;
  for (size_t i = 0; i < strlen(input); ++i) {
    number += (input[i] - '0') * pow(10, strlen(input) - i - 1);
  }
  return number;
}

int main(int argc, char* argv[]) {
  long long answer = 0;
  int* numbers_of_arrays = new int[argc];
  int max_len = 0;
  for (int i = 0; i < argc - 1; ++i) {
    numbers_of_arrays[i] = to_integer(argv[i + 1]);
    max_len = std::max(max_len, numbers_of_arrays[i]);
    std::cerr << numbers_of_arrays[i] << '\n';
  }
  int len_of_arrays = argc - 1;
  int** arrays = new int* [len_of_arrays];
  for (int i = 0; i < len_of_arrays; ++i) {
    arrays[i] = new int[numbers_of_arrays[i]];
    for (int j = 0; j < numbers_of_arrays[i]; ++j) {
      int a;
      std::cin >> a;
      arrays[i][j] = a;
      std::cerr << arrays[i][j] << ' ';
    }
    std::cerr << '\n';
  }
  int* indexes = new int[len_of_arrays];
  for (int i = 0; i < len_of_arrays; ++i) { indexes[i] = 0; }
  if (!(max_len < len_of_arrays)) {
    while (indexes[0] < numbers_of_arrays[0]) {
      bool exists = true;
      for (int i = 0; i < len_of_arrays - 1; ++i) {
        for (int j = i + 1; j < len_of_arrays; ++j) {
          if (indexes[i] == indexes[j]) {
            exists = false;
            break;
          }
        }
        if (!exists) { break; }
      }
      if (exists) {
        long long pr = 1;
        for (int i = 0; i < len_of_arrays; ++i) { pr *= arrays[i][indexes[i]]; }
        answer += pr;
      }
      indexes[len_of_arrays - 1] += 1;
      int index = len_of_arrays - 1;
      while (index > 0 && indexes[index] == numbers_of_arrays[index]) {
        indexes[index] = 0;
        --index;
        indexes[index] += 1;
      }
    }
  }
  std::cout << answer;
  for (int i = 0; i < len_of_arrays; ++i) { delete[] arrays[i]; }
  delete[] arrays;
  delete[] indexes;
  delete[] numbers_of_arrays;
}