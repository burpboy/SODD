/**
 * @file api_hammer.h
 * @brief Header untuk simulasi penanganan Resource Exhaustion & API Hammering
 */

#ifndef API_HAMMER_H
#define API_HAMMER_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Konfigurasi Batas Sumber Daya
#define MAX_CONCURRENT_REQUESTS  100
#define RATE_LIMIT_WINDOW_SEC    1
#define MAX_REQUESTS_PER_WINDOW  50

// Struktur untuk melacak status resource per klien / endpoint
typedef struct {
    uint32_t client_id;
    uint32_t request_count;
    time_t window_start_time;
    bool is_throttled;
} ApiClientState;

// Struktur untuk metrik sistem global (memantau potensi resource exhaustion)
typedef struct {
    uint32_t active_connections;
    uint64_t total_rejected_requests;
    uint64_t total_processed_requests;
    bool memory_exhausted_warning;
} SystemResourceMetrics;

/**
 * @brief Memeriksa apakah permintaan melampaui batas rate limit (mencegah hammering).
 * 
 * @param client Pointer ke status klien.
 * @return true Jika permintaan diizinkan.
 * @return false Jika permintaan ditolak (Rate Limited / Resource Exhaustion Prevention).
 */
bool api_hammer_check_rate_limit(ApiClientState *client) {
    time_t current_time = time(NULL);

    if (current_time - client->window_start_time >= RATE_LIMIT_WINDOW_SEC) {
        // Reset window baru
        client->window_start_time = current_time;
        client->request_count = 0;
        client->is_throttled = false;
    }

    if (client->request_count >= MAX_REQUESTS_PER_WINDOW) {
        client->is_throttled = true;
        return false; // Tolak untuk mencegah kehabisan sumber daya
    }

    client->request_count++;
    return true;
}

/**
 * @brief Memeriksa apakah server mendekati batas kapasitas maksimum (Resource Exhaustion Check).
 * 
 * @param metrics Pointer ke metrik sistem global.
 * @return true Jika server aman.
 * @return false Jika sumber daya hampir habis (Out of Memory / Max Connections reached).
 */
bool api_hammer_verify_capacity(SystemResourceMetrics *metrics) {
    if (metrics->active_connections >= MAX_CONCURRENT_REQUESTS) {
        metrics->memory_exhausted_warning = true;
        metrics->total_rejected_requests++;
        return false; // Tolak koneksi baru untuk melindungi server
    }

    metrics->memory_exhausted_warning = false;
    return true;
}

#endif // API_HAMMER_H

