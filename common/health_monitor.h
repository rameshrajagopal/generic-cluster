#ifndef __HEALTH_MONITOR_H_INCLUDED__
#define __HEALTH_MONITOR_H_INCLUDED__

#include <http2_client.h>
#include <tcp_client.h>

using health_monitor_cb = std::function<void (const HostInfo & host, Status status)>;

struct HostInfo {
    string name;
    string ip;
    int port;
};

typedef enum {
    STATUS_INVALID = 0x0,
    STATUS_NODE_UP = 0x1,
    STATUS_NODE_DOWN = 0x2,
    STATUS_SLAVE_UP  = 0x4,
    STATUS_SLAVE_DOWN = 0x8,
}Status;

typedef struct {
    Status status;
    bool   available;
}HostStatus;

class HealthMonitor {
public:
    HealthMonitor(const vector<HostInfo> & hosts, uint32_t timeout);
    HealthMonitor(const vector<HostInfo> & hosts, uint32_t timeout, health_monitor_cb cb);

    bool is_host_up(const HostInfo & host);
    void add(const HostInfo & host);
    void remove(const HostInfo & host); 
    void register_cb(health_monitor_cb cb);
    virtual void start() = 0;
protected:
    map<HostInfo, HostStatus> hostmap;
    mutex _mutex;
    uint32_t timeout;
    vecotor<health_monitor_cb> listeners;
};

class SlavePinger: HealthMonitor {
    // this will create a thread 
    void start() {
        while (wait_on_condition_with_timeout) {
            //create a Http2Client
            //request a status
            on_response() {
                if (there is a state change notify listeners) {
                    notify_listeners();
                }
            }
        }
    }
    void update_node_status(const HostInfo & host, Status status);
private:
    condition_variable _cond;
};

class NodePinger: HealthMonitor {
    void start() {
        while (wait_on_condition_with_timeout) {
            //make a fresh tcp connection
            //get a node status 
            if (change in node status && cb) {
                notify_listeners();
            }
        }
    }
private:
    condition_variable _cond
};


#endif /*__HEALTH_MONITOR_H_INCLUDED__*/
