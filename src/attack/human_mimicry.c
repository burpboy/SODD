/*
 * human_mimicry.c - Simulasi delay & variasi parameter manusia
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../../include/attack/human_mimicry.h"

// Menghasilkan jeda waktu acak menyerupai waktu jeda interaksi manusia (dalam milidetik)
void human_mimicry_random_sleep(int min_ms, int max_ms) {
    if (max_ms <= min_ms) {
        usleep(min_ms * 1000);
        return;
    }
    
    int sleep_ms = min_ms + (rand() % (max_ms - min_ms + 1));
    usleep(sleep_ms * 1000);
}

// Menghasilkan variasi parameter kueri acak untuk mencegah caching WAF
void human_mimicry_generate_random_params(char *buffer, size_t max_len) {
    const char *keys[] = {"session", "token", "click_id", "ref", "timestamp", "v"};
    int r_key = rand() % 6;
    long r_val = rand() % 99999999;
    
    snprintf(buffer, max_len, "?%s=%ld&rand_track=%d", keys[r_key], r_val, rand() % 100);
}
