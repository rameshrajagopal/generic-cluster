#ifndef _IO_STREAM_H_INCLUDED_
#define _IO_STREAM_H_INCLUDED_

#include <utility>
#include <nghttp2/asio_http2_server.h>
#include <nghttp2/asio_http2.h>
#include <mutex>
#include <memory>
#include <buffer.h>

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;
using namespace std;

class IOStream: public std::enable_shared_from_this<IOStream> {
public:
    IOStream(const request & req, const response & res, boost::asio::io_service & ios);
    void send_response(int res_code, const header_map & h, shared_ptr<Buffer<uint8_t>> response);
    void close(bool f);
private:
   const request & req; 
   const response & res;
   boost::asio::io_service & ios;
   std::mutex mu;
   bool closed;
};

#endif /*_IO_STREAM_H_INCLUDED_*/
