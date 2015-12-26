#ifndef __HOST_INFO_H_INCLUDED__
#define __HOST_INFO_H_INCLUDED__

enum class Status {
    INVALID = 0x0,
    NODE_UP = 0x1,
    NODE_DOWN = 0x2,
    SLAVE_UP  = 0x4,
    SLAVE_DOWN = 0x8,
};

typedef enum {
    HOST_TYPE_UNKNOWN = 0x0,
    HOST_TYPE_SLAVE = 0x1,
    HOST_TYPE_MASTER  = 0x2,
}HostType;

struct HostInfo {
    HostInfo() {};
    HostInfo(std::string name, std::string ip, std::string port):
        name(name), ip(ip), port(port), status(Status::INVALID), available(false){}

    Status get_status() const { return status; }

    void set_status(const Status & s) { status = s; }

    bool is_available() const { return available; }

    void set_available(bool avail) { available = avail; }

    string get_name() const { return name; }

    string get_ip() const { return ip; }

    string get_port() const { return port;}

    std::string name;
    std::string ip;
    std::string port;
    Status status;
    bool available;
};

#endif /*__HOST_INFO_H_INCLUDED__*/
