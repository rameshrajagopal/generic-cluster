#include <client.h>

Client::Client(const HostInfo & h, boost::asio::io_service & ios): 
        host(h), ios(ios)
{
    sess = make_shared<session>(ios, host.get_name(), host.get_port());
}

void Client::connect(session_connect_cb client_connect_cb)
{
    /* we can not access sess out of io service
     * hence post method is needed
     */
    ios.post(
    [this, client_connect_cb]() {
         sess->on_connect([this, client_connect_cb](tcp::resolver::iterator endpoint_it) {
             cout << "session connected" << endl;
             client_connect_cb(host, 0);
          });
          sess->on_error([this, client_connect_cb](const boost::system::error_code & ec) {
             cout << "session error " << ec.message() << endl;
             //need to convert from error_code to our own mapping
             client_connect_cb(host, 1);
          });
     });
}

/*
 * cases to handle 
 * 1) what happens after posted the callback, session got a disconnect error ?
 * 2) node monitor detected that slave is down ? it may be network blip or
 * proper shutdown how do you handle it ?
 * 3) in case of proper slave crash, the host_session has to clear up all
 * entries how do we do that 
 * 4) how do we protect "self" becoming invalid 
 * -> variables to look for "self", "self->sess", "handle", "self->req_map" is
 *  there anything ?
 * 5) How do we test all these scenarios
 */
void Client::do_GET_request(const shared_ptr<Handle> handle, 
                            const string & path, const shared_ptr<HeaderMap> & h, 
                            const shared_ptr<Buffer<uint8_t>> & req, 
                            response_receive_callback resp_cb)
{
    auto self  = shared_from_this();
    string url = self->get_url(path);
    ios.post(
            [self, handle, url, req, resp_cb, h]() {
             self->submit_request(handle, url, "GET", req, resp_cb, h);
            });
}
