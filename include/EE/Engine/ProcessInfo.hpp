#pragma once

#include <cstddef>

namespace ee {

/// Gets the operating system's ID number of the calling thread
unsigned int get_thread_id();

/// Gets the CPU core usage as a percent used by all processes
double cpu_usage_total();

/// Gets the CPU core usage as a percent used by this process
double cpu_usage_process();

/// Gets the total virtual memory available in bytes
std::size_t virt_mem_available();

/// Gets the virtual memory used by all processes in bytes
std::size_t virt_mem_used_total();

/// Gets the virtual memory used by this process in bytes
std::size_t virt_mem_used_process();

/// Gets the total RAM available in bytes
std::size_t ram_available();

/// Gets the RAM used by all process in bytes
std::size_t ram_used_total();

/// Gets the RAM used by this process in bytes
std::size_t ram_used_process();

} // namespace ee
