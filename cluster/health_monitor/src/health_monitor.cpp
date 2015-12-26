#include <health_monitor.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <thread>

//#include <glog/logging.h>
#define LOG(arg) cout

class Pinger {
    static string get_ip_address(char const *hostname) {
        struct addrinfo hints, *res;
        struct in_addr addr;
        int err;

        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family = AF_INET;

        if ((err = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
            LOG(ERROR) << "Error while resolving IP for hostname: " << hostname << " | Error message: " << gai_strerror(err)
                 << " | error code is: " << err << endl;
            return "";
        }

        addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
        string ip =  string(inet_ntoa(addr));
        freeaddrinfo(res);
        return ip;
    }

public:
    static bool is_alive(char const * hostname, uint16_t port) {
        string ip = get_ip_address(hostname);

        bool connected = false;
        struct sockaddr_in client;
        fd_set fdset;
        struct timeval tv;

        client.sin_family = AF_INET;
        client.sin_port = htons(port);
        client.sin_addr.s_addr = inet_addr(ip.c_str());

        int sock = (int) socket(AF_INET, SOCK_STREAM, 0);
        fcntl(sock, F_SETFL, O_NONBLOCK);

        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);

        cout << "connecting to " << hostname << endl;
        connect(sock, (struct sockaddr *)&client, sizeof(client));

        tv.tv_sec = 1;  /* 1 second timeout */
        tv.tv_usec = 0;

        if (select(sock+1,NULL,&fdset,NULL,&tv) == 1 && errno != ENETUNREACH) {
            int sock_error;
            socklen_t len = sizeof(sock_error);
            getsockopt(sock,SOL_SOCKET,SO_ERROR,&sock_error,&len);
            if (sock_error == 0) {
                connected = true;
            }
        }

        if(!connected) {
            LOG(ERROR) << "Health check failed for: " << ip << ":" << port << endl;
        }
        close(sock);
        return connected;
    }
};

HealthMonitor::HealthMonitor(const vector<HostInfo> & hosts, 
                             uint32_t timeout, 
                             health_monitor_cb hm_cb): 
                             timeout(timeout), wakeup(false), is_alive(true)
{
    for (auto & host: hosts) {

        unique_ptr<HostInfo> h (new HostInfo(host.name, host.ip, host.port));
        hostmap[host.name] = std::move(h);
    }
    listeners.push_back(hm_cb);
}

void HealthMonitor::add(const HostInfo & host)
{
    std::unique_lock<std::mutex> mlock(_mutex);
    if (hostmap.find(host.name) != hostmap.end()) {
        cout << "Already found" << endl;
        auto & h = hostmap[host.name];
        h->set_available(true);
    } else {
        cout << "Inserting here " << endl;
        unique_ptr<HostInfo> h ( new HostInfo(host.name, host.ip, host.port));
        h->set_available(true);
        hostmap[host.name] = std::move(h);
    }
    mlock.unlock();
}

int HealthMonitor::remove(const HostInfo & host)
{
    int ret = -1;
    std::unique_lock<std::mutex> mlock(_mutex);
    if (hostmap.find(host.name) != hostmap.end()) {
        auto & h = hostmap[host.name];
        h->set_available(false);
        ret = 0;
    }
    mlock.unlock();
    return ret;
}

void HealthMonitor::register_listener(health_monitor_cb cb)
{
    std::unique_lock<std::mutex> mlock(_mutex);
    listeners.push_back(cb);
    mlock.unlock();
}

void HealthMonitor::start()
{
    std::thread monitor_thread([this]()
    {
        cout << "monitor_thread started " << is_alive <<  " " << timeout << endl;
        while (is_alive) {
           std::unique_lock<std::mutex> mlock(_mutex);
           for (auto & e: hostmap) {
              auto & host = e.second;
              if (host->is_available()) {
                bool status = Pinger::is_alive(host->get_name().c_str(), 
                                              atoi(host->get_port().c_str()));
                Status cur_status = (status) ? Status::NODE_UP : Status::NODE_DOWN;
                cout << "@m " << static_cast<int>(host->get_status()) << " " << static_cast<int>(cur_status) << endl;
                if (host->get_status() != cur_status) {
                    e.second->set_status(cur_status);
                    cout << "updated: " << static_cast<int>(e.second->get_status()) << endl;
                    for (auto & listener_cb: listeners) {
                        cout <<"calling cb" << endl;
                        listener_cb(host, cur_status);
                    }
                }
              }
           }
           cout << "waiting @ monitor thread " <<  timeout << endl;
           _cond.wait_for(mlock, std::chrono::seconds(timeout),
                          [this]() {return wakeup == true;});
           mlock.unlock();
        }
        cout << "monitor thread is exiting" << endl;
    });
    monitor_thread.detach();
}

void HealthMonitor::wakeup_monitor() 
{
    std::unique_lock<std::mutex> mlock(_mutex);
    wakeup = true;
    mlock.unlock();
    _cond.notify_one();
}

void HealthMonitor::stop()
{
    std::unique_lock<std::mutex> mlock(_mutex);
    wakeup = true;
    is_alive = false;
    mlock.unlock();
    cout << "stopping monitor thread" << endl;
    _cond.notify_one();
}

bool HealthMonitor::is_host_up(const HostInfo & host)
{
    std::unique_lock<std::mutex> mlock(_mutex); // will be unlocked when we go out of scope
    if (hostmap.find(host.get_name()) != hostmap.end()) {
        auto & h = hostmap[host.name];
        return (h->get_status() == Status::NODE_UP);
    }
    return false;
}
