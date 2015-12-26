#ifndef __HEALTH_MONITOR_H_INCLUDED__
#define __HEALTH_MONITOR_H_INCLUDED__

#include <common_types.h>
#include <atomic>
#include <host_info.h>

using health_monitor_cb = std::function<void (const unique_ptr<HostInfo> & host, Status status)>;

class HealthMonitor {
public:
    HealthMonitor(const vector<HostInfo> & hosts, uint32_t timeout, health_monitor_cb cb);
    bool is_host_up(const HostInfo & host);
    void add(const HostInfo & host);
    int remove(const HostInfo & host); 
    void register_listener(health_monitor_cb cb);
    void start();
    void stop();
    void wakeup_monitor();
private:
    uint32_t timeout;
    bool wakeup {false};
    std::atomic<bool> is_alive {true};
    vector<health_monitor_cb> listeners;
    std::map<string, unique_ptr<HostInfo>> hostmap;
    std::mutex _mutex;
    std::condition_variable _cond;
};

#endif /*__HEALTH_MONITOR_H_INCLUDED__*/
