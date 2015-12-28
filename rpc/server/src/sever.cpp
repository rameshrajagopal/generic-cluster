#include <server.h>

void req_data_callback(const shared_ptr<RequestMeta> & req_meta, 
                       const shared_ptr<header_map> & hmap,
                       request_dispatch_cb req_cb, const uint8_t * data, size_t len)
{
    req_meta->update_request(data, len);
    if (len == 0) {
        unique_ptr<Buffer<uint8_t>> u_req;
        req_meta->get_request(u_req);
        req_cb(req_meta, std::move(u_req), hmap);
    }
}

void server_request_callback(const request & req, const response & res, 
                                    request_dispatch_cb req_cb)
{
    //increment the reqid as part of monitor  
    auto & io_service = res.io_service();
    shared_ptr<IOStream> st = make_shared<IOStream>(req, res, io_service);
    shared_ptr<RequestMeta> req_meta = make_shared<RequestMeta>(st, 2048);
    res.on_close([st](uint32_t ec) {
      cout << "req close" << ec << endl;
      st->close(true);
    });
    shared_ptr<header_map> h = make_shared<header_map>(req.header());
    req.on_data([req_meta, h, req_cb](const uint8_t * data, size_t len) {
        req_data_callback(req_meta, h, req_cb, data, len);
    });
}

Server::Server(const HostInfo & host, const vector<EpHandler> & handlers, uint32_t timeout)
               : host(host)
{
    timeout = timeout;
    for (auto handler: handlers) {
        auto req_cb = handler.req_cb;
        server.handle(handler.path, [req_cb](const request & req, const response & res) {
            server_request_callback(req, res, req_cb);
        });
    }
}

int Server::run(bool async)
{
    boost::system::error_code ec;
    server.listen_and_serve(ec, host.name, host.port, async);
    return 0;
}

int Server::run(bool async, const string & key, const string & crt)
{
    boost::system::error_code ec;
    boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);

    tls.use_private_key_file(key, boost::asio::ssl::context::pem);
    tls.use_certificate_chain_file(crt);
    configure_tls_context_easy(ec, tls);
    server.listen_and_serve(ec, tls, host.name, host.port, async);
    return 0;
}

void Server::set_num_io_threads(uint32_t num_threads)
{
    num_threads = num_threads;
}

void Server::join() 
{
    server.join();
}

void Server::submit_response(shared_ptr<Handle> handle, int res_code, 
                            bool compression)
{
    header_map h;
    struct header_value hv = {"gzip", compression};
    h.insert(std::make_pair("content-encoding", hv));
    return handle->send_response(res_code, h);
}
