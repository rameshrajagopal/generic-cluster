#include <request_controller.h>

RequestController::RequestController(request_process_cb req_cb, response_process_cb res_cb):
                 request_cb(req_cb), response_cb(res_cb), is_alive(true) {}

void RequestController::start(int num_threads) 
{
    for (int num = 0; num < num_threads; ++num) {
        /* request processor threads */
        std::thread request_processor(
           [this]() {
              while (is_alive) {
                 unique_ptr<Request> req; // = std::move(self->request_q.pop());
                 request_q.pop(req);
                 if (request_cb) {
                    request_cb(req->handle, std::move(req->req), std::move(req->h));
                 }
              }
           }
        );
        request_processor.detach();
     }
     std::thread response_processor(
           [this]() {
              while (is_alive) {
                 unique_ptr<Response> res; // = std::move(self->response_q.pop());
                 response_q.pop(res);
                 if (response_cb) {
                    response_cb(res->handle, res->res_code, std::move(res->res));
                 }
              }
           }
     );
    response_processor.detach();
}

void RequestController::put_request(shared_ptr<Handle> handle, 
                                    unique_ptr<Buffer<uint8_t>> request, 
                                    unique_ptr<HeaderMap> h)
{
    unique_ptr<Request> req (new Request(handle, std::move(request), std::move(h)));
    cout << "ReqController:req->";
    request_q.push(std::move(req));
}

void RequestController::put_response(shared_ptr<Handle> handle, int res_code, 
                                     unique_ptr<Buffer<uint8_t>> response)
{
    unique_ptr<Response> res (new Response(handle, res_code, std::move(response)));
    cout << "ReqController:Response";
    response_q.push(std::move(res));
}
