#ifndef __HEALTH_MONITOR_H_INCLUDED__
#define __HEALTH_MONITOR_H_INCLUDED__

#include <host_info.h>
#include <http2_client.h>

using health_monitor_cb = std::function<void (const HostInfo & host, Status status)>;

typedef struct {
    Status status;
    bool   available;
}HostStatus;

class HealthMonitor {
public:
    HealthMonitor(const vector<HostInfo> & hosts, uint32_t timeout, health_monitor_cb cb);

    bool is_host_up(const HostInfo & host);
    void add(const HostInfo & host);
    void remove(const HostInfo & host); 
    void register_listener(health_monitor_cb cb);
    void start() = 0;
protected:
    map<HostInfo, HostStatus> hostmap;
    mutex _mutex;
    condition_variable _cond;
    uint32_t timeout;
    vecotor<health_monitor_cb> listeners;
};

#endif /*__HEALTH_MONITOR_H_INCLUDED__*/
