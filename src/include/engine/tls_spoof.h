#ifndef TLS_SPOOF_H
#define TLS_SPOOF_H

#include <stdbool.h>
#include <openssl/ssl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Tipe profil JA3/Browser yang didukung untuk spoofing
typedef enum {
    TLS_PROFILE_CHROME_LATEST = 0,
    TLS_PROFILE_FIREFOX_LATEST,
    TLS_PROFILE_SAFARI_LATEST,
    TLS_PROFILE_RANDOM
} TlsProfileType;

// Mengonfigurasi SSL_CTX untuk mensimulasikan Client Hello sidik jari browser tertentu (JA3)
bool tls_spoof_configure_context(SSL_CTX *ctx, TlsProfileType profile_type);

// Mengatur cipher suites khusus untuk mencocokkan string JA3 target
bool tls_spoof_set_cipher_list(SSL *ssl, const char *cipher_list);

// Menerapkan ekstensi TLS tambahan (SNI, ALPN, Supported Versions, dll.)
bool tls_spoof_apply_extensions(SSL *ssl, TlsProfileType profile_type);

#ifdef __cplusplus
}
#endif

#endif // TLS_SPOOF_H

