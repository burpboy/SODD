#ifndef TLS_SPOOF_H
#define TLS_SPOOF_H

#include <openssl/ssl.h>
#include <openssl/err.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Enum untuk jenis target browser fingerprint */
typedef enum {
    JA3_CHROME_LATEST = 0,
    JA3_FIREFOX_LATEST,
    JA3_SAFARI_LATEST,
    JA3_EDGE_LATEST,
    JA3_CUSTOM
} BrowserFingerprintType;

typedef struct {
    BrowserFingerprintType type;
    char *cipher_suites;
    char *extensions;
    int supported_versions;
    int elliptic_curves;
    int point_formats;
} TlSpoofConfig;

/* Menginisialisasi konteks manipulasi TLS handshake berdasarkan target browser */
SSL_CTX* tls_spoof_create_context(BrowserFingerprintType bp_type);

/* Mengatur kustomisasi Client Hello hook untuk socket/libcurl */
int tls_spoof_apply_fingerprint(SSL *ssl, BrowserFingerprintType bp_type);

/* Membersihkan konteks TLS spoofing */
void tls_spoof_free_context(SSL_CTX *ctx);

#ifdef __cplusplus
}
#endif

#endif /* TLS_SPOOF_H */

