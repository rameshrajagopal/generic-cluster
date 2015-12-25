#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"
#include <string.h>

#include <atomic>
#include <thread>
#include <health_monitor.h>

void hm_monitor_callback(const HostInfo & host, Status status)
{
    cout << host.name << endl;
}

TEST(HealthMonitorTest, healthMonitorInit) {
    vector<HostInfo> hosts;

    HostInfo host("localhost", "127.0.0.1", "3000");
    hosts.push_back(host);

    HealthMonitor hm(hosts, 2/*seconds*/, hm_monitor_callback);
    hm.start();
    EXPECT_EQ(hm.is_host_up(host), false);
}

TEST(HealthMonitorTest, healthMonitorNodeupEventTest) {
    vector<HostInfo> hosts;

    HostInfo host("localhost", "127.0.0.1", "3000");
    hosts.push_back(host);

    std::atomic<bool> has_event_come {false};
    health_monitor_cb hm_cb = [&has_event_come](const HostInfo & host, Status status) {
        EXPECT_EQ(status, Status::NODE_UP);
        has_event_come = true;
    };

    HealthMonitor hm(hosts, 2/*seconds*/, hm_cb);
    hm.start();
    hm.add(host);
    /* start the server and wait for the callback to comeback */
    std::this_thread::sleep_for(std::chrono::seconds(10));
    EXPECT_EQ(has_event_come, true);
}
