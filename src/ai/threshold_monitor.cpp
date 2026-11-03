#include "ai/threshold_monitor.h"
#include "utils/logger.h"
#include <stdlib.h>

static int current_rate_limit = 50; // Requests per second default
static int min_rate = 10;
static int max_rate = 200;

void init_threshold_monitor(int initial_rate) {
    current_rate_limit = initial_rate;
    log_info("Threshold Monitor diinisialisasi dengan base rate: %d RPS", current_rate_limit);
}

int adjust_threshold(int success_rate, int block_rate) {
    // Jika block rate (error 403/503) tinggi, turunkan kecepatan
    if (block_rate > 20) {
        current_rate_limit -= 15;
        if (current_rate_limit < min_rate) {
            current_rate_limit = min_rate;
        }
        log_warn("Threshold Monitor: Block rate tinggi (%d%%). Menurunkan RPS ke %d", block_rate, current_rate_limit);
    } 
    // Jika success rate tinggi dan block rate rendah, naikkan kecepatan secara bertahap
    else if (success_rate > 80 && block_rate < 5) {
        current_rate_limit += 10;
        if (current_rate_limit > max_rate) {
            current_rate_limit = max_rate;
        }
        log_info("Threshold Monitor: Kondisi optimal. Meningkatkan RPS ke %d", current_rate_limit);
    }

    return current_rate_limit;
}

int get_current_threshold(void) {
    return current_rate_limit;
}

