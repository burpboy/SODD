#ifndef GEO_LOCATOR_H
#define GEO_LOCATOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Struktur data informasi lokasi IP
typedef struct {
    char ip[46];          // Mendukung IPv4 dan IPv6
    char country_code[3]; // Kode negara ISO (contoh: "ID", "US", "SG")
    char region[64];      // Nama region/provinsi
    char city[64];        // Nama kota
    double latitude;      // Koordinat lintang
    double longitude;     // Koordinat bujur
    bool is_cached;       // Status apakah data dari cache lokal
} GeoLocation;

/**
 * @brief Menginisialisasi database geolokasi (misal: MaxMind GeoIP2 / MMDB).
 * @param db_path Path ke file database geolokasi.
 * @return true jika berhasil, false jika gagal.
 */
bool geo_locator_init(const char *db_path);

/**
 * @brief Melakukan lookup lokasi berdasarkan alamat IP.
 * @param ip_str Alamat IP dalam bentuk string.
 * @param loc Pointer ke struct GeoLocation untuk menyimpan hasil.
 * @return true jika lookup sukses, false jika gagal/IP tidak ditemukan.
 */
bool geo_locator_lookup(const char *ip_str, GeoLocation *loc);

/**
 * @brief Membebaskan sumber daya atau menutup handle database geolokasi.
 */
void geo_locator_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* GEO_LOCATOR_H */

