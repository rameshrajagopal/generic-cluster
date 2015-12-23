#ifndef __CLIENT_H_INCLUDED__
#define __CLIENT_H_INCLUDED__

#include <nghttp2/asio_http2_client.h>
#include <request_meta.h>
#include <buffer.h>
#include <memory>
#include <thread>
#include <host_info.h>

using boost::asio::ip::tcp;
using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::client;
using namespace std;

using session_connect_cb = std::function<void (const HostInfo & host, int ec)>;
using response_receive_callback = 
          std::function<void (shared_ptr<Handle> handle, int res_code, unique_ptr<Buffer<uint8_t>> response)>;

/* make sure io_service is waition on some work */
class Client: public enable_shared_from_this<Client> {
public:
    Client(const HostInfo & host, boost::asio::io_service & ios);
    void connect(session_connect_cb sess_connect_cb);
    int do_GET_request(shared_ptr<Handle> handle, const string & uri, 
                       std::unique_ptr<Buffer<uint8_t>> & req, response_receive_callback cb);
    int do_POST_request(shared_ptr<Handle> handle, const string & uri,
                       std::unique_ptr<Buffer<uint8_t>> & req, response_receive_callback cb);
    int do_GET_request(shared_ptr<Handle> handle, const string & uri, const header_map & h,
                       std::unique_ptr<Buffer<uint8_t>> & req, response_receive_callback cb);
    int do_POST_request(shared_ptr<Handle> handle, const string & uri, const header_map & h,
                       std::unique_ptr<Buffer<uint8_t>> & req, response_receive_callback cb);
    /*
     * returns whether success or failure, a create a on-demand connection to
     * decide to proceed for kill or save
     */
    bool disconnect(); 
    /*
     * when health monitor is detected that the slave is down, so we should make
     * this session as invalid, so that any request got posted will check and 
     * exit from io service
     */
    ~Client() {
       cout << "Destructor called" << endl;
       req_map.clear();
    }
private:
   inline string url_builder(const string & path) {
       return "http://" + host.name + ":" + host.port + path;
   }
   HostInfo host;
   boost::asio::io_service & ios;
   shared_ptr<session> sess;
   std::atomic<uint64_t> id {0};
   std::map<uint64_t, RequestMeta> req_map;
};

#endif /*__CLIENT_H_INCLUDED__*/
