#ifndef __HTTP2_CLIENT_H_INCLUDED__
#define __HTTP2_CLIENT_H_INCLUDED__

#include <nghttp2/asio_http2_client.h>
#include <memory>
#include <thread>

using boost::asio::ip::tcp;
using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::client;

using session_connect_cb = std::function<void (const HostInfo & host, int res_code)>;
using response_dispatch_cb = std::function<void (shared_ptr<Handle> handle, int res_code, Buffer && response)>;

/* make sure io_service is waition on some work */
class Client: public std::enable_shared_from_this<Client> {
    Client(const HostInfo & host, io_service & ios);
    void connect(session_connect_cb sess_connect_cb);
    int do_GET_request(shared_ptr<Handle> handle, const string & uri, 
                       const Buffer & request, response_dispatch_cb cb);
    int do_POST_request(shared_ptr<Handle> handle, const string & uri,
                       const Buffer & request, response_dispatch_cb cb);
private:
   HostInfo host;
   boost::asio::io_service & ios;
   shared_ptr<session> sess;
   std::atomic<uint64_t> id {0};
   std::map<uint64_t, RequestMap> req_map;
};

#endif /*__HTTP2_CLIENT_H_INCLUDED__*/
