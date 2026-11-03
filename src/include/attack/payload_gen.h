#ifndef PAYLOAD_GEN_H
#define PAYLOAD_GEN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Jenis format payload yang didukung
typedef enum {
    PAYLOAD_TYPE_JSON,
    PAYLOAD_TYPE_XML,
    PAYLOAD_TYPE_FORM_URLENCODED,
    PAYLOAD_TYPE_RAW_STRING
} PayloadType;

/**
 * @brief Menghasilkan string acak dengan panjang tertentu untuk padding atau fuzzing.
 * 
 * @param dest Buffer tujuan
 * @param length Panjang string yang diinginkan
 */
void payload_gen_random_string(char *dest, size_t length);

/**
 * @brief Menghasilkan payload dinamis terstruktur (JSON/XML/Form) dengan parameter acak.
 * 
 * @param type Tipe payload (PAYLOAD_TYPE_JSON, dll)
 * @param output_buffer Buffer untuk menyimpan hasil payload
 * @param buffer_size Ukuran maksimum buffer
 * @return int Panjang payload yang dihasilkan, atau - jika gagal
 */
int payload_gen_dynamic_body(PayloadType type, char *output_buffer, size_t buffer_size);

/**
 * @brief Menghasilkan parameter query URL acak (cache-busting parameter).
 * 
 * @param dest Buffer tujuan
 * @param max_len Ukuran maksimum buffer
 */
void payload_gen_cache_busting_param(char *dest, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif /* PAYLOAD_GEN_H */

