#include "rsa.h"

#include <cmath>
#include <iostream>
#include <random>

namespace lab2::rsa {

namespace {

uint16_t gPrimesTable[] = {
    2,    3,    5,    7,    11,   13,   17,   19,   23,   29,   31,   37,
    41,   43,   47,   53,   59,   61,   67,   71,   73,   79,   83,   89,
    97,   101,  103,  107,  109,  113,  127,  131,  137,  139,  149,  151,
    157,  163,  167,  173,  179,  181,  191,  193,  197,  199,  211,  223,
    227,  229,  233,  239,  241,  251,  257,  263,  269,  271,  277,  281};

class RandomPrimeSelector {
 public:
  RandomPrimeSelector()
      : _randomGenerator(std::random_device{}()),
        _distribution(0, std::size(gPrimesTable)) {}

  // cannot be 'const': _distribution::operator()() is mutating
  [[nodiscard]] uint16_t get() {
    return gPrimesTable[_distribution(_randomGenerator)];
  }

 private:
  std::mt19937 _randomGenerator;
  const std::uniform_int_distribution<int> _distribution;
};

namespace detail {

constexpr uint64_t gcd(uint64_t x, uint64_t y) {
  while (y != 0) {
    uint64_t oldX = x;
    x = y;
    y = oldX % y;
  }

  return x;
}

constexpr int64_t modInverse(int64_t a, int64_t m) {
  int64_t m0 = m;
  int64_t y = 0, x = 1;

  if (m == 1) {
    return 0;
  }

  while (a > 1) {
    int64_t q = a / m;
    int64_t t = m;

    m = a % m, a = t;
    t = y;

    y = x - q * y;
    x = t;
  }

  if (x < 0) {
    x += m0;
  }

  return x;
}

constexpr uint64_t powModulo(const uint32_t& a, uint32_t b, uint32_t c) {
  uint32_t x(1), y(a);
  uint64_t result(0);

  while (b > 0) {
    if (b & 1) {
      result = x * y;
      x = result % c;
    }
    result = y * y;
    y = result % c;
    b >>= 1;
  }

  return x % c;
}

}  // namespace detail

}  // namespace

void Init() {
  RsaCipher cipher;
  PrivateKey privateKey = cipher.privateKey();
  std::cout << "privateKey.d: " << privateKey.d << '\n';
  PublicKey publicKey = cipher.publicKey();
  std::cout << "publicKey.n: " << publicKey.n << '\n';

  uint64_t message = 8;
  uint64_t enc = cipher.encrypt(message);
  std::cout << "encode" << '(' << message << ')' << ':' << ' ' << enc << '\n';
  std::cout << "decode" << '(' << enc << ')' << ':' << ' '
            << cipher.decrypt(enc) << '\n';
}

RsaCipher::RsaCipher() {
  RandomPrimeSelector primeSelector;
  for (;;) {
    const uint64_t p = primeSelector.get();
    const uint64_t q = primeSelector.get();
    const uint64_t phi = (p - 1) * (q - 1);
    if (detail::gcd(phi, uint64_t(_publicKey.e)) != 1) {
      continue;
    }
    _publicKey.n = p * q;
    _privateKey.d =
        (double)detail::modInverse(int64_t(_publicKey.e), int64_t(phi));
    break;
  }
}

PublicKey RsaCipher::publicKey() const { return _publicKey; }

PrivateKey RsaCipher::privateKey() const { return _privateKey; }

uint64_t RsaCipher::encrypt(uint64_t message) const {
  if (double(message) >= _publicKey.n) {
    throw std::invalid_argument(
        "Message is too long and cannot be encrypted.\n");
  }

  return (uint64_t)detail::powModulo(message, uint32_t(_publicKey.e),
                                     uint32_t(_publicKey.n));
}

uint64_t RsaCipher::decrypt(uint64_t message) const {
  return (uint64_t)detail::powModulo(message, uint32_t(_privateKey.d),
                                     uint32_t(_publicKey.n));
}
}  // namespace lab2::rsa
