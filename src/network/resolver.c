#include "../../include/network/resolver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

int custom_resolve_dns(const char *hostname, char *ip_output, size_t max_len) {
    struct addrinfo hints, *res = NULL, *p = NULL;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;     // Fokus ke IPv4 terlebih dahulu
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, NULL, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "DNS Resolution Error untuk %s: %s\n", hostname, gai_strerror(status));
        return -1;
    }

    int success = -1;
    for (p = res; p != NULL; p = p->ai_next) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        void *addr = &(ipv4->sin_addr);

        if (inet_ntop(p->ai_family, addr, ip_output, max_len) != NULL) {
            success = 0;
            break; // Ambil IP pertama yang valid
        }
    }

    freeaddrinfo(res);
    return success;
}

