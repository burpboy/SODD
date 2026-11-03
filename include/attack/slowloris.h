/**
 * @file slowloris.h
 * @brief Header untuk modul connection exhaustion (Slowloris)
 */

#ifndef SLOWLORIS_H
#define SLOWLORIS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *target_ip;
    uint16_t target_port;
    uint32_t socket_count;
    uint32_t send_interval_sec;
    bool use_ssl;
} slowloris_config_t;

/**
 * @brief Inisialisasi koneksi slowloris
 * @param config Konfigurasi target dan soket
 * @return int 0 jika sukses, -1 jika gagal
 */
int slowloris_init(const slowloris_config_t *config);

/**
 * @brief Memulai pengiriman fragmen HTTP header berkala untuk menjaga koneksi tetap terbuka
 */
void slowloris_run(void);

/**
 * @brief Menutup seluruh soket dan membersihkan resource
 */
void slowloris_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* SLOWLORIS_H */

