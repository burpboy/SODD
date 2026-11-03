/**
 * @file payload_gen.h
 * @brief Header untuk generator payload dinamis (JSON/XML/String)
 */

#ifndef PAYLOAD_GEN_H
#define PAYLOAD_GEN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PAYLOAD_TYPE_JSON,
    PAYLOAD_TYPE_XML,
    PAYLOAD_TYPE_FORM
} payload_type_t;

/**
 * @brief Menghasilkan payload dinamis secara acak/terstruktur
 * @param type Tipe payload (JSON, XML, Form)
 * @param output_buffer Pointer untuk menampung hasil payload
 * @param max_len Panjang maksimum buffer
 * @return size_t Ukuran payload aktual yang dihasilkan
 */
size_t payload_generate(payload_type_t type, char *output_buffer, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif /* PAYLOAD_GEN_H */

