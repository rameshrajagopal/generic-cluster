#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"
#include <string.h>

#include <host_session.h>

using namespace std;

TEST(HostSessionTest, hsConnectTest) {
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);
    std::thread io_thread(
    [&io_service]() {
        cout << "io thread started..." << endl;
        io_service.run();
        cout << "io service exit" << endl;
    });
    io_thread.detach();
    HostSession hs(host, io_service);
}
