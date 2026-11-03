#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../../include/core/c2_server.h"

#define PORT 8080
#define BUFFER_SIZE 1024

// Fungsi untuk memparsing data yang masuk dari client/bot
void parse_incoming_data(const char *data, size_t length) {
    printf("[C2 Server] Memproses data sepanjang %zu byte...\n", length);
    
    // Contoh parsing sederhana berdasarkan isi payload
    if (strncmp(data, "PING", 4) == 0) {
        printf("[C2 Server] Diterima paket PING (Health Check) dari node.\n");
    } else {
        printf("[C2 Server] Payload diterima: %.*s\n", (int)length, data);
    }
}

// Fungsi utama C2 Server untuk listen dan accept koneksi
void start_c2_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Membuat socket descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Gagal membuat socket");
        exit(EXIT_FAILURE);
    }

    // Melampirkan socket ke port yang ditentukan
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Gagal set socket options");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding socket ke alamat dan port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Binding gagal");
        exit(EXIT_FAILURE);
    }

    // Mulai mendengarkan koneksi masuk
    if (listen(server_fd, 10) < 0) {
        perror("Listen gagal");
        exit(EXIT_FAILURE);
    }

    printf("[C2 Server] Berjalan dan mendengarkan pada port %d...\n", PORT);

    // Loop utama untuk menerima koneksi masuk
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Gagal menerima koneksi (accept)");
            continue;
        }

        printf("[C2 Server] Koneksi diterima dari IP: %s\n", inet_ntoa(address.sin_addr));

        memset(buffer, 0, BUFFER_SIZE);
        ssize_t valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread > 0) {
            parse_incoming_data(buffer, valread);
            
            // Mengirim balasan konfirmasi ke client
            char *response = "ACK: Data received";
            send(new_socket, response, strlen(response), 0);
        }

        close(new_socket);
    }
}

