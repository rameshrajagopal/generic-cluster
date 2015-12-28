#ifndef __HOST_SESSION_H_INCLUDED__
#define __HOST_SESSION_H_INCLUDED__

#include <header_map.h>
#include <client.h>
#include <host_info.h>
#include <response_meta.h>
#include <buffer.h>

using namespace std;
using namespace placeholders;

class HostSession {
    HostSession(const HostInfo & host, boost::asio::io_service & ios):
         host(host.name, host.ip, host.port), ios(ios), client(nullptr), 
         active(false) {}
    int connect() 
    {
       if (client != nullptr) {
           client.reset(new Client(host, ios));
       } else {
           client = make_shared<Client> (host, ios);
       }
       shared_ptr<int> ret = make_shared<int>(1);
       shared_ptr<mutex> sp_mutex = make_shared<mutex>();
       shared_ptr<condition_variable> sp_cond = make_shared<condition_variable>();
       client->connect([ret, sp_mutex, sp_cond]
               (const HostInfo & host, int res_code) {
           std::unique_lock<std::mutex> mlock(*sp_mutex);
           *ret = res_code;
           mlock.unlock();
           sp_cond->notify_one();
       });
       std::unique_lock<std::mutex> mlock(*sp_mutex);
       sp_cond->wait_for(mlock, std::chrono::milliseconds(1000), 
                      [ret]() {return *ret != 1;});
       mlock.unlock();
       return *ret;
    }

    void submit_get_request(const shared_ptr<Handle> handle, 
                            const string & uri, 
                            const shared_ptr<HeaderMap> & h,
                            const shared_ptr<Buffer<uint8_t>> & req, 
                            response_receive_callback resp_cb)
    {
        client->do_GET_request(handle, uri, h, req, resp_cb);
    }

    void submit_post_request(const shared_ptr<Handle> handle,
                             const string & uri,
                             const shared_ptr<HeaderMap> & h,
                             const shared_ptr<Buffer<uint8_t>> & req, 
                             response_receive_callback resp_cb) 
    {
        client->do_POST_request(handle, uri, h, req, resp_cb);
    }
    bool get_session_status() { return active; }
    int  cleanup_session();
private:
    HostInfo host;
    boost::asio::io_service & ios;
    shared_ptr<Client> client;
    bool active;
};


#endif /*__HOST_SESSION_H_INCLUDED__*/
