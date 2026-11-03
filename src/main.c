#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Mengimpor header utama yang mendefinisikan NodeMode secara global
#include "../include/main.h"

// Fungsi untuk menampilkan panduan penggunaan CLI
static void print_usage(const char *prog_name) {
    printf("=== DDoS Botnet Architecture ===\n");
    printf("Penggunaan: %s --mode <master|relay|bot> [opsi]\n\n", prog_name);
    printf("Pilihan Mode:\n");
    printf("  master : Menjalankan node sebagai pengendali pusat C2 (Master)\n");
    printf("  relay  : Menjalankan node sebagai perantara/relay komunikasi\n");
    printf("  bot    : Menjalankan node sebagai worker instance (Bot)\n");
}

int main(int argc, char *argv[]) {
    // Validasi argumen minimum
    if (argc < 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    NodeMode current_mode = MODE_UNKNOWN;

    // Parsing argumen command line untuk menentukan mode
    if (strcmp(argv[1], "--mode") == 0) {
        if (strcmp(argv[2], "master") == 0) {
            current_mode = MODE_MASTER;
        } else if (strcmp(argv[2], "relay") == 0) {
            current_mode = MODE_RELAY;
        } else if (strcmp(argv[2], "bot") == 0) {
            current_mode = MODE_BOT;
        }
    }

    // Logika Switch-Case Utama Berdasarkan Mode
    switch (current_mode) {
        case MODE_MASTER:
            printf("[*] Menginisialisasi sistem dalam mode: MASTER\n");
            // TODO: Panggil fungsi inisialisasi master, muat config master.yaml, 
            // jalankan c2_server, dan threshold_monitor.
            break;

        case MODE_RELAY:
            printf("[*] Menginisialisasi sistem dalam mode: RELAY\n");
            // TODO: Panggil fungsi inisialisasi relay, muat config relay.yaml,
            // dan siapkan penerusan port (port forwarding / proxy handler).
            break;

        case MODE_BOT:
            printf("[*] Menginisialisasi sistem dalam mode: BOT\n");
            // TODO: Panggil fungsi inisialisasi bot, muat config bot.yaml,
            // hubungkan c2_client ke master/relay, dan aktifkan heartbeat.
            break;

        case MODE_UNKNOWN:
        default:
            fprintf(stderr, "[!] Error: Mode tidak valid atau argumen salah.\n\n");
            print_usage(argv[0]);
            return EXIT_FAILURE;
    }

    printf("[+] Program berjalan stabil pada mode yang dipilih.\n");
    return EXIT_SUCCESS;
}
