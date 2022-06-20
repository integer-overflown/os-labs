#ifndef OSLABS_RSA_H
#define OSLABS_RSA_H
#ifdef LAB2_LIB_BUILD
#define LAB2_LIB_API __declspec(dllexport)
#else
#define LAB2_LIB_API __declspec(dllimport)
#endif

#include <cstdint>

namespace lab2::rsa {

struct PrivateKey {
  double d;
};

struct PublicKey {
  double n{};
  const double e = 3;
};

class LAB2_LIB_API RsaCipher {
 public:
  RsaCipher();
  [[nodiscard]] PublicKey publicKey() const;
  [[nodiscard]] PrivateKey privateKey() const;
  [[nodiscard]] uint64_t encrypt(uint64_t message) const;
  [[nodiscard]] uint64_t decrypt(uint64_t message) const;

 private:
  PublicKey _publicKey;
  PrivateKey _privateKey{};
};

}  // namespace lab2::rsa

#endif  // OSLABS_RSA_H
