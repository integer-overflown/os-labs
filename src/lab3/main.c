#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "rsa.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s [message to encrypt]\n", argv[0]);
    return EXIT_FAILURE;
  }

  errno = 0;
  char *endPtr;
  const uint64_t message = strtoull(argv[1], &endPtr, 10);

  if (errno || *endPtr != '\0') {
    const char *reason = errno == ERANGE ? "is too large" : "must be a valid integer";
    fprintf(stderr, "Failed to parse '%s': %s", argv[1], reason);
    return EXIT_FAILURE;
  }

  RsaCipher cipher;
  rsa_cipher_init(&cipher);
  printf(
      "RSA Initialisation:\n"
      "publicKey.e = %.2f\n"
      "publicKey.n = %.2f\n"
      "privateKey.d = %.2f\n",
      cipher.publicKey.e, cipher.publicKey.n, cipher.privateKey.d);

  uint64_t encrypted, decrypted;

  RsaCipherEncryptStatus status =
      rsa_cipher_encrypt(&cipher, message, &encrypted);

  if (status != RSA_CIPHER_ENCRYPT_STATUS_SUCCESS) {
    fprintf(stderr, "Failed to encrypt the message, reason: %s\n",
            rsa_encrypt_status_to_string(status));
    return EXIT_FAILURE;
  }

  printf("encrypt(%" PRIu64 "): %" PRIu64 "\n", message, encrypted);

  if (rsa_cipher_decrypt(&cipher, encrypted, &decrypted) !=
      RSA_CIPHER_DECRYPT_STATUS_SUCCESS) {
    fprintf(stderr, "Failed to decrypt the message");
    return EXIT_FAILURE;
  }

  printf("decrypt(%" PRIu64 "): %" PRIu64 "\n", encrypted, decrypted);

  return EXIT_SUCCESS;
}
