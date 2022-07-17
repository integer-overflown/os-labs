#include <execution>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace lab7 {

class Matrix {
 public:
  Matrix(size_t rows, size_t cols);
  [[nodiscard]] size_t rows() const;
  [[nodiscard]] size_t cols() const;
  float operator()(size_t row, size_t col) const;
  Matrix multiply(const Matrix &other, std::execution::sequenced_policy);
  Matrix multiply(const Matrix &other,
                  std::execution::parallel_unsequenced_policy);
  friend std::ostream &operator<<(std::ostream &out, const Matrix &m);

 private:
  std::vector<std::vector<float>> _matrix;
};

Matrix::Matrix(size_t rows, size_t cols) {
  if (rows == 0 || cols == 0) {
    throw std::invalid_argument("Matrix dimensions must be non-zero");
  }

  _matrix.resize(rows);
  std::generate(_matrix.begin(), _matrix.end(),
                [cols]() { return std::vector<float>(cols); });
}

size_t Matrix::rows() const { return _matrix.size(); }

size_t Matrix::cols() const { return _matrix[0].size(); }

float Matrix::operator()(size_t row, size_t col) const {
  return _matrix[row][col];
}

Matrix Matrix::multiply(const Matrix &other, std::execution::sequenced_policy) {
  return {0, 0};
}

Matrix Matrix::multiply(const Matrix &other,
                        std::execution::parallel_unsequenced_policy) {
  return {0, 0};
}

std::ostream &operator<<(std::ostream &out, const Matrix &m) {
  for (size_t i = 0; i < m.rows(); ++i) {
    for (size_t j = 0; j < m.cols(); ++j) {
      out << std::setw(5) << m(i, j) << ' ';
    }
    out << '\n';
  }
  return out;
}

}  // namespace lab7

int main() {
  lab7::Matrix m(10, 10);
  std::cout << m;
}
