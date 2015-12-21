#include <client.h>

using namespace std;
using Handle = int;
using Buffer = std::string;
using RequestMap = const request *;
using session_connect_cb = std::function<void (const string & host, const string & port, int ec)>;
using query_request_callback = std::function<void (shared_ptr<Handle> handle, int res_code, Buffer * response)>;

Client::Client(string h, string p, boost::asio::io_service & ios): 
        host(h), port(p), ios(ios)
{
    sess = make_shared<session>(ios, host, port);
}

Client::~Client() 
{
   cout << "Destructor called" << endl;
   req_map.clear();
}

void Client::connect(session_connect_cb client_connect_cb)
{
    /* we can not access sess out of io service
     * hence post method is needed
     */
    auto self = shared_from_this();
    ios.post(
    [self, client_connect_cb]() {
         self->sess->on_connect([self, client_connect_cb](tcp::resolver::iterator endpoint_it) {
             cout << "session connected" << endl;
             client_connect_cb(self->host, self->port, 0);
          });
          self->sess->on_error([self, client_connect_cb](const boost::system::error_code & ec) {
             cout << "session error " << ec.message() << endl;
             //need to convert from error_code to our own mapping
             client_connect_cb(self->host, self->port, 1);
          });
     });
}

void Client::do_GET_request(shared_ptr<Handle> handle, const string & uri, Buffer req,
                        query_request_callback request_cb) 
{
    string url("http://"+host+":" + port + uri);
    auto self = shared_from_this();
    ios.post(
            [self, handle, url, req, request_cb]() {
            boost::system::error_code ec;
            string actual(req);
            auto req = self->sess->submit(ec, "GET", url, actual);
            uint64_t rid = ++self->id;
            self->req_map.insert(std::pair<uint64_t, RequestMap>(self->id, req));
            req->on_response(
                [self, handle, request_cb](const response & res) {
                int res_code = res.status_code(); 
                Buffer * buffer(new Buffer());
                res.on_data([handle, res_code, buffer, request_cb](const uint8_t * data, size_t len) {
                    if (len == 0) {
                    request_cb(handle, res_code, buffer);
                    }
                    });
                });
            req->on_close(
                [self, rid](uint32_t ec) {
                cout << "req closed : " << rid << endl;
                self->req_map.erase(rid);
                });
            });
}
