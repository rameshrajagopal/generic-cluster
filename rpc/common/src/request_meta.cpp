#include <request_meta.h>

RequestMeta:: RequestMeta(shared_ptr<IOStream> s, size_t buf_size): 
       stream(s),
       res(new Buffer<uint8_t>(buf_size)),
       req(new Buffer<uint8_t>(buf_size)) {}

void RequestMeta::update_request(uint8_t * buf, size_t len) 
{
    if (req != nullptr) req->put(buf, len);
}
void RequestMeta::update_response(uint8_t * buf, size_t len)
{
   if (res != nullptr) res->put(buf, len);
}

void RequestMeta::get_request(unique_ptr<Buffer<uint8_t>> & u_req)
{
    u_req = std::move(req);
    req = nullptr;
}
void RequestMeta::send_response(int res_code, const header_map & h)  
{
     stream->send_response(res_code, h, std::move(res));
}
