/**
 * @file logger.c
 * @brief Implementasi formatting log ke file (JSON) dan stdout/stderr dengan thread-safety.
 */

#include "utils/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

static FILE *g_log_file = NULL;
static LogLevel g_min_level = LOG_INFO;
static pthread_mutex_t g_log_mutex = PTHREAD_MUTEX_INITIALIZER;

static const char* log_level_strings[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

int logger_init(const char *filepath, LogLevel min_level) {
    pthread_mutex_lock(&g_log_mutex);
    g_min_level = min_level;
    
    if (filepath != NULL && strlen(filepath) > 0) {
        g_log_file = fopen(filepath, "a");
        if (g_log_file == NULL) {
            pthread_mutex_unlock(&g_log_mutex);
            return -1;
        }
    }
    pthread_mutex_unlock(&g_log_mutex);
    return 0;
}

void logger_set_level(LogLevel level) {
    pthread_mutex_lock(&g_log_mutex);
    g_min_level = level;
    pthread_mutex_unlock(&g_log_mutex);
}

void logger_log(LogLevel level, const char *file, int line, const char *fmt, ...) {
    if (level < g_min_level) {
        return;
    }

    time_t now = time(NULL);
    struct tm *tinfo = localtime(&now);
    char time_buf[64];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tinfo);

    pthread_mutex_lock(&g_log_mutex);

    // Tentukan stream output (stdout untuk info/debug, stderr untuk error ke atas)
    FILE *out = (level >= LOG_ERROR) ? stderr : stdout;
    
    // Format human-readable untuk console
    fprintf(out, "[%s] [%s] [%s:%d] ", time_buf, log_level_strings[level], file, line);
    
    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);
    
    fprintf(out, "\n");
    fflush(out);

    // Format terstruktur (JSON) jika log file diaktifkan
    if (g_log_file != NULL) {
        fprintf(g_log_file, "{\"timestamp\":\"%s\",\"level\":\"%s\",\"file\":\"%s\",\"line\":%d,\"message\":\"", 
                time_buf, log_level_strings[level], file, line);
        
        va_start(args, fmt);
        vfprintf(g_log_file, fmt, args);
        va_end(args);
        
        fprintf(g_log_file, "\"}\n");
        fflush(g_log_file);
    }

    pthread_mutex_unlock(&g_log_mutex);
}

void logger_close(void) {
    pthread_mutex_lock(&g_log_mutex);
    if (g_log_file != NULL) {
        fclose(g_log_file);
        g_log_file = NULL;
    }
    pthread_mutex_unlock(&g_log_mutex);
}

