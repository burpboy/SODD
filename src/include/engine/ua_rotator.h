#ifndef UA_ROTATOR_H
#define UA_ROTATOR_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Kategori User-Agent yang dapat dipilih
typedef enum {
    UA_TYPE_CHROME = 0,
    UA_TYPE_FIREFOX,
    UA_TYPE_SAFARI,
    UA_TYPE_MOBILE,
    UA_TYPE_ALL
} UAType;

// Memuat daftar User-Agent dari file (misal: assets/user_agents.txt)
bool ua_rotator_init(const char *filepath);

// Membebaskan alokasi memori daftar User-Agent
void ua_rotator_cleanup(void);

// Mendapatkan User-Agent secara acak berdasarkan tipe yang diinginkan
const char* ua_rotator_get_random(UAType type);

// Mendapatkan User-Agent berikutnya secara berurutan (Round-Robin)
const char* ua_rotator_get_next(void);

// Mengembalikan total jumlah User-Agent yang berhasil dimuat ke dalam memori
int ua_rotator_count(void);

#ifdef __cplusplus
}
#endif

#endif // UA_ROTATOR_H

