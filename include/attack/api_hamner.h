/**
 * @file api_hammer.h
 * @brief Header untuk modul resource exhaustion (API Hammering)
 */

#ifndef API_HAMMER_H
#define API_HAMMER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *endpoint_url;
    uint32_t concurrency_level;
    uint32_t request_interval_ms;
    bool target_heavy_queries;
} api_hammer_config_t;

/**
 * @brief Inisialisasi modul API hammer
 * @param config Konfigurasi target endpoint
 * @return int 0 jika sukses, -1 jika gagal
 */
int api_hammer_init(const api_hammer_config_t *config);

/**
 * @brief Menjalankan serangan resource exhaustion pada endpoint tertentu
 */
void api_hammer_start(void);

/**
 * @brief Menghentikan serangan API hammer
 */
void api_hammer_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* API_HAMMER_H */

