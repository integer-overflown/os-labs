#ifndef OSLABS_RSA_H
#define OSLABS_RSA_H
#ifdef __cplusplus
#include <cstdint>
#define LAB3_BEGIN_DECLS extern "C" {
#define LAB3_END_DECLS }
#else
#include <stdint.h>
#define LAB3_BEGIN_DECLS
#define LAB3_END_DECLS
#endif

LAB3_BEGIN_DECLS

typedef struct {
  double d;
} PrivateKey;

typedef struct {
  double n;
  double e;
} PublicKey;

typedef struct {
  PrivateKey privateKey;
  PublicKey publicKey;
} RsaCipher;

typedef enum {
  RSA_CIPHER_ENCRYPT_STATUS_SUCCESS,
  RSA_CIPHER_ENCRYPT_STATUS_ERROR_MESSAGE_TOO_LONG
} RsaCipherEncryptStatus;

const char *rsa_encrypt_status_to_string(RsaCipherEncryptStatus status);

typedef enum { RSA_CIPHER_DECRYPT_STATUS_SUCCESS } RsaCipherDecryptStatus;

void rsa_cipher_init(RsaCipher *instance);

_Check_return_ RsaCipherEncryptStatus rsa_cipher_encrypt(
    const RsaCipher *instance, uint64_t message, uint64_t *outEncrypted);

_Check_return_ RsaCipherDecryptStatus rsa_cipher_decrypt(
    const RsaCipher *instance, uint64_t message, uint64_t *outDecrypted);

LAB3_END_DECLS

#endif  // OSLABS_RSA_H
