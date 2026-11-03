#include "../../include/engine/ua_rotator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_UA_COUNT 100
#define MAX_UA_LENGTH 256

static char *ua_pool[MAX_UA_COUNT];
static int current_ua_count = 0;
static int initialized = 0;

// Daftar User-Agent bawaan (fallback jika file eksternal tidak ditemukan)
static const char *default_ua_list[] = {
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.2.1 Safari/605.1.15",
    "Mozilla/5.0 (X11; Linux x86_64; rv:123.0) Gecko/20100101 Firefox/123.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:123.0) Gecko/20100101 Firefox/123.0",
    "Mozilla/5.0 (iPhone; CPU iPhone OS 17_3_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148"
};

int ua_rotator_init(const char *filepath) {
    srand((unsigned int)time(NULL));
    current_ua_count = 0;

    // Coba buka file assets/user_agents.txt jika ada
    FILE *file = fopen(filepath, "r");
    if (file) {
        char buffer[MAX_UA_LENGTH];
        while (fgets(buffer, sizeof(buffer), file) && current_ua_count < MAX_UA_COUNT) {
            // Hapus karakter newline di akhir baris
            buffer[strcspn(buffer, "\r\n")] = 0;
            if (strlen(buffer) > 0) {
                ua_pool[current_ua_count] = strdup(buffer);
                current_ua_count++;
            }
        }
        fclose(file);
    }

    // Jika file kosong atau gagal dibaca, gunakan default fallback pool
    if (current_ua_count == 0) {
        int default_count = sizeof(default_ua_list) / sizeof(default_ua_list[0]);
        for (int i = 0; i < default_count && i < MAX_UA_COUNT; i++) {
            ua_pool[i] = strdup(default_ua_list[i]);
            current_ua_count++;
        }
    }

    initialized = 1;
    return current_ua_count;
}

void ua_rotator_get_random(char *dest, size_t max_len) {
    if (!initialized || current_ua_count == 0) {
        // Fallback aman jika belum diinisialisasi
        snprintf(dest, max_len, "%s", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
        return;
    }

    int index = rand() % current_ua_count;
    snprintf(dest, max_len, "%s", ua_pool[index]);
}

void ua_rotator_cleanup(void) {
    for (int i = 0; i < current_ua_count; i++) {
        if (ua_pool[i]) {
            free(ua_pool[i]);
            ua_pool[i] = NULL;
        }
    }
    current_ua_count = 0;
    initialized = 0;
}

