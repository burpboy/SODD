#ifndef RESOLVER_H
#define RESOLVER_H

#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>

#ifdef __cplusplus
extern "C" {
#endif

// Maksimal panjang alamat IP hasil resolve
#define RESOLVER_MAX_IP_LEN 46

/**
 * @brief Menginisialisasi custom DNS resolver dengan upstream server pilihan.
 * @param upstream_dns Alamat IP server DNS utama (contoh: "1.1.1.1" atau "8.8.8.8").
 * @return true jika inisialisasi berhasil, false jika gagal.
 */
bool resolver_init(const char *upstream_dns);

/**
 * @brief Melakukan resolusi nama domain ke alamat IP (IPv4/IPv6) secara kustom.
 * @param hostname Nama domain target yang akan di-resolve (contoh: "target.com").
 * @param ip_buffer Buffer untuk menyimpan hasil string IP.
 * @param buffer_len Panjang maksimum buffer.
 * @return true jika berhasil, false jika gagal/domain tidak ditemukan.
 */
bool resolver_resolve(const char *hostname, char *ip_buffer, size_t buffer_len);

/**
 * @brief Membersihkan cache DNS internal atau menutup socket resolver.
 */
void resolver_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* RESOLVER_H */

