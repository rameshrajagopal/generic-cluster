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

TEST(RequestMetaTest, createRequestMetaUpdateRequest) {
    request req;
    response res;
    boost::asio::io_service ios;
    shared_ptr<IOStream> stream = make_shared<IOStream>(req, res, ios);

    size_t buf_size = 2 * 1024;
    RequestMeta req_meta(stream, buf_size);
    /*
     * update the request
     */
     size_t len = 1024;
     uint8_t source[len];
     int i = 0, num_times = 10;
     while (i < num_times) {
         memset(source, 'a' + i, sizeof(source));
         req_meta.update_request(source, len);
         i++;
     }
     /* get the request and verify the data */
     unique_ptr<Buffer<uint8_t>> dest;
     req_meta.get_request(dest);
     i = 0;
     size_t ret = 0;
     while (true) {
         ret = dest->copy(source, len);
         if (ret < len) {
             break;
         }
         EXPECT_EQ(source[0], 'a' + i);
         i++;
     }
     EXPECT_EQ(i, num_times);
}

TEST(RequestMetaTest, createRequestMetaUpdateResponse) {
    request req;
    response res;
    boost::asio::io_service ios;
    shared_ptr<IOStream> stream = make_shared<IOStream>(req, res, ios);

    size_t buf_size = 1024;
    RequestMeta req_meta(stream, buf_size);
    /*
     * update the request
     */
     size_t len = 2048;
     uint8_t source[len];
     int i = 0, num_times = 10;
     while (i < num_times) {
         memset(source, 'a' + i, sizeof(source));
         req_meta.update_response(source, len);
         i++;
     }
     /* get the request and verify the data */
     unique_ptr<Buffer<uint8_t>> dest;
     req_meta.get_response(dest);
     i = 0;
     size_t ret = 0;
     while (true) {
         ret = dest->copy(source, len);
         if (ret < len) {
             break;
         }
         EXPECT_EQ(source[0], 'a' + i);
         i++;
     }
     EXPECT_EQ(i, num_times);
}
