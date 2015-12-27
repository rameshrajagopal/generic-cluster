#ifndef __RPC_META_DATA_H_INCLUDED__
#define __RPC_META_DATA_H_INCLUDED__

#include <nghttp2/asio_http2.h>
#include <buffer.h>

using namespace std;
using namespace nghttp2::asio_http2;

class MetaData {
public:
   MetaData(size_t buf_size);
   void update_request(const uint8_t * buf, size_t len);
   void update_response(const uint8_t * buf, size_t len);
   void get_request(unique_ptr<Buffer<uint8_t>> & u_req);
   void get_response(unique_ptr<Buffer<uint8_t>> & u_res);
   void set_closed(bool close);
   bool is_closed() { return closed; }
private:
   unique_ptr<Buffer<uint8_t>> res;
   unique_ptr<Buffer<uint8_t>> req;
   uint64_t rand_id;
   bool closed {false};
};

#endif /*__RPC_META_DATA_H_INCLUDED__*/
