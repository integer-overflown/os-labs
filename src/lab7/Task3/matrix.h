#ifndef OSLABS_MATRIX_H
#define OSLABS_MATRIX_H
#include <cstddef>
#include <execution>
#include <iostream>
#include <vector>

namespace lab7 {

class Matrix {
 public:
  using EntryGenerator = float (*)(size_t, size_t);
  Matrix(size_t rows, size_t cols, EntryGenerator entryGenerator = nullptr);
  [[nodiscard]] size_t rows() const;
  [[nodiscard]] size_t cols() const;
  float operator()(size_t row, size_t col) const;
  float &operator()(size_t row, size_t col);
  Matrix multiply(const Matrix &other, std::execution::sequenced_policy);
  Matrix multiply(const Matrix &other,
                  std::execution::parallel_unsequenced_policy);
  friend std::ostream &operator<<(std::ostream &out, const Matrix &m);

 private:
  std::vector<std::vector<float>> _matrix;
};

}  // namespace lab7

#endif  // OSLABS_MATRIX_H
