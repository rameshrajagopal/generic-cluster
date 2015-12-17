#ifndef __MASTER_H_INCLUDED__
#define __MASTER_H_INCLUDED__

#include <health_monitor.h>
#include <host_info.h>
#include <host_session.h>

class Master {
public:
    Master(const HostInfo & master, vector<HostInfo> & slaves);
    void init();
    void slave_register(const HostInfo & slave);
    void slave_set_state(const HostInfo & slave, Status status);
    /* peer is master secondary */
    void peer_register(const HostInfo & slave);
    void peer_set_state(const HostInfo & peer, Status status);
    bool get_status(); 
    void submit_response(shared_ptr<Handle> handle, int res_code, Buffer & response);
    void submit_slave_request(shared_ptr<Handle> handle, const string & uri, const Buffer & request, response_dispatch_cb res_cb);
    void host_monitor_cb(const HostInfo & host, Status status);
    void register_request_cb(shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap &h);
private:
    Http2Server server;
    vector<HostSession> slave_sessions;
    vector<HostSession> peer_sessions;
    HealthMonitor health_monitor;
};



#endif /*__MASTER_H_INCLUDED__*/
