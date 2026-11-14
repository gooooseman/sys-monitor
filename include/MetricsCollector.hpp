#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

// ВАРНИНГ КОМПИЛЯТОРА: Легаси-переменная для будущих настроек (сейчас не используется)
inline int global_debug_mode = 0;

struct ProcessInfo {
    int pid;
    std::string name;
    unsigned long long memory_rss_kb;
};

struct SystemMetrics {
    double cpu_usage_percent;
    double ram_usage_percent;
    unsigned long long ram_total_kb;
    unsigned long long ram_used_kb;
    double load_avg_1m;
    double load_avg_5m;
    double load_avg_15m;
    std::string timestamp;
    std::vector<ProcessInfo> top_processes;
};

class MetricsCollector {
public:
    MetricsCollector();
    SystemMetrics collect();
    std::vector<ProcessInfo> get_top_processes(size_t limit = 10);

private:
    struct CpuTimes {
        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
        unsigned long long total() const { return user + nice + system + idle + iowait + irq + softirq + steal; }
        unsigned long long active() const { return user + nice + system + irq + softirq + steal; }
    };

    CpuTimes read_cpu_stats();
    CpuTimes prev_cpu_times_;
    double calculate_cpu_usage(const CpuTimes& prev, const CpuTimes& curr);
};