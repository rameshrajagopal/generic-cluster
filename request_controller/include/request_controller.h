#ifndef _REQUEST_CONTROLLER_H_INCLUDED__
#define _REQUEST_CONTROLLER_H_INCLUDED__

#include <common_types.h>
#include <request_meta.h>
#include <buffer.h>
#include <queue.h>
#include <atomic>

using request_process_cb = std::function<
            int (shared_ptr<Handle> handle, unique_ptr<Buffer<uint8_t>> request, unique_ptr<HeaderMap> h)>;
using response_process_cb = std::function<
            void(shared_ptr<Handle> handle, int res_code, unique_ptr<Buffer<uint8_t>> response)>;

struct QueryMeta{
   shared_ptr<Handle> handle;
   int res_count;
   shared_ptr<vector<shared_ptr<Buffer<uint8_t>>>> responses;
};

struct Request {
  Request(shared_ptr<Handle> handle, unique_ptr<Buffer<uint8_t>> req, unique_ptr<HeaderMap> h):
       handle(handle), req(std::move(req)), h(std::move(h)) {}
  shared_ptr<Handle> handle;
  unique_ptr<Buffer<uint8_t>> req;
  unique_ptr<HeaderMap> h;
};

struct Response {
  Response(shared_ptr<Handle> handle, int res_code, unique_ptr<Buffer<uint8_t>> res):
           handle(handle), res_code(res_code), res(std::move(res)) {}
  shared_ptr<Handle> handle;
  int res_code;
  unique_ptr<Buffer<uint8_t>> res;
};

//class RequestController:public std::enable_shared_from_this<RequestController> {
//double check whether do we need to pass a shared ptr to the callback theads 
//
class RequestController {
public:
    RequestController(request_process_cb req_cb, response_process_cb res_cb);
    void start(int num_threads);
    void put_request(shared_ptr<Handle> handle, unique_ptr<Buffer<uint8_t>> req, 
                     unique_ptr<HeaderMap> h);
    void put_response(shared_ptr<Handle> handle, int res_code, unique_ptr<Buffer<uint8_t>> res);
private:
   Queue<unique_ptr<Request>>  request_q;
   Queue<unique_ptr<Response>> response_q;
   request_process_cb  request_cb;
   response_process_cb response_cb;
   std::atomic<bool> is_alive {false};
};

#endif /*_REQUEST_CONTROLLER_H_INCLUDED__*/
