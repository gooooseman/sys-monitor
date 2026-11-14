#pragma once
#include "MetricsCollector.hpp"
#include <string>

class SnapshotManager {
public:
    SnapshotManager(double cpu_threshold = 85.0, double ram_threshold = 85.0);
    void check_and_save(SystemMetrics& metrics, MetricsCollector& collector);

private:
    double cpu_threshold_;
    double ram_threshold_;
    void save_to_json(const SystemMetrics& metrics);
};