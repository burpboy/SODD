/**
 * @file human_mimicry.h
 * @brief Header untuk simulasi delay manusia dan pergerakan/interaksi
 */

#ifndef HUMAN_MIMICRY_H
#define HUMAN_MIMICRY_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t min_delay_ms;
    uint32_t max_delay_ms;
    double mouse_jitter_probability;
} mimicry_config_t;

/**
 * @brief Mengatur parameter delay acak gaya manusia
 * @param config Konfigurasi delay & jitter
 */
void mimicry_init(const mimicry_config_t *config);

/**
 * @brief Melakukan sleep acak untuk menyerupai pola penjelajahan manusia
 */
void mimicry_apply_delay(void);

/**
 * @brief Menghasilkan header tambahan atau simulasi koordinat interaksi
 * @param buffer Buffer penampung data simulasi
 * @param size Ukuran buffer
 */
void mimicry_generate_interaction_headers(char *buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* HUMAN_MIMICRY_H */

