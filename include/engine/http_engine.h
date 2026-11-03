#ifndef HTTP_ENGINE_H
#define HTTP_ENGINE_H

#include <stddef.h>
#include <stdint.h>
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *url;
    char *method;          /* "GET", "POST", "PUT", dll. */
    char *headers;         /* Kustom header dipisahkan dengan CRLF */
    char *body;            /* Data payload request */
    size_t body_len;
    char *proxy;           /* Format string proxy: IP:Port atau socks5://... */
    char *proxy_auth;      /* Autentikasi proxy: user:pass */
    int timeout_ms;
    int follow_redirects;
} HttpRequest;

typedef struct {
    int status_code;
    char *body;
    size_t body_len;
    double total_time;
    char *effective_url;
} HttpResponse;

/* Inisialisasi global engine HTTP (wrapper curl_global_init) */
int http_engine_global_init(void);

/* Membersihkan resource global engine HTTP */
void http_engine_global_cleanup(void);

/* Melakukan eksekusi HTTP request secara sinkron dengan dukungan proxy & TLS */
HttpResponse* http_engine_execute(const HttpRequest *req, void *tls_context);

/* Membebaskan memori alokasi dari struktur HttpResponse */
void http_response_free(HttpResponse *response);

#ifdef __cplusplus
}
#endif

#endif /* HTTP_ENGINE_H */

