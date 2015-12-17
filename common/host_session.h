#ifndef __HOST_SESSION_H_INCLUDED__
#define __HOST_SESSION_H_INCLUDED__

#include <rpc_utils.h>
#include <host_info.h>
#include <http2_client.h>

class HostSession {
    HostSession(const HostInfo & host);
    void submit_get_request(shared_ptr<Handle> handle, const string & uri, shared_ptr<Buffer> request, response_dispatch_cb cb);
    void submit_post_request(shared_ptr<Handle> handle, const string & uri, shared_ptr<Buffer> request, response_dispatch_cb cb);
    void session_connect_cb(const HostInfo & host, int res_code);
    bool get_session_status();
private:
    Client client;
    bool active;
};


#endif /*__HOST_SESSION_H_INCLUDED__*/
