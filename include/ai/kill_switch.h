#ifndef KILL_SWITCH_H
#зывают
#define KILL_SWITCH_H

#include <atomic>
#include <chrono>
#include <csignal>
#include <functional>

class KillSwitch {
private:
    std::atomic<bool> trip_triggered_;
    std::chrono::time_point<std::chrono::steady_clock> start_time_;
    uint64_t max_duration_seconds_;
    uint64_t max_packets_;
    std::atomic<uint64_t> current_packets_;
    std::function<void()> on_trip_callback_;

public:
    KillSwitch(uint64_t max_duration_sec = 0, uint64_t max_packets = 0)
        : trip_triggered_(false),
          max_duration_seconds_(max_duration_sec),
          max_packets_(max_packets),
          current_packets_(0) {
        start_time_ = std::chrono::steady_clock::now();
    }

    // Daftarkan sinyal sistem untuk penghentian darurat (Ctrl+C, dll)
    void registerSignals() {
        std::signal(SIGINT, signalHandlerStatic);
        std::signal(SIGTERM, signalHandlerStatic);
    }

    // Periksa apakah kriteria penghentian telah terpenuhi
    bool shouldStop() {
        if (trip_triggered_.load()) {
            return true;
        }

        // Cek batas waktu (jika diatur > 0)
        if (max_duration_seconds_ > 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time_).count();
            if (elapsed >= static_cast<int64_t>(max_duration_seconds_)) {
                trip("Batas waktu maksimal serangan tercapai.");
                return true;
            }
        }

        // Cek batas total paket (jika diatur > 0)
        if (max_packets_ > 0 && current_packets_.load() >= max_packets_) {
            trip("Batas total paket maksimal tercapai.");
            return true;
        }

        return false;
    }

    // Tambah counter paket yang telah dikirim
    void incrementPackets(uint64_t count = 1) {
        current_packets_.fetch_add(count, std::memory_order_relaxed);
    }

    // Picu penghentian secara manual
    void trip(const std::string& reason) {
        if (!trip_triggered_.exchange(true)) {
            // Eksekusi callback jika ada
            if (on_trip_callback_) {
                on_trip_callback_();
            }
        }
    }

    void setOnTripCallback(std::function<void()> cb) {
        on_trip_callback_ = cb;
    }

    bool isTripped() const {
        return trip_triggered_.load();
    }

private:
    static void signalHandlerStatic(int signal) {
        // Penanganan sinyal dasar untuk keluar aman
        exit(signal);
    }
};

#endif // KILL_SWITCH_H

