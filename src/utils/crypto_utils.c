/**
 * @file crypto_utils.c
 * @brief AES-256-CBC Encryption/Decryption wrapper using OpenSSL EVP API.
 */

#include "utils/crypto_utils.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>
#include <stdio.h>

int crypto_encrypt(const unsigned char *plaintext, int plaintext_len,
                   const unsigned char *key, const unsigned char *iv,
                   unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = NULL;
    int len = 0;
    int ciphertext_len = 0;

    // Alokasikan konteks cipher
    if (!(ctx = EVP_CIPHER_CTX_new())) return -1;

    // Inisialisasi enkripsi AES-256-CBC dengan key dan IV
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    // Proses enkripsi blok data plaintext
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len = len;

    // Finalisasi padding PKCS#7
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int crypto_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                   const unsigned char *key, const unsigned char *iv,
                   unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx = NULL;
    int len = 0;
    int plaintext_len = 0;

    // Alokasikan konteks cipher
    if (!(ctx = EVP_CIPHER_CTX_new())) return -1;

    // Inisialisasi dekripsi AES-256-CBC
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    // Proses dekripsi blok data ciphertext
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len = len;

    // Finalisasi unpadding PKCS#7
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

