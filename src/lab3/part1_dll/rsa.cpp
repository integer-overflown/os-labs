#include "rsa.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

struct RsaCipher {
  PrivateKey privateKey;
  PublicKey publicKey;
};

namespace {

uint16_t gPrimesTable[] = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
    53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281};

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

constexpr uint64_t powModulo(const uint32_t &a, uint32_t b, uint32_t c) {
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

RsaCipherEncryptStatus rsa_cipher_encrypt(const RsaCipher *instance,
                                          uint64_t message,
                                          uint64_t *outEncrypted) {
  assert(instance);
  assert(outEncrypted);
  if (double(message) >= instance->publicKey.n) {
    return RSA_CIPHER_ENCRYPT_STATUS_ERROR_MESSAGE_TOO_LONG;
  }

  *outEncrypted =
      (uint64_t)detail::powModulo(message, uint32_t(instance->publicKey.e),
                                  uint32_t(instance->publicKey.n));

  return RSA_CIPHER_ENCRYPT_STATUS_SUCCESS;
}

RsaCipherDecryptStatus rsa_cipher_decrypt(const RsaCipher *instance,
                                          uint64_t message,
                                          uint64_t *outDecrypted) {
  assert(instance);
  assert(outDecrypted);
  *outDecrypted =
      (uint64_t)detail::powModulo(message, uint32_t(instance->privateKey.d),
                                  uint32_t(instance->publicKey.n));

  return RSA_CIPHER_DECRYPT_STATUS_SUCCESS;
}

const char *rsa_encrypt_status_to_string(RsaCipherEncryptStatus status) {
  switch (status) {
    case RSA_CIPHER_ENCRYPT_STATUS_SUCCESS:
      return "success";
    case RSA_CIPHER_ENCRYPT_STATUS_ERROR_MESSAGE_TOO_LONG:
      return "message is too long";
    default:
      return "unknown error";
  }
}

RsaCipher *rsa_cipher_new() {
  // By design, I wanted the library to be usable from C as well
  // So we can't use 'new' here, because it throws, and C cannot handle
  // exceptions i.e. we must stick with C way of memory allocations
  auto instance = static_cast<RsaCipher *>(malloc(sizeof(RsaCipher)));

  if (!instance) {  // out of memory
    return nullptr;
  }

  instance->publicKey.e = 3;
  RandomPrimeSelector primeSelector;
  for (;;) {
    const uint64_t p = primeSelector.get();
    const uint64_t q = primeSelector.get();
    const uint64_t phi = (p - 1) * (q - 1);
    if (detail::gcd(phi, uint64_t(instance->publicKey.e)) != 1) {
      continue;
    }
    instance->publicKey.n = double(p * q);
    instance->privateKey.d = (double)detail::modInverse(
        int64_t(instance->publicKey.e), int64_t(phi));
    break;
  }

  return instance;
}

PublicKey rsa_cipher_get_public_key(const RsaCipher *instance) {
  assert(instance);
  return instance->publicKey;
}

PrivateKey rsa_cipher_get_private_key(const RsaCipher *instance) {
  assert(instance);
  return instance->privateKey;
}

void rsa_cipher_free(RsaCipher *instance) { free(instance); }
