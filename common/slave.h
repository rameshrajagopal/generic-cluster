#ifndef __SLAVE_H_INCLUDED__
#define __SLAVE_H_INCLUDED__

class SlaveIf {
public:
    virtual bool get_status();
    virtual void init();
};

class Slave: SlaveIf {
    Slave(const HostInfo & host) {
        //register handler for on_status 
    }
    void stauts_check_cb(uint64_t reqid, Buffer & request) {
        //call the current objects' get status method and 
        //return the status
        //server.submit_response(reqid, int errorcode, Buffer & response);
    }
    bool get_status() {

    }
    void init() {
    }
protected:
    Http2Server server;
    HostInfo host;
    atomic_t<bool> initialized {false};
};

#endif /*__SLAVE_H_INCLUDED__*/
