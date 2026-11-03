#ifndef HUMAN_MIMICRY_H
#define HUMAN_MIMICRY_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Struktur untuk merepresentasikan koordinat/pergerakan mouse tiruan
typedef struct {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int steps;
    unsigned int duration_ms;
} MouseMovement;

/**
 * @brief Melakukan jeda waktu (sleep) acak dengan distribusi Gaussian/Poisson 
 *        untuk meniru jeda pikir manusia antar request.
 * 
 * @param min_ms Jeda minimum dalam milidetik
 * @param max_ms Jeda maksimum dalam milidetik
 */
void mimic_random_delay(unsigned int min_ms, unsigned int max_ms);

/**
 * @brief Mensimulasikan jeda pengetikan antar karakter (typing cadence).
 * 
 * @param char_count Jumlah karakter yang akan diketik
 */
void mimic_typing_delay(size_t char_count);

/**
 * @brief Menghasilkan kumpulan titik koordinat pergerakan mouse tiruan 
 *        menggunakan kurva Bézier atau interpolasi acak.
 * 
 * @param start_x Posisi awal X
 * @param start_y Posisi awal Y
 * @param end_x Posisi akhir X
 * @param end_y Posisi akhir Y
 * @return MouseMovement Konfigurasi pergerakan mouse
 */
MouseMovement mimic_generate_mouse_path(int start_x, int start_y, int end_x, int end_y);

/**
 * @brief Menghasilkan header tambahan (misal: sec-ch-ua, sec-fetch-*, dnt)
 *        untuk menyamakan sidik jari browser asli.
 * 
 * @pointer buffer Buffer penampung string header
 * @param max_len Ukuran maksimum buffer
 */
void mimic_generate_behavioral_headers(char *buffer, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif /* HUMAN_MIMICRY_H */

