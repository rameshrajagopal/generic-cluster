#ifndef _QUERY_CONTROLLER_H_INCLUDED__
#define _QUERY_CONTROLLER_H_INCLUDED__

using query_request_cb = std::function<int (shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap & h)>;
using query_response_cb = std::function<void(shared_ptr<Handle> handle, shared_ptr<vector<shared_ptr<Buffer>>> response)>;

struct QueryMeta{
   shared_ptr<Handle> handle;
   int res_count;
   shared_ptr<vector<shared_ptr<Buffer>>> responses;
};

class QueryController {
    QueryController(query_request_cb req_cb, query_response_cb res_cb, shared_ptr<SerializeDeserializeIf> sede) {}
    void start(int num_threads);
    void query_request_cb(shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap & h);
    void query_response_cb(shared_ptr<Handle> handle, int res_code, shared_ptr<Response> response); 
private:
   void process_request();
   void process_response();
   map<uint64_t, shared_ptr<QueryMeta>> query_map;
   Queue<Request>  request_q;
   Queue<Response> response_q;
   query_request_callback request_cb;
   query_response_callback response_cb;
   shared_ptr<SerializeDeSerializeIf> sede;
   int num_threads;
};


#endif /*_QUERY_CONTROLLER_H_INCLUDED__*/
