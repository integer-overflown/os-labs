#include "matrix.h"

#include <Windows.h>

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <stdexcept>

namespace lab7 {

namespace {

struct ThreadExecutionParams {
  MatrixRowIterator row;
  MatrixColumnIterator column;
  size_t length;
  float *result;
};

DWORD GetNumberOfProcessors() {
  SYSTEM_INFO systemInfo;
  GetSystemInfo(&systemInfo);
  return systemInfo.dwNumberOfProcessors;
}

DWORD WINAPI MatrixMultiplicationThreadRoutine(LPVOID lpThreadParameter) {
  const auto params = static_cast<ThreadExecutionParams *>(lpThreadParameter);
  for (size_t i = 0; i < params->length; ++i) {
    *params->result += (*params->row) * (*params->column);
    ++params->row;
    ++params->column;
  }
  return 0;
}

constexpr size_t BoundedAdd(size_t value, size_t max,
                            size_t term = 1) noexcept {
  return (value < max ? value : 0) + term;
}

constexpr void BoundedIncrement(size_t &value, size_t max,
                                size_t term = 1) noexcept {
  value = BoundedAdd(value, max, term);
}

[[noreturn]] void HandleFatalWinApiError(const char *reason) {
  std::cerr << reason << ", error code " << GetLastError() << '\n';
  std::terminate();
}

}  // namespace

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
  if (cols() != other.rows()) {
    throw std::invalid_argument("matrices are of invalid dimensions");
  }

  const size_t length = cols();
  const size_t numberOfThreads = GetNumberOfProcessors();
  std::vector<ThreadExecutionParams> threadParams;
  threadParams.reserve(numberOfThreads);
  std::vector<HANDLE> threadHandles;
  threadHandles.reserve(numberOfThreads);

  size_t selfRow = 0;
  size_t otherCol = 0;
  size_t numRunningThreads = 0;

  Matrix result(rows(), other.cols());

  for (size_t i = 0; i < numberOfThreads; ++i) {
    threadParams.push_back({MatrixRowIterator{*this, selfRow},
                            MatrixColumnIterator{other, otherCol}, length,
                            &result(selfRow, otherCol)});
    HANDLE threadHandle =
        CreateThread(nullptr,  // default security attrs
                     0,        // default stack size
                     MatrixMultiplicationThreadRoutine,  // thread function
                     &threadParams[i],                   // function parameter
                     CREATE_SUSPENDED,  // create, but don't run yet
                     nullptr            // out ptr to thread id, ignored
        );

    if (threadHandle == INVALID_HANDLE_VALUE) {
      HandleFatalWinApiError("CreateThread failed");
    }

    threadHandles.push_back(threadHandle);

    if (otherCol == other.cols() - 1) {
      if (selfRow == rows() - 1) {
        break;
      }
      ++selfRow;
      otherCol = 0;
    } else {
      ++otherCol;
    }
  }

  std::for_each(threadHandles.begin(), threadHandles.end(), ResumeThread);
  numRunningThreads = threadHandles.size();

  for (;;) {
    DWORD waitStatus = WaitForMultipleObjects(
        threadHandles.size(), threadHandles.data(), false, INFINITE);

    if (waitStatus == WAIT_FAILED) {
      HandleFatalWinApiError("WaitForMultipleObjects failed");
    }

    --numRunningThreads;

    assert(WAIT_OBJECT_0 <= waitStatus && waitStatus < threadHandles.size());
    size_t finishedThreadIdx = waitStatus - WAIT_OBJECT_0;

    if (otherCol == other.cols() - 1) {
      if (selfRow == rows() - 1) {
        if (numRunningThreads > 0) {
          continue;
        } else {
          break;
        }
      } else {
        ++selfRow;
        otherCol = 0;
      }
    } else {
      ++otherCol;
    }

    assert(threadParams.size() == numberOfThreads);
    CloseHandle(threadHandles[finishedThreadIdx]);
    threadParams[finishedThreadIdx] = {MatrixRowIterator{*this, selfRow},
                                       MatrixColumnIterator{other, otherCol},
                                       length, &result(selfRow, otherCol)};

    threadHandles[finishedThreadIdx] =
        CreateThread(nullptr,  // default security attrs
                     0,        // default stack size
                     MatrixMultiplicationThreadRoutine,  // thread function
                     &threadParams[finishedThreadIdx],   // function parameter
                     0,                                  // run immediately
                     nullptr  // out ptr to thread id, ignored
        );
  }

  std::for_each(threadHandles.begin(), threadHandles.end(), CloseHandle);

  return result;
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

std::pair<MatrixRowIterator, MatrixRowIterator> Matrix::rowEntries(
    size_t index) const {
  return {{*this, index}, {*this, index, cols()}};
}

std::pair<MatrixColumnIterator, MatrixColumnIterator> Matrix::columnEntries(
    size_t index) const {
  return {{*this, index}, {*this, index, rows()}};
}

}  // namespace lab7
