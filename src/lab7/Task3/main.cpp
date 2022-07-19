#include <iostream>

#include "matrix.h"

int main() {
  lab7::Matrix m1(6, 10, [](size_t i, size_t j) { return float(i + j); });
  lab7::Matrix identity(10, 1, [](size_t i, size_t j) { return float(i == j); });

  std::cout << "m1:\n" << m1 << '\n';
  std::cout << "identity:\n" << identity << '\n';

  std::cout << "result:\n" << m1.multiply(identity, std::execution::par_unseq);
}
