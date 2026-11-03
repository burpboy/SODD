#include "ai/kill_switch.h"
#include "utils/logger.h"
#include <unistd.h>
#include <time.h>

static int consecutive_errors = 0;
static time_t pause_until = 0;

int check_kill_switch(int http_code) {
    time_t now = time(NULL);
    
    // Jika sedang dalam masa pause akibat trigger kill switch
    if (now < pause_until) {
        return 1; // Indikasi bahwa sistem sedang dipause
    }

    // Deteksi HTTP 503, 502, atau 429 yang menandakan pembatasan WAF / overload
    if (http_code == 503 || http_code == 502 || http_code == 429) {
        consecutive_errors++;
        log_warn("Kill Switch: Terdeteksi response code pembatasan: %d (Akumulasi: %d)", http_code, consecutive_errors);
        
        // Jika error beruntun mencapai threshold (misal: 5 kali), aktifkan cooldown
        if (consecutive_errors >= 5) {
            pause_until = now + 30; // Pause selama 30 detik
            log_error("Kill Switch TRIGGERED! Target memblokir atau kewalahan. Jeda serangan selama 30 detik.");
            consecutive_errors = 0;
            return 1;
        }
    } else if (http_code >= 200 && http_code < 400) {
        // Reset akumulasi error jika request berhasil
        if (consecutive_errors > 0) {
            consecutive_errors--;
        }
    }

    return 0;
}

