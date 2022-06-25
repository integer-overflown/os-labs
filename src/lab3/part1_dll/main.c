/*
Several considerations taken when writing this file:
1) It's pure C intentionally, to show that the lib is interoperable
with C and C++
2) It intentionally doesn't include "rsa.h" header, consequently not using
any of types declared there, to show that the lib can be used purely in
runtime mode (as plugin, for example)
*/

#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>

typedef struct RsaCipher RsaCipher; /* opaque pointer */

typedef RsaCipher *(*RsaLibNewFunc)();
/* enum types are safely convertible to int */
typedef int (*RsaLibEncryptFunc)(const RsaCipher *, uint64_t, uint64_t *);
typedef int (*RsaLibDecryptFunc)(const RsaCipher *, uint64_t, uint64_t *);
typedef const char *(*RsaLibEncryptStatusToStringFunc)(int);
typedef void (*RsaLibFreeFunc)(RsaCipher *);

typedef struct {
  RsaLibNewFunc
      allocate; /* not 'new', because this word is reserved (in C++) */
  RsaLibEncryptFunc encrypt;
  RsaLibDecryptFunc decrypt;
  RsaLibEncryptStatusToStringFunc encryptStatusToString;
  RsaLibFreeFunc free;
} RsaLibFunctionTable;

_Check_return_ static int loadRsaLibrary(
    RsaLibFunctionTable *const functionTable) {
#define LOAD_SYMBOL(place, type, name)                          \
  do {                                                          \
    if (!((place) = (type)GetProcAddress(dllHandle, (name)))) { \
      fprintf(stderr, "Symbol not found: %s\n", name);          \
      FreeLibrary(dllHandle);                                   \
      return 0;                                                 \
    }                                                           \
  } while (0)

  const TCHAR *const fileName = _T("os_lab3.dll");
  HANDLE dllHandle = LoadLibrary(fileName);

  if (!dllHandle) {
    _tprintf(_T("Failed to load library: %s\n"), fileName);
    return 0;
  }

  LOAD_SYMBOL(functionTable->allocate, RsaLibNewFunc, "rsa_cipher_new");
  LOAD_SYMBOL(functionTable->encrypt, RsaLibEncryptFunc, "rsa_cipher_encrypt");
  LOAD_SYMBOL(functionTable->decrypt, RsaLibDecryptFunc, "rsa_cipher_decrypt");
  LOAD_SYMBOL(functionTable->encryptStatusToString,
              RsaLibEncryptStatusToStringFunc, "rsa_encrypt_status_to_string");
  LOAD_SYMBOL(functionTable->free, RsaLibFreeFunc, "rsa_cipher_free");

  return 1;

#undef LOAD_SYMBOL
}

int main(int argc, char *argv[]) {
  char *endPtr = NULL;
  uint64_t message;
  uint64_t encrypted, decrypted;
  int status;
  RsaLibFunctionTable funcTable;
  int loaded;
  RsaCipher *cipher = NULL;

  if (argc != 2) {
    fprintf(stderr, "usage: %s [message to encrypt]\n", argv[0]);
    return EXIT_FAILURE;
  }

  errno = 0;
  message = strtoull(argv[1], &endPtr, 10);

  if (errno || *endPtr != '\0') {
    const char *reason =
        errno == ERANGE ? "is too large" : "must be a valid integer";
    fprintf(stderr, "Failed to parse '%s': %s", argv[1], reason);
    return EXIT_FAILURE;
  }

  loaded = loadRsaLibrary(&funcTable);

  if (!loaded) {
    fprintf(stderr, "Failed to load the library");
    return EXIT_FAILURE;
  }

  cipher = funcTable.allocate();

  if (!cipher) {
    fprintf(stderr, "Out of memory");
    return EXIT_FAILURE;
  }

  status = funcTable.encrypt(cipher, message, &encrypted);

  if (status) {
    fprintf(stderr, "Failed to encrypt the message, reason: %s\n",
            funcTable.encryptStatusToString(status));
    funcTable.free(cipher);
    return EXIT_FAILURE;
  }

  printf("encrypt(%" PRIu64 "): %" PRIu64 "\n", message, encrypted);

  if (funcTable.decrypt(cipher, encrypted, &decrypted)) {
    fprintf(stderr, "Failed to decrypt the message");
    funcTable.free(cipher);
    return EXIT_FAILURE;
  }

  printf("decrypt(%" PRIu64 "): %" PRIu64 "\n", encrypted, decrypted);

  funcTable.free(cipher);
  return EXIT_SUCCESS;
}
