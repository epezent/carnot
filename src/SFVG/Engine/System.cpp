#include <SFVG/Engine/System.hpp>
#include <cstring>
#include <ctime>
#include <iomanip>

#ifdef _WIN32
#include <pdh.h>
#include <psapi.h>
#include <tchar.h>
#include <windows.h>
#else
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#endif

namespace sfvg {

unsigned int get_thread_id() {
    #ifdef _WIN32
    return GetCurrentThreadId();
    #elif defined(__linux__)
    return static_cast<unsigned int>(::syscall(__NR_gettid));
    #elif defined(__APPLE__)
    uint64_t tid64;
    pthread_threadid_np(NULL, &tid64);
    return static_cast<unsigned int>(tid64);
    #endif
}

#ifdef _WIN32

// for CPU usage total
static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

// for CPU usage process
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

// We need to initialize a few things for Windows functions, so we create a
// simple class with the the init code in its constructor and create an isntance
struct PerformanceInitializer {
    PerformanceInitializer() {
        // for CPU usage total
        PdhOpenQuery(0, 0, &cpuQuery);
        PdhAddCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", 0, &cpuTotal);
        PdhCollectQueryData(cpuQuery);
        // for CPU usage process
        SYSTEM_INFO sysInfo;
        FILETIME ftime, fsys, fuser;

        GetSystemInfo(&sysInfo);
        numProcessors = sysInfo.dwNumberOfProcessors;

        GetSystemTimeAsFileTime(&ftime);
        memcpy(&lastCPU, &ftime, sizeof(FILETIME));

        self = GetCurrentProcess();
        GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
        memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
        memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
    }
    ~PerformanceInitializer() { }
};

// create an instance, calls the init code in constructor
PerformanceInitializer global_initializer;

double cpu_usage_total() {
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;
}

double cpu_usage_process() {
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (double)((sys.QuadPart - lastSysCPU.QuadPart) +
        (user.QuadPart - lastUserCPU.QuadPart));
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}

std::size_t virt_mem_available() {
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPageFile;
}

std::size_t virt_mem_used_total() {
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPageFile - mem_info.ullAvailPageFile;
}

std::size_t virt_mem_used_process() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.PrivateUsage;
}

std::size_t ram_available() {
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPhys;
}

std::size_t ram_used_total() {
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPhys - mem_info.ullAvailPhys;
}

std::size_t ram_used_process() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

#else

double cpu_usage_total() {
    return 0.0;
}

double cpu_usage_process() {
    return 0.0;
}

std::size_t virt_mem_available() {
    return 0;
}

std::size_t virt_mem_used_total() {
    return 0;
}

std::size_t virt_mem_used_process() {
    return 0;
}

std::size_t ram_available() {
    return 0;
}

std::size_t ram_used_total() {
    return 0;
}

std::size_t ram_used_process() {
    return 0;
}

#endif

} // namespace sfvg
