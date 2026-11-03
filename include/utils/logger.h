/**
 * @file logger.h
 * @brief Logging terstruktur berbasis JSON untuk monitoring runtime node.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Menginisialisasi sistem logger (file output & tingkat minimum).
 * 
 * @param log_filepath Path file log output (NULL untuk stdout saja).
 * @param min_level Batas minimum level log yang dicatat.
 * @return true Jika inisialisasi berhasil.
 * @return false Jika gagal membuka file log.
 */
bool logger_init(const char *log_filepath, LogLevel min_level);

/**
 * @brief Menulis log terstruktur berformat JSON.
 * 
 * @param level Level log (DEBUG, INFO, WARN, ERROR, FATAL).
 * @param module Nama modul yang memanggil (misal: "c2_client", "http_engine").
 * @param event Deskripsi singkat event.
 * @param fmt Format string tambahan untuk argumen variadic (opsional, printf style).
 */
void logger_log_json(LogLevel level, const char *module, const char *event, const char *fmt, ...);

/**
 * @brief Menutup file handler logger dan membersihkan resource.
 */
void logger_close(void);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_H */
