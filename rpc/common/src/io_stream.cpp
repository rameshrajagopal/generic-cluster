#include <io_stream.h>
#include <memory>
#include <mutex>
using namespace std;

IOStream::IOStream(const request & req, const response & res, boost::asio::io_service & ios)
                : req(req), res(res), ios(ios), closed(false) {}
/*
* Need share ptr because of post callback and lamba 
*/
void IOStream::send_response(int res_code, const header_map & h, 
                             std::shared_ptr<Buffer<uint8_t>> resp)
{
    auto self = shared_from_this();
    ios.post([self, h, res_code, resp]() {
            std::lock_guard<std::mutex> lg(self->mu);
            if (self->closed) {
                return;
            }
            self->res.write_head(res_code, h); 
            self->res.end(
                [resp](uint8_t * buf, size_t len, uint32_t * flags) -> ssize_t {
                size_t ret = resp->copy(buf, len);
                if (ret < len) {/* end of response */
                    *flags = NGHTTP2_DATA_FLAG_EOF;
                }
                return ret;
                });
    });
}

void IOStream::close(bool f)
{
    std::lock_guard<std::mutex> lg(mu);
    closed = f;
}

