#include <iostream>
#include <atomic>
#include "../include/queue.h"
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        return -1;
    }
    Queue<int> q;
    int num_consumers = atoi(argv[2]);
    int num_producers = atoi(argv[1]);
    int num_entries = atoi(argv[3]);
    int expected_count = num_producers * num_entries;
    std::atomic<int> src_counter {0};
    std::atomic<int> dest_counter {expected_count};
    std::atomic<bool> status {true};
    std::mutex _mutex;
    std::condition_variable _cond;

    for (int num = 0; num < num_producers; ++num) {
        std::thread producer([&q, num_entries, &src_counter]() {
                usleep(1 * 1000);
                for (int i = 0; i < num_entries; ++i) {
                   q.push(++src_counter);
                }
        });
        producer.detach();
    }
    for (int num = 0; num < num_consumers; ++num) {
         std::thread consumer([&q, &dest_counter, &_mutex, &_cond, &status]() {
           usleep(2 * 1000);
           int counter = 0;
           int cur_value = 0;
           int prev_value = -1;
           while (true) {
              cur_value = q.pop();
              if (prev_value > cur_value) {
                   status = false;
              }
              prev_value = cur_value;
              std::unique_lock<std::mutex> mlock(_mutex);
              counter = --dest_counter;
              mlock.unlock();
              if (counter == 0) {
                 cout << "exiting when counter is zero"<< endl;
                 _cond.notify_one();
                 break;
              } else if (counter < 0) {
                cout << "exiting " << counter << endl;
                break;
              }
           }
       });
       consumer.detach();
    }
    std::unique_lock<std::mutex> mlock(_mutex);
    auto now = std::chrono::system_clock::now();
    if (dest_counter > 0) {
        cout << "expected counter " << expected_count << endl;
        _cond.wait_until(mlock, now + std::chrono::seconds(20),
                [&dest_counter](){ return dest_counter <= 0;});
    }
    mlock.unlock();
    cout << "status: " << status << endl;
    cout << "expected : " << dest_counter << endl;
}

