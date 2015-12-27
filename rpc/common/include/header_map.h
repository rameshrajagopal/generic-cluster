#ifndef __HEADER_MAP_H_INCLUDED__
#define __HEADER_MAP_H_INCLUDED__

#include <map>
#include <nghttp2/asio_http2.h>

using namespace std;
using namespace nghttp2::asio_http2;

class HeaderMap {
public:
    HeaderMap(const header_map & h): hdr(h) {}

    void add(const string & key, const string & value) {
        struct header_value hv = {value, true};
        hdr.insert(std::make_pair(key, hv));
    }
    void copy(header_map & hmap) {
        hmap = hdr;
    }
    void enable_compression() {
        struct header_value hv = {"gzip", true};
        hdr.insert(std::make_pair("content-encoding", hv));
    }
private:
    header_map hdr;
};


#endif /*__HEADER_MAP_H_INCLUDED__*/
