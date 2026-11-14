#include "Display.hpp"
#include <iostream>
#include <iomanip>

void Display::clear_screen() {
    std::cout << "\033[H\033[J";
}

void Display::render(const SystemMetrics& metrics) {
    clear_screen();
    
    std::cout << "Время: " << metrics.timestamp << "\n\n";

    // Строка прогресса CPU
    int cpu_bars = static_cast<int>(metrics.cpu_usage_percent / 5);
    if (cpu_bars > 20) cpu_bars = 20;
    std::string cpu_bar_str(cpu_bars, '|');
    std::string cpu_space_str(20 - cpu_bars, ' ');
    
    std::cout << "CPU: [" << cpu_bar_str << cpu_space_str << "] " 
              << std::fixed << std::setprecision(1) << metrics.cpu_usage_percent << "%\n";

    // Строка прогресса RAM
    int ram_bars = static_cast<int>(metrics.ram_usage_percent / 5);
    if (ram_bars > 20) ram_bars = 20;
    std::string ram_bar_str(ram_bars, '|');
    std::string ram_space_str(20 - ram_bars, ' ');

    std::cout << "RAM: [" << ram_bar_str << ram_space_str << "] " 
              << std::fixed << std::setprecision(1) << metrics.ram_usage_percent << "% "
              << "(" << (metrics.ram_used_kb / 1024) << "MB / " << (metrics.ram_total_kb / 1024) << "MB)\n\n";

    std::cout << "Load Average: " << metrics.load_avg_1m << ", " 
              << metrics.load_avg_5m << ", " << metrics.load_avg_15m << "\n";

}