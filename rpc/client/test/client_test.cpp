#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"
#include <string.h>
#include <condition_variable>
#include <mutex>
#include <host_info.h>
#include <client.h>

TEST(ClientTest, clientConnectSessionErrorTest) {
    /* start a io service */
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);
    std::thread io_thread(
    [&io_service]() {
        cout << "io thread started..." << endl;
        io_service.run();
        cout << "io service exit" << endl;
    });
    io_thread.detach();
    /* for sync b/w callback and this thread */
    std::mutex _mutex;
    std::condition_variable _cond;
    bool connected = false;
    bool got_response = false;
    /* create a client */
    string ip("127.0.0.1");
    string port("3000");
    string name("localhost");
    HostInfo host(name, ip, port);
    Client client(host, io_service);
    client.connect(
      [&_mutex, &_cond, &connected, &got_response](const HostInfo & host, int ec){
          std::unique_lock<std::mutex> mlock(_mutex);
          got_response = true;
          if (ec == 0) {
             connected = true;
          } else {
             connected = false;
          }
          cout << "session error with " << host.ip << endl;
          mlock.unlock();
          _cond.notify_one();
      });
      std::unique_lock<std::mutex> mlock(_mutex);
      _cond.wait_for(mlock, std::chrono::seconds(2), [&got_response](){return got_response == true;});
      EXPECT_EQ(connected, false);
      io_service.stop();
}

TEST(ClientTest, clientConnectSessionConnectTest) {
    /* start a io service */
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);
    std::thread io_thread(
    [&io_service]() {
        cout << "io thread started..." << endl;
        io_service.run();
        cout << "io service exit" << endl;
    });
    io_thread.detach();
    /*
     * start the mock_server 
     */
    std::this_thread::sleep_for(std::chrono::seconds(10));
    /* for sync b/w callback and this thread */
    std::mutex _mutex;
    std::condition_variable _cond;
    bool connected = false;
    bool got_response = false;
    /* create a client */
    string ip("127.0.0.1");
    string port("3000");
    string name("localhost");
    HostInfo host(name, ip, port);
    Client client(host, io_service);
    client.connect(
      [&_mutex, &_cond, &connected, &got_response](const HostInfo & host, int ec){
          std::unique_lock<std::mutex> mlock(_mutex);
          got_response = true;
          if (ec == 0) {
             connected = true;
          } else {
             connected = false;
          }
          cout << "session error with " << host.ip << endl;
          mlock.unlock();
          _cond.notify_one();
      });
      std::unique_lock<std::mutex> mlock(_mutex);
      _cond.wait_for(mlock, std::chrono::seconds(2), [&got_response](){return got_response == true;});
      EXPECT_EQ(connected, true);
      io_service.stop();
}

