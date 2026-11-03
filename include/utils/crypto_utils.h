/**
 * @file crypto_utils.h
 * @brief Enkripsi/dekripsi AES-256 menggunakan OpenSSL EVP API untuk komunikasi C2.
 */

#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <stddef.h>
#include <stdint.h>

#define AES_KEY_SIZE 32 /* 256 bits */
#define AES_BLOCK_SIZE 16

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Mengenkripsi plaintext menggunakan AES-256-CBC.
 * 
 * @param plaintext Buffer data asli yang akan dienkripsi.
 * @param plaintext_len Panjang data plaintext dalam byte.
 * @param key Kunci enkripsi 256-bit (32 byte).
 * @param iv Initialization Vector (16 byte).
 * @param ciphertext Buffer output untuk menampung hasil enkripsi.
 * @param ciphertext_len Pointer untuk menyimpan panjang ciphertext hasil akhir.
 * @return int Mengembalikan 1 jika sukses, 0 jika gagal.
 */
int crypto_aes_encrypt(
    const unsigned char *plaintext, int plaintext_len,
    const unsigned char *key, const unsigned char *iv,
    unsigned char *ciphertext, int *ciphertext_len
);

/**
 * @brief Mendekripsi ciphertext menggunakan AES-256-CBC.
 * 
 * @param ciphertext Buffer data terenkripsi.
 * @param ciphertext_len Panjang data ciphertext dalam byte.
 * @param key Kunci dekripsi 256-bit (32 byte).
 * @param iv Initialization Vector (16 byte).
 * @param plaintext Buffer output untuk menampung hasil dekripsi.
 * @param plaintext_len Pointer untuk menyimpan panjang plaintext hasil akhir.
 * @return int Mengembalikan 1 jika sukses, 0 jika gagal/padding error.
 */
int crypto_aes_decrypt(
    const unsigned char *ciphertext, int ciphertext_len,
    const unsigned char *key, const unsigned char *iv,
    unsigned char *plaintext, int *plaintext_len
);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_UTILS_H */
