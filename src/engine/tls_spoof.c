#include "../../include/engine/tls_spoof.h"
#include <curl/curl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>

// Callback kustom OpenSSL untuk memodifikasi konteks TLS (Client Hello Manipulation)
CURLcode tls_ctx_callback(CURL *curl, void *ssl_ctx, void *userptr) {
    SSL_CTX *ctx = (SSL_CTX *)ssl_ctx;

    // Atur versi minimum dan maksimum TLS (contoh: TLS 1.2 hingga TLS 1.3)
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION);

    // Kustomisasi Cipher Suites agar sesuai dengan urutan sidik jari browser standar (JA3 masking)
    // Contoh string cipher modern Chrome/Firefox
    const char *cipher_list = "TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256:"
                              "ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:"
                              "ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384";
    
    if (SSL_CTX_set_cipher_list(ctx, cipher_list) != 1) {
        // Fallback jika gagal menetapkan cipher spesifik
        // Penanganan error internal OpenSSL
    }

    // Menonaktifkan ekstensi tertentu atau memodifikasi ekstensi penunjang TLS 
    // untuk menghindari deteksi Cloudflare / Akamai JA3/JA4 anomaly.
    SSL_CTX_set_options(ctx, SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION);

    return CURLE_OK;
}

void tls_spoof_apply_options(CURL *curl) {
    // Daftarkan callback SSL context ke libcurl
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, tls_ctx_callback);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, NULL);

    // Paksa penggunaan HTTP/2 atau HTTP/1.1 sesuai konfigurasi fingerprint
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
}

int tls_spoof_init(void) {
    // Inisialisasi tambahan OpenSSL jika diperlukan secara global
    OPENSSL_init_ssl(0, NULL);
    return 0;
}

