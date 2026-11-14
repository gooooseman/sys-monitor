#include "SnapshotManager.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

SnapshotManager::SnapshotManager(double cpu_threshold, double ram_threshold) 
    : cpu_threshold_(cpu_threshold), ram_threshold_(ram_threshold) {}

void SnapshotManager::check_and_save(SystemMetrics& metrics, MetricsCollector& collector) {
    if (metrics.cpu_usage_percent >= cpu_threshold_ || metrics.ram_usage_percent >= ram_threshold_) {
        metrics.top_processes = collector.get_top_processes(10);
        save_to_json(metrics);
    }
}

void SnapshotManager::save_to_json(const SystemMetrics& metrics) {
    std::string dir = "/app/snapshots";
    std::filesystem::create_directories(dir);

    // Получаем текущее время в читаемом формате для имени файла
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    
    // Формируем имя файла: snapshot_YYYY-MM-DD_HH-MM-SS.json
    std::string filename = dir + "/snapshot_" + ss.str() + ".json";
    std::ofstream out(filename);

    if (out.is_open()) {
        out << "{\n";
        out << "  \"timestamp\": \"" << metrics.timestamp << "\",\n";
        out << "  \"event\": \"OVERLOAD_TRIGGERED\",\n";
        out << "  \"cpu_usage_percent\": " << metrics.cpu_usage_percent << ",\n";
        out << "  \"ram_usage_percent\": " << metrics.ram_usage_percent << ",\n";
        out << "  \"top_processes\": [\n";
        
        for (size_t i = 0; i < metrics.top_processes.size(); ++i) {
            const auto& p = metrics.top_processes[i];
            out << "    {\n";
            out << "      \"pid\": " << p.pid << ",\n";
            out << "      \"name\": \"" << p.name << "\",\n";
            out << "      \"rss_mb\": " << p.memory_rss_kb / 1024 << "\n";
            out << "    }" << (i + 1 < metrics.top_processes.size() ? "," : "") << "\n";
        }
        
        out << "  ]\n";
        out << "}\n";
        
        std::cout << "\n\033[1;31m[!] СНАПШОТ СОХРАНЕН: " << filename << "\033[0m\n";
    } else {
        std::cerr << "\n\033[1;31m[!] ОШИБКА: Не удалось открыть файл для записи: " << filename << "\033[0m\n";
    }
}