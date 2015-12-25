#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"
#include <string.h>

#include <io_stream.h>
#include "server.h"

using namespace std;
void req_data_callback(const shared_ptr<RequestMeta> & req_meta, 
                              const shared_ptr<header_map> & hmap,
                              request_dispatch_cb req_cb, const uint8_t * data, size_t len);

TEST(ServerTest, reqDataCbTest) {
    request req;
    response res;
    boost::asio::io_service ios;
    shared_ptr<IOStream> st = make_shared<IOStream>(req, res, ios);
    shared_ptr<RequestMeta> req_meta = make_shared<RequestMeta>(st, 2048);
    size_t len = 2048;
    uint8_t source[len];
    int i = 0, num_buffers = 4;
    memset(source, 'c', sizeof(source));
    request_dispatch_cb my_callback = [num_buffers]
    (shared_ptr<Handle> handle, unique_ptr<Buffer<uint8_t>> req, shared_ptr<HeaderMap> hmap) {
        int i = 0;
        uint8_t * buf;
        while (req->get(buf) != 0) {
            ++i;
        }
        EXPECT_EQ(num_buffers, i);
    };
    shared_ptr<header_map> h = make_shared<header_map>();
    while (i < num_buffers) {
       req_data_callback(req_meta, h, my_callback, source, len);
       ++i;
    }
    req_data_callback(req_meta, h, my_callback, source, 0);
}
