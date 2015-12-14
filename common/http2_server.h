#ifndef __HTTP2_SERVER_H_INCLUDED__
#define __HTTP2_SERVER_H_INCLUDED__

using request_dispatch_cb  = std::function<void (int reqid, Buffer & request)>;

typedef struct {
    shared_ptr<Stream> st;
    shared_ptr<Buffer> buffer;
}RequestMeta;

typedef struct {
    string path;
    request_dispatch_cb request_cb;
}EpHandler

class ServerIf {
    virtual void run();
    virtual int submit_response(int reqid, int response_code, Buffer & response) = 0;
};

class Http2Server: ServerIf {
     Http2Server(const HostInfo & host, vector<EpHandler> & handlers) {
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
     void run() {
     }
     int submit_response(int reqid, int response_code, Buffer & response) {
         //findout stream entry using reqid
         //if the req is not closed 
         //post the response into io_service
         return 0;
     }
private:
     HostInfo host;
     vector<EpHandler> handlers;
     map<uint64_t, RequestMeta> req_map;
     atomic_t<uint64_t> req_id;
};


#endif /*__HTTP2_SERVER_H_INCLUDED__*/
