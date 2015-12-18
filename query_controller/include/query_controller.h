#ifndef _QUERY_CONTROLLER_H_INCLUDED__
#define _QUERY_CONTROLLER_H_INCLUDED__

#include <common_types.h>
#include <buffer.h>
#include <queue.h>

struct RequestMetaMock {
    int id;
};
struct HeaderValueMock {
    std::string value;
    bool sensitive;
};
using Handle = RequestMetaMock;
using HeaderMap = std::map<std::string, HeaderValueMock>;

using request_process_cb = std::function<
            int (shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap & h)>;
using response_process_cb = std::function<
            void(shared_ptr<Handle> handle, shared_ptr<vector<shared_ptr<Buffer>>> response)>;

struct QueryMeta{
   shared_ptr<Handle> handle;
   int res_count;
   shared_ptr<vector<shared_ptr<Buffer>>> responses;
};

struct Request {
  shared_ptr<Handle> handle;
  shared_ptr<Buffer> req;
  HeaderMap h;
};
struct Response {
  shared_ptr<Handle> handle;
  shared_ptr<Buffer> res;
  int res_code;
};

class QueryController {
    QueryController(request_process_cb req_cb, response_process_cb res_cb);
    void start(int num_threads);
    void query_request_cb(shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap & h);
    void query_response_cb(shared_ptr<Handle> handle, int res_code, shared_ptr<Buffer> response);
private:
   void process_request();
   void process_response();
   Queue<Request>  request_q;
   Queue<Response> response_q;
   request_process_cb  request_cb;
   response_process_cb response_cb;
};

#endif /*_QUERY_CONTROLLER_H_INCLUDED__*/
