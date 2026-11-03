#ifndef HTTP_ENGINE_H
#define HTTP_ENGINE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Struktur untuk menyimpan konfigurasi request HTTP
typedef struct {
    const char *url;
    const char *method;       // "GET", "POST", "PUT", dll.
    const char *payload;      // Body data untuk POST/PUT
    size_t payload_len;
    const char *proxy;        // Format: ip:port atau http://ip:port
    const char *user_agent;
    long timeout_ms;
    bool follow_redirects;
} HttpRequest;

// Struktur untuk menampung hasil response HTTP
typedef struct {
    long status_code;
    char *body;
    size_t body_len;
    double total_time_ms;
    char error_buffer[CURL_ERROR_SIZE];
} HttpResponse;

// Inisialisasi global untuk engine HTTP (memanggil curl_global_init)
bool http_engine_global_init(void);

// Membersihkan resource global HTTP engine
void http_engine_global_cleanup(void);

// Mengirimkan request HTTP tunggal menggunakan wrapper libcurl
bool http_engine_send(const HttpRequest *req, HttpResponse *res);

// Membersihkan memori yang dialokasikan pada objek HttpResponse
void http_engine_free_response(HttpResponse *res);

#ifdef __cplusplus
}
#endif

#endif // HTTP_ENGINE_H

