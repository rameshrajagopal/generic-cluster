#ifndef __MASTER_H_INCLUDED__
#define __MASTER_H_INCLUDED__

class MasterIf {
public:
    MasterIf(const HostInfo & master, vector<HostInfo> & slaves);
    bool get_host_status(const HostInfo & host);
    void add_host(const HostInfo & host);
    void remove_host(const HostInfo & host); 
    void register_node_listener(health_monitor_cb cb);
    void register_slave_listener(health_monitor_cb cb);
    void init() {
        //will start the node pinger 
        //will start the slave pinger
    }
    virtual bool get_status(); 
protected:
    NodePinger  node_pinger;
    SlavePinger slave_pinger;
    vector<HostInfo> _slaves;
};

class Master: MasterIf {
    Master(const HostInfo & master, vector<HostInfo> & slaves);
    void start() {
    }
    bool get_status() {
        //incase of sherlock elb, we can start the listener on this call 
        //to make the service is available
    }
private:
    void node_status_monitor(const HostInfo & host, HostStatus & status);
};



#endif /*__MASTER_H_INCLUDED__*/
