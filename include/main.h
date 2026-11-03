/**
 * @file main.h
 * @brief Definisi mode operasi utama untuk arsitektur Botnet (MASTER, RELAY, BOT).
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * @enum NodeMode
 * @brief Enumerasi untuk mendefinisikan peran node saat runtime.
 */
typedef enum {
    MODE_UNKNOWN = 0,   /**< Mode belum ditentukan atau invalid */
    MODE_MASTER  = 1,   /**< Node pengendali pusat (C2 Master Controller) */
    MODE_RELAY   = 2,   /**< Node perantara / proxy penerus instruksi (Relay Node) */
    MODE_BOT     = 3    /**< Node pekerja / eksekutor serangan (Bot Instance) */
} NodeMode;

/**
 * @struct AppConfig
 * @brief Struktur konfigurasi global aplikasi yang dibaca saat inisialisasi.
 */
typedef struct {
    NodeMode mode;
    char config_path[256];
    int verbose_level;
    int max_threads;
} AppConfig;

/**
 * @brief Mendapatkan representasi string dari mode node saat ini.
 * @param mode Enumerasi NodeMode.
 * @return String nama mode.
 */
const char* get_mode_string(NodeMode mode);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */

