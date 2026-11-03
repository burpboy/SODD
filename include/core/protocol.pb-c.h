/**
 * @file protocol.pb-c.h
 * @brief Header hasil simulasi kompilasi Protocol Buffers untuk komunikasi C2.
 * Catatan: Biasanya dihasilkan secara otomatis oleh protoc-c dari file protocol.proto.
 */

#ifndef PROTOCOL_PB_C_H
#define PROTOCOL_PB_C_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Forward declarations untuk tipe pesan Protobuf-c
typedef struct _C2Message C2Message;
typedef struct _CommandPayload CommandPayload;
typedef struct _HeartbeatPayload HeartbeatPayload;

typedef enum {
    C2_MESSAGE_TYPE_COMMAND = 1,
    C2_MESSAGE_TYPE_HEARTBEAT = 2,
    C2_MESSAGE_TYPE_RESPONSE = 3
} C2MessageType;

struct _CommandPayload {
    char *target_url;
    uint32_t duration_sec;
    uint32_t intensity_rps;
    char *attack_type; // cth: slowloris, api_hammer
};

struct _HeartbeatPayload {
    uint32_t node_id;
    float cpu_usage;
    float memory_usage;
    uint64_t req_count;
    bool is_attacking;
};

struct _C2Message {
    C2MessageType msg_type;
    uint32_t timestamp;
    CommandPayload *command;
    HeartbeatPayload *heartbeat;
};

/**
 * @brief Menghitung ukuran byte pesan terserialisasi.
 * @param message Pointer ke struktur C2Message.
 * @return size_t Ukuran dalam byte.
 */
size_t c2_message_get_packed_size(const C2Message *message);

/**
 * @brief Melakukan serialisasi (packing) struktur C2Message ke buffer biner.
 * @param message Pointer ke struktur C2Message.
 * @param out Buffer tujuan.
 * @return size_t Jumlah byte yang ditulis.
 */
size_t c2_message_pack(const C2Message *message, uint8_t *out);

/**
 * @brief Melakukan deserialisasi (unpacking) buffer biner ke struktur C2Message.
 * @param len Panjang data biner.
 * @param data Buffer sumber biner.
 * @return C2Message* Pointer ke struktur baru, atau NULL jika gagal.
 */
C2Message *c2_message_unpack(size_t len, const uint8_t *data);

/**
 * @brief Membebaskan memori struktur C2Message yang telah di-unpack.
 * @param message Pointer ke C2Message.
 */
void c2_message_free_unpacked(C2Message *message);

#endif // PROTOCOL_PB_C_H

