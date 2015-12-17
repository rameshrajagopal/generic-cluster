#ifndef __RPC_UTILS_H_INCLUDED__
#define __RPC_UTILS_H_INCLUDED__

#include <utility>
#include <nghttp2/asio_http2_server.h>
#include <nghttp2/asio_http2.h>
#include <mutex>
#include <cstdlib>

using namespace std;
using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

//has 4k buffer allocation on demand
class Buffer {
public:
      Buffer() {};
      uint32_t get(uint8_t ** buf);
      void put(uint8_t * buf, uint32_t len);
private:
      size_t offset;
      vector<uint8_t *> buffers;
};
using HeaderMap = header_map;

class RequestMeta: public std::enable_shared_from_this<RequestMeta> {
    RequestMeta(const request & req, const response & res, 
                boost::asio::io_service & ios, shared_ptr<Buffer> buf)
                : req(req), res(res), ios(ios), closed(false), buffer(buf) {}

    void commit_result(int res_code, shared_ptr<Buffer> buffer, HeaderMap & hmap, bool compression) {
        auto = self = shared_from_this();
        ios.post([self, compression, buffer]() {
             std::lock_gaurd<std::mutex> lg(self->_mutex);_
             if (self->closed) {
                return;
             }
             header_map h;
             if (compression) enable_compression(h);
             self->res.write_head(res_code, h);
             self->res.end([buffer](uint8_t * buf, size_t len, uint32_t * flags) -> ssize_t {
                /* send request from here */
             });
        });
    }
    void set_closed(bool f) {
        std::lock_gaurd<std::mutex> lg(_mutex);
        closed = f;
    }
    uint64_t get_rand_id() {
        return rand_id;
    }
    int get_response_cnt() {
        return res_cnt;
    }
    void update_response_cnt(int value) {
        res_cnt = value;
    }
private:
   void enable_compression(header_map & h) {
        struct header_value hv = {"gzip", true};
        h.insert(std::make_pair("content-encoding", hv))
   }
   const request & req; 
   const response & res;
   boost:asio::io_service & ios;
   std::mutex _mutex;
   bool closed;
   shared_ptr<Buffer> buffer;
   int  res_cnt {0};
   uint64_t rand_id {0};
};

using Handle = RequestMeta;


#endif /*__RPC_UTILS_H_INCLUDED__*/
