#ifndef __SERVER_H_INCLUDED__
#define __SERVER_H_INCLUDED__

#include <vector>
#include <memory>
#include <nghttp2/asio_http2_server.h>

#include <host_info.h>
#include <request_meta.h>
#include <buffer.h>
using namespace std;
using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

using request_dispatch_cb  = 
      std::function< void (shared_ptr<Handle> handle, unique_ptr<Buffer<uint8_t>> request, 
                     shared_ptr<HeaderMap> hmap)>;
struct EpHandler{
    EpHandler(const std::string & path, request_dispatch_cb cb): 
         path(path), req_cb(cb) {}
    std::string path;
    request_dispatch_cb req_cb;
};

class Server {
     Server(const HostInfo & host, const vector<EpHandler> & handlers, uint32_t timeout);
     void set_num_io_threads(uint32_t num_threads);
     int run(bool async);
     int run(bool async, const string & key, const string & crt);
     void join();
     void submit_response(shared_ptr<Handle> handle,  int res_code, bool compression);
private:
     http2 server;
     HostInfo host;
     vector<EpHandler> handlers;
     uint32_t timeout;
     uint32_t num_threads;
};


#endif /*__SERVER_H_INCLUDED__*/
