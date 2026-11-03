#include "../../include/network/geo_locator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Fungsi untuk melakukan lookup IP dan mengembalikan kode negara (simulasi/implementasi dasar)
int geo_lookup(const char *ip_address, char *country_code_out, size_t max_len) {
    if (!ip_address || !country_code_out) {
        return -1;
    }

    // Contoh implementasi placeholder/lokal. 
    // Pada implementasi produksi, Anda bisa mengintegrasikan MaxMind GeoIP2 atau query HTTP API.
    if (strncmp(ip_address, "192.168.", 8) == 0 || strcmp(ip_address, "127.0.0.1") == 0) {
        snprintf(country_code_out, max_len, "LO"); // Local / LAN
    } else {
        // Default mock untuk pengujian
        snprintf(country_code_out, max_len, "US");
    }

    return 0;
}

