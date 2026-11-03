#ifndef THRESHOLD_MONITOR_H
#define THRESHOLD_MONITOR_H

#include <chrono>
#include <atomic>
#include <mutex>

class ThresholdMonitor {
private:
    std::atomic<uint64_t> request_count_;
    std::atomic<uint64_t> error_count_;
    uint64_t max_error_rate_percent_;
    std::chrono::time_point<std::chrono::steady_clock> window_start_;
    std::mutex monitor_mutex_;

public:
    ThresholdMonitor(uint64_t max_error_rate = 50)
        : request_count_(0),
          error_count_(0),
          max_error_rate_percent_(max_error_rate) {
        window_start_ = std::chrono::steady_clock::now();
    }

    void recordRequest(bool is_error = false) {
        request_count_.fetch_add(1, std::memory_order_relaxed);
        if (is_error) {
            error_count_.fetch_add(1, std::memory_order_relaxed);
        }
    }

    // Evaluasi apakah tingkat error melampaui batas aman (indikasi WAF memblokir total)
    bool isThresholdExceeded() {
        std::lock_guard<std::mutex> lock(monitor_mutex_);
        uint64_t reqs = request_count_.load();
        uint64_t errs = error_count_.load();

        if (reqs < 100) {
            return false; // Sampel terlalu kecil
        }

        uint64_t error_rate = (errs * 100) / reqs;

        // Reset window berkala
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - window_start_).count();
        if (elapsed >= 10) { // Reset setiap 10 detik
            request_count_.store(0);
            error_count_.store(0);
            window_start_ = now;
        }

        return error_rate >= max_error_rate_percent_;
    }

    void reset() {
        request_count_.store(0);
        error_count_.store(0);
        window_start_ = std::chrono::steady_clock::now();
    }
};

#endif // THRESHOLD_MONITOR_H

