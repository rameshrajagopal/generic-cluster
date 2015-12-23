
#include <nghttp2/asio_http2_server.h>
#include <iostream>
#include <vector>
#include <list>
#include <thread>
#include <memory>
#include <nghttp2/asio_http2_server.h>
#include <buffer.h>

using namespace std;
using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

void http2_server_thread(const char * host_name, const char * port) 
{
    std::thread server_thread(
    [host_name, port]() {
    http2 server;
    std::atomic<int> req_num {0};

    server.handle("/", [&req_num](const request & req, const response & res) {
        int cnt = req_num++;
        shared_ptr<bool> is_req_alive = make_shared<bool>(true);
        shared_ptr<Buffer<uint8_t>> buffer = make_shared<Buffer<uint8_t>>(2048);
        res.on_close([is_req_alive](uint32_t error_code) {
            *is_req_alive = false;
        });
        req.on_data([is_req_alive, buffer, &res](const uint8_t * data, size_t len) {
            if (!is_req_alive) return;
            buffer->put(data,len);
            if (len == 0) {
                res.write_head(200);
                res.end(
                 [is_req_alive, buffer, &res](uint8_t * buf, size_t len, uint32_t * flags) -> ssize_t {
                    ssize_t ret = buffer->copy(buf, len);
                    if (ret < len) {
                        *flags = NGHTTP2_DATA_FLAG_EOF;
                    }
                    return ret;
                 });
            }
        });
    });
    server.handle("/hello", [](const request & req, const response & res) {
       res.write_head(200);
       res.end("Hello World");
       res.on_close([](uint32_t ec) {
           cout << "request got closed" << endl;
       });
    });
    boost::system::error_code ec;
    if (server.listen_and_serve(ec, host_name, port, true)) {
        std::cerr << "error: " << ec.message() << std::endl;
    }
    server.join();//this will block the server thread
    });
    server_thread.detach();
}

int main(int argc, char * argv[])
{
    if (argc == 3) {
        http2_server_thread(argv[1], argv[2]);
    } else {
        http2_server_thread("localhost", "3000");
    }
    getchar();
    return 0;
}
