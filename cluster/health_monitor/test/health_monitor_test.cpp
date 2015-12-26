#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"
#include <string.h>

#include <atomic>
#include <thread>
#include <health_monitor.h>

void hm_monitor_callback(const unique_ptr<HostInfo> & host, Status status)
{
    cout << host->name << endl;
}

TEST(HealthMonitorTest, healthMonitorHostNotEnabled) {
    vector<HostInfo> hosts;

    HostInfo host("localhost", "127.0.0.1", "3000");
    hosts.push_back(host);

    HealthMonitor hm(hosts, 2/*seconds*/, hm_monitor_callback);
    hm.start();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    EXPECT_EQ(hm.is_host_up(host), false);
    hm.stop();
}

TEST(HealthMonitorTest, healthMonitorNodeupEventTest) {
    vector<HostInfo> hosts;
    std::mutex _mutex;
    std::condition_variable _cond;

    HostInfo host("localhost", "127.0.0.1", "3000");
    hosts.push_back(host);

    std::atomic<bool> has_event_come {false};
    Status node_status;
    health_monitor_cb hm_cb = 
        [&has_event_come, &_cond, &node_status](const unique_ptr<HostInfo> & host, Status status) {
        node_status = status;
        has_event_come = true;
        _cond.notify_one();
    };

    HealthMonitor hm(hosts, 2/*seconds*/, hm_cb);
    hm.start();
    hm.add(host);
    /* start the server and wait for the callback to comeback */
    std::unique_lock<std::mutex> mlk(_mutex);
    _cond.wait_for(mlk, std::chrono::seconds(5), 
                  [&has_event_come](){return has_event_come == true;});
    hm.stop();
    EXPECT_EQ(has_event_come, true);
    EXPECT_EQ(node_status, Status::NODE_UP);
}

TEST(HealthMonitorTest, healthMonitorMultipleNodes) {
    vector<HostInfo> hosts;

    HostInfo host1("localhost", "127.0.0.1", "3000");
    hosts.push_back(host1);
    HostInfo host2("ramesh@debian", "127.0.0.1", "3000");
    hosts.push_back(host2);
    HostInfo host3("ramesh@indix", "192.168.0.243", "3000");
    hosts.push_back(host3);

    HealthMonitor hm(hosts, 2/*seconds*/, hm_monitor_callback);
    EXPECT_EQ(hm.is_host_up(host1), false);
    hm.add(host2);
    EXPECT_EQ(hm.is_host_up(host2), false);
    EXPECT_EQ(hm.is_host_up(host3), false);
}

TEST(HealthMonitorTest, healthMonitorMultipleEvents) {
    vector<HostInfo> hosts;
    std::mutex _mutex;
    std::condition_variable _cond;

    HostInfo host1("localhost", "127.0.0.1", "3000");
    hosts.push_back(host1);
    HostInfo host2("127.0.0.1", "127.0.0.1", "4000");
    HostInfo host3("ramesh@indix", "192.168.0.243", "3000");
    hosts.push_back(host3);

    std::atomic<int> num_of_events {0};
    health_monitor_cb hm_cb = 
        [&_cond, &num_of_events](const unique_ptr<HostInfo> & host, Status status) {
        cout << "health monitor cb " << endl;
        EXPECT_EQ(status, Status::NODE_UP);
        _cond.notify_one();
        ++num_of_events;
    };

    HealthMonitor hm(hosts, 3/*seconds*/, hm_cb);
    hm.start();
    hm.add(host1);
    hm.add(host2);
    /* start the server and wait for the callback to comeback */
    while (num_of_events < 2) {
        std::unique_lock<std::mutex> mlk(_mutex);
        _cond.wait_for(mlk, std::chrono::seconds(1), 
                [&num_of_events](){return num_of_events == 2;});
        mlk.unlock();
    }
    hm.stop();
    EXPECT_EQ(num_of_events, 2);
}

