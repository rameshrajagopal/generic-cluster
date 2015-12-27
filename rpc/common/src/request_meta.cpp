#include <meta_data.h>

MetaData::MetaData(size_t buf_size):
       res(new Buffer<uint8_t>(buf_size)),
       req(new Buffer<uint8_t>(buf_size))
{}

void MetaData::update_request(const uint8_t * buf, size_t len) 
{
    if (req != nullptr) req->put(buf, len);
}
void MetaData::update_response(const uint8_t * buf, size_t len)
{
   if (res != nullptr) res->put(buf, len);
}

void MetaData::get_request(unique_ptr<Buffer<uint8_t>> & u_req)
{
    u_req = std::move(req);
    req = nullptr;
}

void MetaData::get_response(unique_ptr<Buffer<uint8_t>> & u_res)
{
    u_res = std::move(res);
    res = nullptr;
}

void MetaData::set_closed(bool close)
{
    closed = close;
}
