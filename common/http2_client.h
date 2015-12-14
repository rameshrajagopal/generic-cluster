#ifndef __HTTP2_CLIENT_H_INCLUDED__
#define __HTTP2_CLIENT_H_INCLUDED__

using connect_response_cb = std::function<void (const HostInfo & host, int response_code)>;
using response_dispatch_cb = std::function<void (uint64_t reqid, int response_code, Buffer & response)>;

typedef struct {
    uint64_t reqid;
    int res_code;
    shared_ptr<Buffer> buffer;
}ResponseMeta;

class ClientIf {
public:
    virtual uint64_t submit_request(Buffer & request, response_dispatch_cb cb, int extra) = 0;
};

/* make sure io_service is waition on some work */
class Http2Client : ClientIf {
    Http2Client(const HostInfo & host, io_service & ios, connect_response_cb connect_cb) {
        // create a session and register on_connect, on_error callback
        // call the corresponding connect_cb
    }
    int do_GET_request(const Buffer & request, response_dispatch_cb cb) {
    }
    int do_POST_request(const Buffer & request, response_dispatch_cb cb) {
    }
    uint64_t submit_request(const Buffer & request, response_dispatch_cb cb, int extra) {
        //generate reqId
        //create a response buffer
        //put into res_map
        for each response on data {
            // get the buffer
            // copy the data
            on_complete {
                // call response_dispatch_cb (reqid, responsecode, Buffer & response)
            }
        }
    }
private:
   HostInfo host;
   session & sess;
   io_service & ios;
   atomic_t<uint64_t> req_id;
   //we can do away with res_map, if we are able to pass through responseMeta
   //object through lambdas
   map<uint64_t, Buffer> res_map;
};

#endif /*__HTTP2_CLIENT_H_INCLUDED__*/
