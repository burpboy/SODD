#include "../../include/network/proxy_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

typedef struct {
    char ip[64];
    int port;
    char username[64];
    char password[64];
} ProxyNode;

static ProxyNode *proxy_pool = NULL;
static int proxy_count = 0;
static int current_index = 0;

int load_proxies_from_csv(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Gagal membuka file proxy CSV");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    // Hitung jumlah baris/proxy
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || strlen(line) < 3) continue; // Lewati komentar/baris kosong
        count++;
    }
    rewind(file);

    if (count == 0) {
        fclose(file);
        return 0;
    }

    proxy_pool = malloc(sizeof(ProxyNode) * count);
    if (!proxy_pool) {
        fclose(file);
        return -1;
    }

    proxy_count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || strlen(line) < 3) continue;

        // Asumsi format CSV: IP:Port:User:Pass
        char *token = strtok(line, ":\r\n");
        if (!token) continue;
        strncpy(proxy_pool[proxy_count].ip, token, sizeof(proxy_pool[proxy_count].ip) - 1);

        token = strtok(NULL, ":\r\n");
        if (!token) continue;
        proxy_pool[proxy_count].port = atoi(token);

        token = strtok(NULL, ":\r\n");
        if (token) {
            strncpy(proxy_pool[proxy_count].username, token, sizeof(proxy_pool[proxy_count].username) - 1);
        }

        token = strtok(NULL, ":\r\n");
        if (token) {
            strncpy(proxy_pool[proxy_count].password, token, sizeof(proxy_pool[proxy_count].password) - 1);
        }

        proxy_count++;
    }

    fclose(file);
    return proxy_count;
}

const char* get_next_proxy(void) {
    if (proxy_count == 0) return NULL;

    static char proxy_str[256];
    ProxyNode *p = &proxy_pool[current_index];
    
    if (strlen(p->username) > 0) {
        snprintf(proxy_str, sizeof(proxy_str), "%s:%s@%s:%d", p->username, p->password, p->ip, p->port);
    } else {
        snprintf(proxy_str, sizeof(proxy_str), "%s:%d", p->ip, p->port);
    }

    // Rotasi Round-Robin
    current_index = (current_index + 1) % proxy_count;
    return proxy_str;
}

void free_proxy_manager(void) {
    if (proxy_pool) {
        free(proxy_pool);
        proxy_pool = NULL;
        proxy_count = 0;
        current_index = 0;
    }
}

