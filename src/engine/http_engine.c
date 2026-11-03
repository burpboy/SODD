#include "../../include/engine/http_engine.h"
#include "../../include/engine/ua_rotator.h"
#include "../../include/engine/tls_spoof.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Callback untuk menampung respons (jika diperlukan untuk debugging/logging)
size_t http_write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Bisa diimplementasikan buffer penampung jika ingin membaca body respons
    return realsize;
}

int http_engine_init(void) {
    CURLcode res = curl_global_init(CURLGLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        fprintf(stderr, "[HTTP_ENGINE] Gagal menginisialisasi CURL global: %s\n", curl_easy_strerror(res));
        return -1;
    }
    return 0;
}

void http_engine_cleanup(void) {
    curl_global_cleanup();
}

HttpResponse http_engine_send_request(const HttpRequestConfig *config) {
    HttpResponse response = {0.0, 0, 0, NULL};
    CURL *curl = curl_easy_init();
    
    if (!curl) {
        response.status_code = -1;
        return response;
    }

    struct curl_slist *headers = NULL;
    char current_ua[256];

    // Ambil User-Agent dari rotator jika tidak diset secara manual
    if (config->user_agent && strlen(config->user_agent) > 0) {
        snprintf(current_ua, sizeof(current_ua), "%s", config->user_agent);
    } else {
        ua_rotator_get_random(current_ua, sizeof(current_ua));
    }

    char ua_header[512];
    snprintf(ua_header, sizeof(ua_header), "User-Agent: %s", current_ua);
    headers = curl_slist_append(headers, ua_header);

    // Tambahkan kustom header tambahan jika ada
    for (int i = 0; i < config->header_count; i++) {
        headers = curl_slist_append(headers, config->custom_headers[i]);
    }

    // Konfigurasi opsi CURL
    curl_easy_setopt(curl, CURLOPT_URL, config->url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_callback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, config->timeout_sec);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Dilewati untuk simulasi/testing
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Konfigurasi Proxy jika disediakan
    if (config->proxy_url && strlen(config->proxy_url) > 0) {
        curl_easy_setopt(curl, CURLOPT_PROXY, config->proxy_url);
    }

    // Terapkan manipulasi TLS handshake kustom (JA3 spoofing)
    tls_spoof_apply_options(curl);

    // Eksekusi request dan ukur waktu latensi
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.status_code);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &response.latency_ms);
        response.success = 1;
    } else {
        response.status_code = -2; // Error koneksi / timeout
        response.success = 0;
        fprintf(stderr, "[HTTP_ENGINE] Permintaan gagal ke %s: %s\n", config->url, curl_easy_strerror(res));
    }

    // Cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response;
}

