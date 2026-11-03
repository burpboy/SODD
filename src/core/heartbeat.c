#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include "../../include/core/heartbeat.h"

#define C2_SERVER_IP "127.0.0.1"
#define C2_SERVER_PORT 8080
#define HEARTBEAT_INTERVAL 10 // Interval waktu dalam detik

// Fungsi untuk mengirimkan satu siklus heartbeat ke C2 Server
void send_heartbeat() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *heartbeat_msg = "PING: STATUS_OK";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[Heartbeat] Gagal membuat socket");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(C2_SERVER_PORT);

    // Konversi alamat IP C2 Server
    if (inet_pton(AF_INET, C2_SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("[Heartbeat] Alamat IP tidak valid / tidak didukung");
        close(sock);
        return;
    }

    // Melakukan koneksi ke C2 Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("[Heartbeat] Koneksi ke C2 Server gagal (Server tidak aktif)");
        close(sock);
        return;
    }

    // Mengirim payload health check
    send(sock, heartbeat_msg, strlen(heartbeat_msg), 0);
    printf("[Heartbeat] Health check dikirim ke %s:%d\n", C2_SERVER_IP, C2_SERVER_PORT);

    // Membaca respon balik dari C2 Server
    ssize_t valread = read(sock, buffer, 1024);
    if (valread > 0) {
        printf("[Heartbeat] Respon dari C2: %s\n", buffer);
    }

    close(sock);
}

// Fungsi pengatur timer untuk menjalankan heartbeat secara terus-menerus
void start_heartbeat_timer() {
    printf("[Heartbeat] Timer diaktifkan. Mengirim health check setiap %d detik...\n", HEARTBEAT_INTERVAL);
    
    while (1) {
        send_heartbeat();
        sleep(HEARTBEAT_INTERVAL); // Menunda eksekusi sesuai interval yang ditentukan
    }
}

