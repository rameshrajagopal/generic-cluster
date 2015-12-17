#ifndef __SERVER_H_INCLUDED__
#define __SERVER_H_INCLUDED__

using request_dispatch_cb  = std::function<void (shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap & h)>;

typedef struct {
    string path;
    request_dispatch_cb request_cb;
}EpHandler

class Server {
     Server(const HostInfo & host, vector<EpHandler> & handlers);
     void set_response_timeout(uint32_t timeout);
     void set_num_io_threads(uint32_t num_threads);
#if 0
     Server(const HostInfo & host, vector<EpHandler> & handlers) {
        //register the handle
         for each handlers {
             for each request {
                 //increment the reqid
                 //create a stream object with req, res object
                 //create buffer 
                 //create a RequestMeta object push into map
                 for each req_data {
                     // extract the buffer 
                     // push into buffer 
                     on_complete {
                         // call corresponding request_call_back
                     }
                 }
             }
         }
     }
#endif
     void run();
     int submit_response(shared_ptr<Handle> handle,  int response_code, Buffer & response);
     //{
         //findout stream entry using reqid
         //if the req is not closed 
         //post the response into io_service
     //}
private:
     HostInfo host;
     vector<EpHandler> handlers;
     uint32_t timeout;
     uint32_t num_threads;
};


#endif /*__SERVER_H_INCLUDED__*/
