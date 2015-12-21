#ifndef _IO_STREAM_H_INCLUDED_
#define _IO_STREAM_H_INCLUDED_

#include <utility>
#include <nghttp2/asio_http2_server.h>
#include <nghttp2/asio_http2.h>
#include <mutex>

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

class IOStream: public std::enable_shared_from_this<IOStream> {
    IOStream(const request & req, const response & res, boost::asio::io_service & ios)
                : req(req), res(res), ios(ios), closed(false) {}
    /*
     * Need share ptr because of post callback and lamba 
     */
    void send_response(int res_code, const header_map & h, shared_ptr<Buffer<uint8_t>> response)
    {
        auto = self = shared_from_this();
        ios.post([self, h, response]() {
             std::lock_gaurd<std::mutex> lg(self->_mutex);_
             if (self->closed) {
                return;
             }
             self->res.write_head(res_code, h); 
             self->res.end([buffer](uint8_t * buf, size_t len, uint32_t * flags) -> ssize_t {
                 ssize_t ret = response.copy(buf, len);
                 if (ret < len) {/* end of response */
                    *flags = NGHTTP2_DATA_FLAG_EOF;
                 }
                 return ret;
             });
        });
    }
    void close(bool f) {
        std::lock_gaurd<std::mutex> lg(_mutex);
        closed = f;
    }
private:
   const request & req; 
   const response & res;
   boost:asio::io_service & ios;
   std::mutex _mutex;
   bool closed;
};

#endif /*_IO_STREAM_H_INCLUDED_*/
