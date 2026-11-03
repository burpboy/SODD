#ifndef UA_ROTATOR_H
#define UA_ROTATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UA_DEVICE_DESKTOP = 0,
    UA_DEVICE_MOBILE,
    UA_DEVICE_TABLET,
    UA_DEVICE_ALL
} UADeviceType;

/* Memuat daftar User-Agent dari file eksternal (misal: assets/user_agents.txt) */
int ua_rotator_init(const char *filepath);

/* Mengambil string User-Agent acak berdasarkan preferensi tipe perangkat */
const char* ua_rotator_get_random(UADeviceType type);

/* Mengambil User-Agent spesifik yang konsisten dengan fingerprint browser tertentu */
const char* ua_rotator_get_matching(int browser_type);

/* Membersihkan memori pool User-Agent */
void ua_rotator_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* UA_ROTATOR_H */

