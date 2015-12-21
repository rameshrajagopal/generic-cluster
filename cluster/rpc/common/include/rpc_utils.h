#ifndef __RPC_UTILS_H_INCLUDED__
#define __RPC_UTILS_H_INCLUDED__

#include <nghttp2/asio_http2.h>
#include <io_stream.h>
#include <buffer.h>

using namespace std;

class RequestMeta {
public:
   RequestMeta(shared_ptr<IOStream> s, int buf_size): 
       stream(s),
       res(new Buffer<uint8_t>(buf_size)),
       req(new Buffer<uint8_t>(buf_size)) {}

   void update_request(uint8_t * buf, size_t len) 
   {
       if (req != nullptr) req->put(buf, len);
   }
   void update_response(uint8_t * buf, size_t len);
   {
       if (res != nullptr) res->put(buf, len);
   }
   void get_request(Buffer<uint8_t> & u_req)
   {
       u_req = std::move(req);
       req = nullptr;
   }
   void send_response(int res_code, const header_map & h)  
   {
       stream->send_response(res_code, h, std::move(res));
   }
private:
   shared_ptr<IOStream> stream;
   unique_ptr<Buffer<uint8_t>> res;
   unique_ptr<Buffer<uint8_t>> req;
   uint64_t rand_id {0};
};

using Handle = RequestMeta;

#endif /*__RPC_UTILS_H_INCLUDED__*/
