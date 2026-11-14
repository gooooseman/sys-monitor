#include "MetricsCollector.hpp"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <unistd.h>

namespace fs = std::filesystem;

MetricsCollector::MetricsCollector() {
    prev_cpu_times_ = read_cpu_stats();
}

MetricsCollector::CpuTimes MetricsCollector::read_cpu_stats() {
    std::ifstream stat_file("/proc/stat");
    std::string line;
    CpuTimes times{0, 0, 0, 0, 0, 0, 0, 0};
    
    if (stat_file.is_open() && std::getline(stat_file, line)) {
        std::istringstream ss(line);
        std::string cpu;
        ss >> cpu >> times.user >> times.nice >> times.system >> times.idle 
           >> times.iowait >> times.irq >> times.softirq >> times.steal;
    }
    return times;
}

double MetricsCollector::calculate_cpu_usage(const CpuTimes& prev, const CpuTimes& curr) {
    unsigned long long total_d = curr.total() - prev.total();
    unsigned long long active_d = curr.active() - prev.active();
    if (total_d == 0) return 0.0;
    return (static_cast<double>(active_d) / total_d) * 100.0;
}

std::vector<ProcessInfo> MetricsCollector::get_top_processes(size_t limit) {
    std::vector<ProcessInfo> processes;

    for (const auto& entry : fs::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;
        
        std::string filename = entry.path().filename().string();
        if (!std::all_of(filename.begin(), filename.end(), ::isdigit)) continue;

        int pid = std::stoi(filename);
        std::ifstream status_f(entry.path().string() + "/status");
        
        if (!status_f.is_open()) {
            std::cerr << "[WARN] Не смогли прочитать /proc/" << pid << "/status (уже отжил свое?)\n";
            continue;
        }

        std::string proc_name = "unknown";
        unsigned long long rss_kb = 0;
        std::string key;
        while (status_f >> key) {
            if (key == "Name:") status_f >> proc_name;
            if (key == "VmRSS:") status_f >> rss_kb;
            status_f.ignore(1000, '\n');
        }

        processes.push_back({pid, proc_name, rss_kb});
    }

    std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.memory_rss_kb > b.memory_rss_kb;
    });

    if (processes.size() > limit) {
        processes.resize(limit);
    }

    return processes;
}

SystemMetrics MetricsCollector::collect() {
    SystemMetrics metrics;

    // 1. Считаем CPU
    CpuTimes curr_cpu = read_cpu_stats();
    metrics.cpu_usage_percent = calculate_cpu_usage(prev_cpu_times_, curr_cpu);
    prev_cpu_times_ = curr_cpu;

    // 2. Читаем RAM из /proc/meminfo
    std::ifstream mem_file("/proc/meminfo");
    std::string key;
    unsigned long long total_kb = 0, avail_kb = 0;
    while (mem_file >> key) {
        if (key == "MemTotal:") mem_file >> total_kb;
        else if (key == "MemAvailable:") mem_file >> avail_kb;
        mem_file.ignore(1000, '\n');
    }
    
    metrics.ram_total_kb = total_kb;
    unsigned long long ram_used = (total_kb > avail_kb) ? (total_kb - avail_kb) : 0;
    metrics.ram_used_kb = ram_used;
    metrics.ram_usage_percent = total_kb > 0 ? (static_cast<double>(ram_used) / total_kb) * 100.0 : 0.0;

    // 3. Load Average
    std::ifstream load_file("/proc/loadavg");
    if (load_file.is_open()) {
        load_file >> metrics.load_avg_1m >> metrics.load_avg_5m >> metrics.load_avg_15m;
    }

    // 4. Время
    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    metrics.timestamp = buf;

    return metrics;
}