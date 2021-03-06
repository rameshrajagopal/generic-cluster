#ifndef __MASTER_H_INCLUDED__
#define __MASTER_H_INCLUDED__

#include <health_monitor.h>
#include <host_info.h>
#include <buffer.h>
#include <request_meta.h>
#include <server.h>

/*
 * to avoid mixing http2client & http2 server related declrations 
 */
class HostSession;
using response_dispatch_cb = 
          std::function<void (shared_ptr<Handle> handle, int res_code, unique_ptr<Buffer<uint8_t>> response)>;
          

class Master {
public:
    Master(const HostInfo & master, const vector<HostInfo> & slaves);
    void init();
    void slave_register(const HostInfo & slave);
    void slave_set_state(const HostInfo & slave, Status status);
    /* peer is master secondary */
    void peer_register(const HostInfo & slave);
    void peer_set_state(const HostInfo & peer, Status status);
    bool get_status(); 
    void submit_response(shared_ptr<Handle> handle, int res_code, unique_ptr<Buffer<uint8_t>> res);
    void submit_slave_request(shared_ptr<Handle> handle, const string & uri, 
                              unique_ptr<Buffer<uint8_t>> req, response_dispatch_cb res_cb);
    void host_monitor_cb(const HostInfo & host, Status status);
    void register_request_cb(shared_ptr<Handle> handle, shared_ptr<Buffer<uint8_t>> request, 
                             const HeaderMap &h);
private:
    Server server;
    HealthMonitor health_monitor;
    vector<unique_ptr<HostSession>> slave_sessions;
    vector<unique_ptr<HostSession>> peer_sessions;
};


#endif /*__MASTER_H_INCLUDED__*/
