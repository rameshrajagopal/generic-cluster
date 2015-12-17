#ifndef _QUERY_CONTROLLER_H_INCLUDED__
#define _QUERY_CONTROLLER_H_INCLUDED__

using query_request_cb = std::function<int (shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap & h)>;
using query_response_cb = std::function<void(shared_ptr<Handle> handle, shared_ptr<vector<shared_ptr<Buffer>>> response)>;

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
  shared_ptr<Buffer> res
  int res_code;
};

class QueryController {
    QueryController(query_request_cb req_cb, query_response_cb res_cb) {}
    void start(int num_threads) {
        /* start on thread for queuing the query */
        /* start num_threads to process the queries */
        /* request_dispatcher_thread {
             pop from request q
             generate random id for this request 
             update the handle with random id or unique value
             call query_request_callback
             update the handle with number of expected results
             create QueryMeta object 
             put into map
        }
        */
        /*
           response_dispatched_thread {
              pop from response q
              get the handle from response
              copy the buffer into responses;
              decrement response cnt
              if (response cnt == 0) {
                  call query_response_cb()
              }
           }
         */
    }
    void query_request_cb(shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap & h) {
        /* forms the Request by coping the handle, request, header values */
        /* push into request q */
    }
    void query_response_cb(shared_ptr<Handle> handle, int res_code, shared_ptr<Buffer> response) {
        /* forms the Response object */
        /* push into response q */
    }
private:
   void process_request();
   void process_response();
   map<uint64_t, shared_ptr<QueryMeta>> query_map;
   Queue<Request>  request_q;
   Queue<Response> response_q;
   query_request_callback request_cb;
   query_response_callback response_cb;
   int num_threads;
};


#endif /*_QUERY_CONTROLLER_H_INCLUDED__*/
