#ifndef __SLAVE_H_INCLUDED__
#define __SLAVE_H_INCLUDED__

class Slave {
    Slave(const HostInfo & master, const HostInfo & host) {
    }
    void status_check_cb(shared_ptr<Handle> handle, shared_ptr<Buffer> request, const HeaderMap & h);
    bool get_status();
    void init();
    void submit_response(shared_ptr<Handle> handle, int res_code, Buffer & response);
private:
    HostSession master_sess;
    HostInfo host;
    Server server;
};

#endif /*__SLAVE_H_INCLUDED__*/
