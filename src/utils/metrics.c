/**
 * @file metrics.c
 * @brief Performance metrics accumulator (requests/sec, success/fail counts).
 */

#include "utils/metrics.h"
#include <pthread.h>
#include <time.h>
#include <string.h>

static Metrics g_metrics;
static pthread_mutex_t g_metrics_mutex = PTHREAD_MUTEX_INITIALIZER;

void metrics_init(void) {
    pthread_mutex_lock(&g_metrics_mutex);
    memset(&g_metrics, 0, sizeof(Metrics));
    g_metrics.start_time = time(NULL);
    pthread_mutex_unlock(&g_metrics_mutex);
}

void metrics_increment_requests(void) {
    pthread_mutex_lock(&g_metrics_mutex);
    g_metrics.total_requests++;
    g_metrics.current_window_requests++;
    pthread_mutex_unlock(&g_metrics_mutex);
}

void metrics_increment_success(void) {
    pthread_mutex_lock(&g_metrics_mutex);
    g_metrics.success_requests++;
    pthread_mutex_unlock(&g_metrics_mutex);
}

void metrics_increment_failure(void) {
    pthread_mutex_lock(&g_metrics_mutex);
    g_metrics.failed_requests++;
    pthread_mutex_unlock(&g_metrics_mutex);
}

double metrics_get_req_per_sec(void) {
    pthread_mutex_lock(&g_metrics_mutex);
    time_t now = time(NULL);
    double elapsed = (double)(now - g_metrics.start_time);
    if (elapsed <= 0.0) {
        pthread_mutex_unlock(&g_metrics_mutex);
        return 0.0;
    }
    double rps = (double)g_metrics.total_requests / elapsed;
    pthread_mutex_unlock(&g_metrics_mutex);
    return rps;
}

void metrics_get_snapshot(Metrics *out_metrics) {
    if (!out_metrics) return;
    pthread_mutex_lock(&g_metrics_mutex);
    memcpy(out_metrics, &g_metrics, sizeof(Metrics));
    pthread_mutex_unlock(&g_metrics_mutex);
}

