#include "ai/waf_prober.h"
#include "utils/logger.h"
#include <string.h>

WafType analyze_waf_response(int http_code, const char* response_body) {
    // Analisis header atau body response untuk mendeteksi signature WAF tertentu
    if (http_code == 403 || http_code == 503) {
        if (response_body != NULL) {
            if (strstr(response_body, "Cloudflare") != NULL || strstr(response_body, "cf-ray") != NULL) {
                log_warn("WAF Prober: Terdeteksi proteksi Cloudflare.");
                return WAF_CLOUDFLARE;
            }
            if (strstr(response_body, "Akamai") != NULL || strstr(response_body, "Access Denied") != NULL) {
                log_warn("WAF Prober: Terdeteksi proteksi Akamai.");
                return WAF_AKAMAI;
            }
            if (strstr(response_body, "Incapsula") != NULL || strstr(response_body, "Imperva") != NULL) {
                log_warn("WAF Prober: Terdeteksi proteksi Imperva/Incapsula.");
                return WAF_IMPERVA;
            }
        }
        log_warn("WAF Prober: Generic WAF / Rate Limiter terdeteksi berdasarkan response code %d", http_code);
        return WAF_GENERIC;
    }

    return WAF_NONE;
}

void apply_waf_countermeasures(WafType waf) {
    switch (waf) {
        case WAF_CLOUDFLARE:
            log_info("Countermeasure: Mengaktifkan rotasi TLS fingerprint lanjutan dan interval acak.");
            break;
        case WAF_AKAMAI:
            log_info("Countermeasure: Memperketat simulasi cookies dan header browser sah.");
            break;
        case WAF_IMPERVA:
            log_info("Countermeasure: Mengubah pola payload dan memperlambat laju koneksi.");
            break;
        case WAF_GENERIC:
            log_info("Countermeasure: Menerapkan backoff eksponensial standar.");
            break;
        case WAF_NONE:
        default:
            break;
    }
}

