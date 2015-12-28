#ifndef __CLIENT_H_INCLUDED__
#define __CLIENT_H_INCLUDED__

#include <nghttp2/asio_http2.h>
#include <nghttp2/asio_http2_client.h>
#include <header_map.h>
#include <mutex>
#include <response_meta.h>
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
          std::function<void (shared_ptr<Handle> handle, int res_code, 
                        unique_ptr<Buffer<uint8_t>> response)>;

/* make sure io_service is waition on some work */
class Client: public enable_shared_from_this<Client> {
public:
    Client(const HostInfo & host, boost::asio::io_service & ios);
    void connect(session_connect_cb sess_connect_cb);
    void do_GET_request(const shared_ptr<Handle> handle, const string & path, 
                       const shared_ptr<HeaderMap> & h,
                       const shared_ptr<Buffer<uint8_t>> & req, 
                       response_receive_callback cb);
    int do_POST_request(const shared_ptr<Handle> handle, const string & uri, 
                       const shared_ptr<HeaderMap> & h,
                       const shared_ptr<Buffer<uint8_t>> & req, 
                       response_receive_callback cb);
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
   inline string get_url(const string & path) {
       return "http://" + host.name + ":" + host.port + path;
   }
   void on_response_cb(const shared_ptr<Handle> handle, 
                       const shared_ptr<ResponseMeta> & res_meta, 
                       response_receive_callback resp_cb,
                       const response & res)
   { 
    std::lock_guard<std::mutex> lg(_mutex);
    if (closed) return;
    if (res_meta->is_closed()) return;
    int res_code = res.status_code(); 
    res.on_data([handle, res_code, res_meta, resp_cb](const uint8_t * data, size_t len) { 
        res_meta->update_response(data, len);
        if (len == 0) {
            unique_ptr<Buffer<uint8_t>> u_resp;
            res_meta->get_response(u_resp);
            resp_cb(handle, res_code, std::move(u_resp));
        }
        });
    }
    void on_req_close_cb(const shared_ptr<Handle> handle, 
                         const shared_ptr<ResponseMeta> & res_meta, 
                         response_receive_callback resp_cb, 
                         uint32_t ec,
                         uint64_t id)
    {
       std::lock_guard<std::mutex> lg(_mutex);
       if (closed) return;
       res_meta->set_closed(true);
       if (ec != 0) {
           resp_cb(handle, ec, nullptr);
       }
       req_map.erase(id);
    }
/*
 * this happens at io thread, check shareable entity b/w io thread and normal
 * guy
 *  client -> can become invalid if the slave is went down b/w io executing
 *  handle -> no sharable entity
 * 
 */
    void submit_request(const shared_ptr<Handle> & handle, 
                    const string & url, const string & method, 
                    const shared_ptr<Buffer<uint8_t>> & req_buf, 
                    response_receive_callback resp_cb, 
                    const shared_ptr<HeaderMap> & h) 
    {
       boost::system::error_code ec;
       generator_cb data_cb = 
           [req_buf](uint8_t * buf, size_t len, uint32_t * flags) -> ssize_t {
            size_t ret = req_buf->copy(buf, len);
            if (ret < len) {
                *flags = NGHTTP2_DATA_FLAG_EOF; 
            }
            return ret;
           };
    auto client = shared_from_this();
    /* No need to unlock, it is taken care by return */
    std::lock_guard<std::mutex> lg(_mutex);
    if (closed) return;
    /* */
    header_map hdr;
    h->copy(hdr);
    /* create meta data */
    auto req = sess->submit(ec, method, url, data_cb, hdr);
    uint64_t rid = client->id;
    shared_ptr<ResponseMeta> res_meta = make_shared<ResponseMeta>(4 * 1024);
    client->req_map.insert(std::pair<uint64_t, shared_ptr<ResponseMeta>>(rid, res_meta));
    /* on response call the response callback*/
    req->on_response(
            [client, handle, res_meta, resp_cb](const response & res) {
            client->on_response_cb(handle, res_meta, resp_cb, res);
            });
    /* close the request */
    req->on_close( [client, handle, res_meta, resp_cb, rid](uint32_t ec) {
            client->on_req_close_cb(handle, res_meta, resp_cb, ec, rid);
            });
   }
   HostInfo host;
   boost::asio::io_service & ios;
   std::map<uint64_t, shared_ptr<ResponseMeta>> req_map;
   shared_ptr<session> sess;
   std::atomic<uint64_t> id;
   std::mutex _mutex;
   bool closed {false};
};

#endif /*__CLIENT_H_INCLUDED__*/
