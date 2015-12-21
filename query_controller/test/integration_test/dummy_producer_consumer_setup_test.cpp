#include "../../include/query_controller.h"
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstdlib>
#include <string.h>

using namespace std;
using namespace std::placeholders;
using request_dispatch_cb  = std::function<
void (shared_ptr<Handle> handle, unique_ptr<Buffer<uint8_t>> request, unique_ptr<HeaderMap> h)>;

typedef struct {
   string path;
   request_dispatch_cb request_cb;
}EpHandler;

class Producer {
public:
    Producer(request_dispatch_cb cb): req_cb(cb) {
    }
    void start(int num_threads) {
        is_alive = true;
        for (int i = 0; i < num_threads; ++i) {
            std::thread producer([this]() {
                while (is_alive) {
                /*
                 * create a request and send it to req controller 
                 */
                   shared_ptr<Handle> handle = make_shared<Handle>(req_num);
                   unique_ptr<Buffer<uint8_t>> req(new Buffer<uint8_t>(2048));
                   uint8_t req_source[2048] = "Producer";
                   cout << "Producer:req->";
                   uint64_t cnt = ++req_num;
                   cnt = snprintf((char *)req_source, sizeof(req_source), "%s%lu", "Request:", cnt);
                   req_source[cnt] = '\0';
                   req->put(req_source, sizeof(req_source));
                   unique_ptr<HeaderMap> h(new HeaderMap());
                   req_cb(handle, std::move(req), std::move(h));
                   std::this_thread::sleep_for(std::chrono::milliseconds(200));
               }
            });
            producer.detach();
        }
    }
    void stop() {
        is_alive = false;
    }
private:
   std::atomic<uint64_t> req_num {0};
   std::atomic<bool> is_alive {false};
   //vector<EpHandler> handlers;
   request_dispatch_cb req_cb;
};

class Consumer {
public:
    void consume_request(shared_ptr<Handle> handle, unique_ptr<Buffer<uint8_t>> req, unique_ptr<HeaderMap> h)
    {
        cout << "Consumer:Request-> " << *static_cast<int *>(handle.get()) << " " << static_cast<void *>(h.get());
        uint8_t * req_buf;
        size_t ret_len = 0;
        while ((ret_len = req->get(req_buf)) != 0) {
            cout << " " << req_buf << endl;
        }
    }
    void consume_response(shared_ptr<Handle> handle, int res_code, unique_ptr<Buffer<uint8_t>> res)
    {
        cout << "Consumer:Response-> " << *static_cast<int *>(handle.get()) << " rescode " << res_code << endl;
        uint8_t * res_buf;
        size_t ret_len = 0;
        while ((ret_len = res->get(res_buf)) != 0) {
            cout << res_buf << endl;
        }
    }
};


class DummyMaster {
public:
    DummyMaster(): 
        req_controller(std::bind(&DummyMaster::req_process_cb, this, _1, _2, _3), 
            std::bind(&DummyMaster::res_process_cb, this, _1, _2, _3)),
        producer(std::bind(&RequestController::request_receive_cb, &req_controller, _1, _2, _3)) {}
    void start(int num_producers, int num_consumers)
    {
        req_controller.start(num_consumers);
        producer.start(num_producers);
    }
    int req_process_cb(shared_ptr<Handle> handle, unique_ptr<Buffer<uint8_t>> req, unique_ptr<HeaderMap> h)
    {
        cout << "Master:req->";
        consumer.consume_request(handle, std::move(req), std::move(h));
        return 0;
    }
    void res_process_cb(shared_ptr<Handle> handle, int res_code, unique_ptr<Buffer<uint8_t>> res) 
    {
        consumer.consume_response(handle, res_code, std::move(res));
    };
    void stop() 
    {
        producer.stop();
    }
private:
    RequestController req_controller;
    Producer producer;
    Consumer consumer;
};

int main(int argc, char * argv[])
{
    DummyMaster master;

    if (argc != 3) {
        cout << argv[0] << " num_producers num_consumers" << endl;
        return -1;
    }
    master.start(atoi(argv[1]), atoi(argv[2]));
    std::this_thread::sleep_for(std::chrono::seconds(30));
    master.stop();
    return 0;
}

