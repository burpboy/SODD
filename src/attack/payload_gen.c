/*
 * payload_gen.c - Generator string, JSON, dan XML acak
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../include/attack/payload_gen.h"

// Generator string acak dengan panjang tertentu
void payload_gen_random_string(char *dest, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-.";
    for (size_t i = 0; i < length - 1; i++) {
        int key = rand() % (sizeof(charset) - 1);
        dest[i] = charset[key];
    }
    dest[length - 1] = '\0';
}

// Generator payload berformat JSON dinamis
void payload_gen_json(char *buffer, size_t max_len) {
    char rand_str[16];
    payload_gen_random_string(rand_str, sizeof(rand_str));
    
    snprintf(buffer, max_len, 
             "{\"user_id\": %d, \"action\": \"query\", \"signature\": \"%s\", \"timestamp\": %ld}",
             rand() % 10000, rand_str, time(NULL));
}

// Generator payload berformat XML dinamis
void payload_gen_xml(char *buffer, size_t max_len) {
    char rand_str[16];
    payload_gen_random_string(rand_str, sizeof(rand_str));
    
    snprintf(buffer, max_len, 
             "<request><id>%d</id><data>%s</data><status>active</status></request>",
             rand() % 10000, rand_str);
}
