#include "MetricsCollector.hpp"
#include "SnapshotManager.hpp"
#include "Display.hpp"
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    MetricsCollector collector;
    SnapshotManager snapshot_mgr(85.0, 85.0); // Пороги по CPU и RAM (85%)

    std::cout << "Стартуем системный страж...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    while (true) {
        SystemMetrics metrics = collector.collect();
        
        Display::render(metrics);
        snapshot_mgr.check_and_save(metrics, collector);

        // Тик раз в секунду
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}