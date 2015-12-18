#ifndef __HOST_INFO_H_INCLUDED__
#define __HOST_INFO_H_INCLUDED__

typedef enum {
    STATUS_INVALID = 0x0,
    STATUS_NODE_UP = 0x1,
    STATUS_NODE_DOWN = 0x2,
    STATUS_SLAVE_UP  = 0x4,
    STATUS_SLAVE_DOWN = 0x8,
}Status;

typedef enum {
    HOST_TYPE_UNKNOWN = 0x0,
    HOST_TYPE_SLAVE = 0x1,
    HOST_TYPE_MASTER  = 0x2,
}HostType;

struct HostInfo {
    string name;
    string ip;
    int port;
};

#endif /*__HOST_INFO_H_INCLUDED__*/
