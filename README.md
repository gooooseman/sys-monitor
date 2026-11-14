================================================================================
                        C++ SYSTEM TELEMETRY MONITOR
================================================================================

Low-level system telemetry generator written in C++, interacting directly 
with the Linux kernel virtual filesystems (/proc and /sys) to detect hardware 
anomalies and generate automated JSON state snapshots during performance 
degradation.


[ CORE FEATURES ]
--------------------------------------------------------------------------------
Direct VFS Parsing
  Reads metrics straight from /proc (CPU stats, memory, load average) and /sys 
  (thermal zones, hardware sensors) without external agent overhead.

Anomaly Detection
  Tracks system metrics in real-time and flags resource spikes.

JSON State Snapshots
  Automatically dumps structured telemetry payloads during performance 
  degradation for external analysis.

Linux Kernel Integration
  Built specifically around Linux process and hardware interfaces.


[ TECH STACK ]
--------------------------------------------------------------------------------
Language          : C++
System Interfaces : Linux VFS (/proc, /sys), POSIX APIs
Build System      : CMake
Version Control   : Git, GitHub SSH


[ REPOSITORY STRUCTURE ]
--------------------------------------------------------------------------------
sys-monitor/
├── include/           Header files
├── src/               Implementation source files
├── CMakeLists.txt     CMake build configuration
└── README.md          Project documentation


[ BUILDING AND RUNNING ]
--------------------------------------------------------------------------------
Prerequisites
  Linux environment (Arch Linux / EndeavourOS / Ubuntu)
  CMake 3.15+
  C++17 compliant compiler (GCC / Clang)

Compilation
  git clone git@github.com:gooooseman/sys-monitor.git
  cd sys-monitor
  mkdir build && cd build
  cmake ..
  make

Execution
  Run the binary with permissions required to read kernel virtual filesystems:
  ./sys-monitor


[ AUTHOR ]
--------------------------------------------------------------------------------
Vladimir Khabibullin
GitHub : @gooooseman (https://github.com/gooooseman)
================================================================================
