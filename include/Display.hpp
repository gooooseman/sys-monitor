#pragma once
#include "MetricsCollector.hpp"

class Display {
public:
    static void clear_screen();
    static void render(const SystemMetrics& metrics);
};