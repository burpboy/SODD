#ifndef SLOWLORIS_H
#define SLOWLORIS_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// Struktur konfigurasi konteks Slowloris
typedef struct {
    char *target_ip;
    uint16_t target_port;
    bool use_tls;
    int socket_fd;
    unsigned int interval_sec; // Jeda pengiriman header parsial
    bool is_active;
} SlowlorisContext;

/**
 * @brief Menginisialisasi koneksi soket ke target untuk serangan Slowloris.
 * 
 * @param ctx Pointer ke struktur SlowlorisContext
 * @param ip IP target
 * @param port Port target (biasanya 80 atau 443)
 * @param use_tls Gunakan HTTPS/TLS atau tidak
 * @return true Jika koneksi berhasil dibuat, false jika gagal
 */
bool slowloris_init_connection(SlowlorisContext *ctx, const char *ip, uint16_t port, bool use_tls);

/**
 * @brief Mengirimkan baris header awal HTTP (partial request).
 * 
 * @param ctx Pointer ke struktur SlowlorisContext
 * @return true Jika berhasil dikirim, false jika koneksi terputus
 */
bool slowloris_send_initial_headers(SlowlorisContext *ctx);

/**
 * @brief Mengirimkan header tiruan berkala secara lambat (keep-alive / dummy header)
 *        untuk menjaga agar koneksi tetap hidup (open).
 * 
 * @param ctx Pointer ke struktur SlowlorisContext
 * @return true Jika berhasil, false jika server menutup koneksi
 */
bool slowloris_keep_connection_alive(SlowlorisContext *ctx);

/**
 * @brief Menutup soket dan membersihkan resource Slowloris.
 * 
 * @param ctx Pointer ke struktur SlowlorisContext
 */
void slowloris_cleanup(SlowlorisContext *ctx);

#ifdef __cplusplus
}
#endif

#endif /* SLOWLORIS_H */

