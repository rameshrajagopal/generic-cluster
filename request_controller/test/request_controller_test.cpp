#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"

#include "../include/request_controller.h"
#include <mutex>
#include <condition_variable>
#include <atomic>

TEST(RequestControllerTest, createReqControllerBasicTest) {
    //int req_num = 10;
    int req_size = 1024;
    int res_size = 512;
    uint8_t req_source[req_size];
    uint8_t res_source[res_size];
    std::mutex _mutex;
    std::condition_variable _cond;
    std::atomic<bool> has_result_come {false};
    std::atomic<bool> has_request_come {false};

    memset(req_source, 'a', sizeof(req_source));
    memset(res_source, 'b', sizeof(res_source));

    request_process_cb request_cb = [req_size, &req_source, &_cond, &has_request_come]
        (shared_ptr<Handle> handle, unique_ptr<Buffer<uint8_t>> req, unique_ptr<HeaderMap> h) -> int {
          uint8_t * req_buf = nullptr;
          int size = req->get(req_buf); 
          cout << "received request ";
          cout << static_cast<void *>(handle.get());
          cout << size << " " << static_cast<void *>(h.get()) << endl;
          EXPECT_EQ(size, req_size);
          EXPECT_EQ(memcmp(req_buf, req_source, req_size), 0);
          has_request_come = true;
          _cond.notify_one();
          return 0;
    };

    response_process_cb response_cb = [res_size, &res_source, &_cond, &has_result_come] 
        (shared_ptr<Handle> handle, int res_code, unique_ptr<Buffer<uint8_t>> res) {
             uint8_t * res_buf = nullptr;
             cout << "received response: ";
             int size = res->get(res_buf);
             cout << static_cast<void *>(handle.get()) << endl;
             EXPECT_EQ(size, res_size);
             EXPECT_EQ(res_code, 200);
             EXPECT_EQ(memcmp(res_buf, res_source, res_size), 0);
             has_result_come = true;
             _cond.notify_one();
    };
    RequestController req_controller(request_cb, response_cb);
    req_controller.start(1);
    /* produce the request */
    shared_ptr<Handle> handle;
    /* create a request buffer */
    unique_ptr<Buffer<uint8_t>> req(new Buffer<uint8_t>(req_size));
    req->put(req_source, sizeof(req_source));
    /* create a headmer map */
    unique_ptr<HeaderMap> h(new HeaderMap());
    req_controller.put_request(handle, std::move(req), std::move(h));
    /* request received in callback now send the response */
    std::unique_lock<std::mutex> mlock(_mutex);
    while(!has_request_come) {
        _cond.wait_for(mlock, std::chrono::seconds(2), [&has_request_come](){return (has_request_come == true);});
    }
    mlock.unlock();
    /* send the response to the client */
    unique_ptr<Buffer<uint8_t>> res(new Buffer<uint8_t>(res_size));
    res->put(res_source, res_size);
    req_controller.put_response(handle, 200, std::move(res));
    /* wait till response come */
    mlock.lock();
    while(!has_result_come) {
        _cond.wait_for(mlock, std::chrono::seconds(2), [&has_result_come](){return (has_result_come == true);});
    }
    mlock.unlock();
}


