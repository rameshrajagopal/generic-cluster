#include <query_controller.h>

QueryController::QueryController(request_process_cb req_cb, response_process_cb res_cb):
                 request_cb(req_cb), response_cb(res_cb) {}
#if 0
void QueryController::start(int num_threads) {
        num_threads = 10;
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
void QueryController::query_request_cb(shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap & h) {
        /* forms the Request by coping the handle, request, header values */
        /* push into request q */
}
void QueryController::query_response_cb(shared_ptr<Handle> handle, int res_code, shared_ptr<Buffer> response) {
        /* forms the Response object */
        /* push into response q */
}
#endif
