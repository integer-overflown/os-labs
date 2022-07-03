#include <iostream>
#include <limits>

#include "rsa.h"

int main() {
  using namespace lab2::rsa;
  RsaCipher cipher;
  PrivateKey privateKey = cipher.privateKey();
  std::cout << "privateKey.d: " << privateKey.d << '\n';
  PublicKey publicKey = cipher.publicKey();
  std::cout << "publicKey.n: " << publicKey.n << '\n';

  uint64_t message;
  uint64_t enc;

  for (;;) {
    while (!((std::cout << '>' << ' ') && std::cin >> message)) {
      std::cout << "Please enter a valid number" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    try {
      enc = cipher.encrypt(message);
      break;
    } catch (const std::invalid_argument &ex) {
      std::cout << "Unable to process given message, reason: " << ex.what();
    } catch (const std::exception &ex) {
      std::cout << "Unknown error occurred: " << ex.what();
    }
  }

  std::cout << "encode" << '(' << message << ')' << ':' << ' ' << enc << '\n';
  std::cout << "decode" << '(' << enc << ')' << ':' << ' '
            << cipher.decrypt(enc) << '\n';
}
