message(STATUS "NGHTTP2_HOME: $ENV{NGHTTP2_HOME}")

find_path(Nghttp2_INCLUDE_DIR
  NAMES nghttp2/asio_http2_client.h
        nghttp2/asio_http2.h
        nghttp2/asio_http2_server.h
        nghttp2/nghttp2.h
        nghttp2/nghttp2ver.h
  HINTS $ENV{NGHTTP2_HOME}/include/ /usr/local/include/ /usr/include/
)

# find static lib path
find_path(Nghttp2_STATIC_LIB_PATH
  NAMES libnghttp2.a
        libnghttp2_asio.a
  HINTS $ENV{NGHTTP2_HOME}/usr/local/lib/ /usr/local/lib/ /usr/lib/
)

if (Nghttp2_STATIC_LIB_PATH)
  set(Nghttp2_FOUND TRUE)
else ()
  set(Nghttp2_FOUND FALSE)
endif ()

if (Nghttp2_FOUND)
  message(STATUS "Nghttp2 include dir: ${Nghttp2_INCLUDE_DIR}")
  message(STATUS "Nghttp2 static library: ${Nghttp2_STATIC_LIB_PATH}")
endif ()
