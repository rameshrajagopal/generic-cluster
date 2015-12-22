#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"
#include <string.h>

#include <io_stream.h>
#include <request_meta.h>

using namespace std;

TEST(RequestMetaTest, createRequestMetaTest) {
    request req;
    response res;
    boost::asio::io_service ios;
    shared_ptr<IOStream> stream = make_shared<IOStream>(req, res, ios);

    size_t buf_size = 2 * 1024;
    RequestMeta req_meta(stream, buf_size);
}
