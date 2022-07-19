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
        << "ms"
        << '\n';
  }

 private:
  std::chrono::time_point<ClockType> _startTime;
};

int main() {
  lab7::Matrix m1(10, 100'000, [](size_t i, size_t j) { return float(i + j); });
  lab7::Matrix identity(100'000, 8,[](size_t i, size_t j) { return float(i == j); });

  {
    ScopedObservation observation;
    m1.multiply(identity, std::execution::seq);
  }

  {
    ScopedObservation observation;
    m1.multiply(identity, std::execution::par_unseq);
  }
}
