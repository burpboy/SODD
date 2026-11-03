#ifndef PROXY_MANAGER_H
#define PROXY_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Tipe protokol proxy
typedef enum {
    PROXY_TYPE_HTTP = 0,
    PROXY_TYPE_HTTPS,
    PROXY_TYPE_SOCKS4,
    PROXY_TYPE_SOCKS5
} ProxyType;

// Struktur data sebuah proxy
typedef struct {
    char host[256];       // IP atau Domain proxy
    uint16_t port;        // Port proxy
    char username[128];   // Username autentikasi (opsional)
    char password[128];   // Password autentikasi (opsional)
    ProxyType type;       // Tipe proxy
    bool is_active;       // Status kesehatan proxy saat ini
    uint32_t failure_count; // Akumulasi kegagalan koneksi
    int64_t last_checked; // Timestamp pengecekan terakhir
} ProxyNode;

/**
 * @brief Menginisialisasi manajer proxy dan memuat pool dari file CSV.
 * @param csv_path Path ke file aset `proxies.csv`.
 * @return true jika berhasil memuat minimal satu proxy, false jika gagal.
 */
bool proxy_manager_init(const char *csv_path);

/**
 * @brief Mengambil satu proxy aktif dari pool menggunakan strategi rotasi (Round-Robin / Random).
 * @return Pointer ke struct ProxyNode, atau NULL jika pool kosong/habis.
 */
ProxyNode* proxy_manager_get_next(void);

/**
 * @brief Melaporkan status penggunaan proxy (sukses/gagal) untuk penyesuaian skor kesehatan.
 * @param proxy Pointer ke ProxyNode yang digunakan.
 * @brief success true jika request sukses, false jika timeout/gagal terhubung.
 */
void proxy_manager_report_status(ProxyNode *proxy, bool success);

/**
 * @brief Menjalankan background thread untuk melakukan health check berkala pada semua proxy.
 */
void proxy_manager_start_health_checker(void);

/**
 * @brief Membersihkan seluruh alokasi memori pool proxy.
 */
void proxy_manager_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* PROXY_MANAGER_H */

