#include <chrono>
#include <iostream>

#include "matrix.h"

class ScopedObservation {
  using ClockType = std::chrono::high_resolution_clock;

 public:
  ScopedObservation() : _startTime(ClockType::now()) {}

  ~ScopedObservation() {
    using namespace std::chrono;
    std::cout
        << duration_cast<milliseconds>(ClockType::now() - _startTime).count()
        << "ms" << '\n';
  }

 private:
  std::chrono::time_point<ClockType> _startTime;
};

int main() {
  lab7::Matrix m1(10, 100'000, [](size_t i, size_t j) { return float(i + j); });
  lab7::Matrix identity(100'000, 8,
                        [](size_t i, size_t j) { return float(i == j); });

  lab7::Matrix smallMatrix1(
      8, 8, [](size_t i, size_t j) { return float(2 * i + j); });
  lab7::Matrix smallMatrix2(8, 8,
                            [](size_t i, size_t j) { return float(int(i) - int(j)); });

  std::cout << "smallMatrix1\n" << smallMatrix1 << '\n';
  std::cout << "smallMatrix2\n" << smallMatrix2 << '\n';
  std::cout << "smallMatrix1 x smallMatrix2 (sequential)\n" << smallMatrix1.multiply(smallMatrix2, std::execution::seq) << '\n';

  {
    std::cout << "Large multiplication (sequential):\n";
    ScopedObservation observation;
    m1.multiply(identity, std::execution::seq);
  }

  {
    std::cout << "Large multiplication (parallel):\n";
    ScopedObservation observation;
    m1.multiply(identity, std::execution::par_unseq);
  }
}
