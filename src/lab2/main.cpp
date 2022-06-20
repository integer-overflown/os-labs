#include <iostream>

#include "rsa.h"

int main() {
  using namespace lab2::rsa;
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
