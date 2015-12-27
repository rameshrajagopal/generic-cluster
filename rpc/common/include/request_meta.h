#ifndef __REQUEST_META_H_INCLUDED__
#define __REQUEST_META_H_INCLUDED__

#include <meta_data.h>
#include <io_stream.h>

class RequestMeta : public MetaData {
public:
   RequestMeta(size_t buf_size): MetaData(buf_size) {}
   RequestMeta(shared_ptr<IOStream> s, size_t buf_size):
            MetaData(buf_size), stream(s) {}       
   void send_response(int res_code, const header_map & h) {
       unique_ptr<Buffer<uint8_t>> u_res;
       get_response(u_res);
       stream->send_response(res_code, h, std::move(u_res));
   }
private:
   shared_ptr<IOStream> stream;
};
using Handle = RequestMeta;


#endif /*__REQUEST_META_H_INCLUDED__*/
