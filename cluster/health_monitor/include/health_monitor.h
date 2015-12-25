#ifndef __HEALTH_MONITOR_H_INCLUDED__
#define __HEALTH_MONITOR_H_INCLUDED__

#include <common_types.h>
#include <atomic>
#include <host_info.h>

using health_monitor_cb = std::function<void (const HostInfo & host, Status status)>;

class HealthMonitor {
public:
    HealthMonitor(const vector<HostInfo> & hosts, uint32_t timeout, health_monitor_cb cb);
    bool is_host_up(HostInfo host);
    void add(const HostInfo & host);
    void remove(const HostInfo & host); 
    void register_listener(health_monitor_cb cb);
    void start();
    void wakeup_monitor();
private:
    std::atomic<bool> is_alive {true};
    std::map<string, HostInfo> hostmap;
    std::mutex _mutex;
    bool wakeup {false};
    std::condition_variable _cond;
    uint32_t timeout;
    vector<health_monitor_cb> listeners;
};

#endif /*__HEALTH_MONITOR_H_INCLUDED__*/
