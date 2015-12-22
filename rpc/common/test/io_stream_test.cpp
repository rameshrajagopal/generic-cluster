#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"
#include <string.h>

#include "io_stream.h"

using namespace std;

TEST(IOStreamTest, iostreamBasicTest) {
    request req;
    response res;
    boost::asio::io_service ios;
    shared_ptr<IOStream> sp = make_shared<IOStream>(req, res, ios);
}
