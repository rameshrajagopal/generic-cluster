#ifndef __RPC_UTILS_H_INCLUDED__
#define __RPC_UTILS_H_INCLUDED__

#include <nghttp2/asio_http2.h>
#include <io_stream.h>
#include <buffer.h>

using namespace std;

class RequestMeta {
public:
   RequestMeta(shared_ptr<IOStream> s, size_t buf_size);
   void update_request(uint8_t * buf, size_t len);
   void update_response(uint8_t * buf, size_t len);
   void get_request(unique_ptr<Buffer<uint8_t>> & u_req);
   void send_response(int res_code, const header_map & h);
private:
   shared_ptr<IOStream> stream;
   unique_ptr<Buffer<uint8_t>> res;
   unique_ptr<Buffer<uint8_t>> req;
   uint64_t rand_id {0};
};
using Handle = RequestMeta;

#endif /*__RPC_UTILS_H_INCLUDED__*/
