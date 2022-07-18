#include <execution>
#include <iomanip>
#include <iostream>
#include <stdexcept>
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

Matrix::Matrix(size_t rows, size_t cols, EntryGenerator entryGenerator) {
  if (rows == 0 || cols == 0) {
    throw std::invalid_argument("Matrix dimensions must be non-zero");
  }

  _matrix.resize(rows);

  if (entryGenerator) {
    std::generate(_matrix.begin(), _matrix.end(),
                  [=, rowNo = size_t(0)]() mutable -> std::vector<float> {
                    std::vector<float> row(cols);
                    std::generate(row.begin(), row.end(),
                                  [=, colNo = size_t(0)]() mutable -> float {
                                    return entryGenerator(rowNo, colNo++);
                                  });
                    ++rowNo;
                    return row;
                  });
  } else {
    std::generate(_matrix.begin(), _matrix.end(),
                  [cols]() { return std::vector<float>(cols); });
  }
}

size_t Matrix::rows() const { return _matrix.size(); }

size_t Matrix::cols() const { return _matrix[0].size(); }

float Matrix::operator()(size_t row, size_t col) const {
  return _matrix[row][col];
}

float &Matrix::operator()(size_t row, size_t col) { return _matrix[row][col]; }

Matrix Matrix::multiply(const Matrix &other, std::execution::sequenced_policy) {
  if (cols() != other.rows()) {
    throw std::invalid_argument("matrices are of invalid dimensions");
  }

  Matrix result(rows(), other.cols());

  for (size_t i = 0; i < rows(); ++i) {
    for (size_t j = 0; j < other.cols(); ++j) {
      for (size_t k = 0; k < cols(); ++k) {
        result(i, j) += (*this)(i, k) * other(k, j);
      }
    }
  }

  return result;
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
