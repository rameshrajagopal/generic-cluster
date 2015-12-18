#ifndef __HTTP2_CLIENT_H_INCLUDED__
#define __HTTP2_CLIENT_H_INCLUDED__

using connect_response_cb = std::function<void (const HostInfo & host, int res_code)>;
using response_dispatch_cb = std::function<void (shared_ptr<Handle> handle, int res_code, shared_ptr<Buffer> response>;

/* make sure io_service is waition on some work */
class Client {
    Client(const HostInfo & host, io_service & ios, connect_response_cb connect_cb) {
        // create a session and register on_connect, on_error callback
        // call the corresponding connect_cb
    }
    int do_GET_request(shared_ptr<Handle> handle, const string & uri, const Buffer & request, response_dispatch_cb cb) {
    }
    int do_POST_request(shared_ptr<Handle> handle, const string & uri, const Buffer & request, response_dispatch_cb cb) {
    }
private:
   HostInfo host;
   session & sess;
   io_service & ios;
   map<uint64_t, RequestMeta> req_map;
};

#endif /*__HTTP2_CLIENT_H_INCLUDED__*/
