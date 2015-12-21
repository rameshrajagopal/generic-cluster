#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"

#include <atomic>
#include "buffer.h"
#include "queue.h"

TEST(QueueTest, createQueue) {
    Buffer<uint8_t> buffer(2048);

    Queue<Buffer<uint8_t>> q;
    EXPECT_EQ(q.is_empty(), true);
}
TEST(QueueTest, queueBasicTest) {
    Queue<int> q;

    int i = 0;
    int num_entries = 10;
    while (i < num_entries) {
        q.push(i);
        ++i;
    }
    EXPECT_EQ(q.is_empty(), false);
    i = 0;
    while (!q.is_empty()) {
        EXPECT_EQ(q.pop(), i);
        ++i;
    }
}

TEST(QueueTest, queueConcurrencyTest) {
    Queue<int> q;

    int num_entries = 10;
    bool status = false;
    std::thread producer([&q, num_entries]() {
         for (int i = 0; i < num_entries; ++i) {
            q.push(i);
         }
    });
    std::thread consumer([&q, num_entries, &status]() {
         int verifier = 0;
         status = true;
         while (verifier < num_entries) {
             int i = q.pop();
             if (i != verifier) {
                status = false;
                break;
             }
             ++verifier;
         }
    });
    producer.join();
    consumer.join();
    EXPECT_EQ(status, true);
}

#if 0
TEST(QueueTest, queueConcurrencyStressTest) {
    Queue<int> q;
    int num_consumers = 10;
    int num_producers = 10;
    int num_entries = 10000;
    int expected_count = num_producers * num_entries;
    std::atomic<int> src_counter {0};
    std::atomic<int> dest_counter {expected_count};
    std::mutex _mutex;
    std::condition_variable _cond;

    for (int num = 0; num < num_producers; ++num) {
        std::thread producer([&q, num_entries, &src_counter]() {
                for (int i = 0; i < num_entries; ++i) {
                   q.push(++src_counter);
                }
        });
        producer.detach();
    }
    for (int num = 0; num < num_consumers; ++num) {
         std::thread consumer([&q, &dest_counter, &_mutex, &_cond]() {
           int counter = 0;
           int cur_value = 0;
           while (true) {
              cur_value = q.pop();
              std::unique_lock<std::mutex> mlock(_mutex);
              --dest_counter;
              counter = dest_counter;
              mlock.unlock();
              if (counter == 0) {
                 _cond.notify_one();
                 break;
              } else if (counter < 0) {
                break;
              }
           }
       });
       consumer.detach();
    }
    std::unique_lock<std::mutex> mlock(_mutex);
    auto now = std::chrono::system_clock::now();
    _cond.wait_until(mlock, now + std::chrono::seconds(10),
                    [&dest_counter, expected_count](){ return dest_counter <= 0;});
    EXPECT_EQ(dest_counter, 0);
}

TEST(QueueTest, queueConcurrencyIntegrityTest) {
    Queue<int> q;
    int num_consumers = 2;
    int num_producers = 1;
    int num_entries = 100000;
    int expected_count = num_producers * num_entries;
    std::atomic<int> src_counter {0};
    std::atomic<int> dest_counter {expected_count};
    std::atomic<bool> status {true};
    std::mutex _mutex;
    std::condition_variable _cond;

    for (int num = 0; num < num_producers; ++num) {
        std::thread producer([&q, num_entries, &src_counter]() {
                for (int i = 0; i < num_entries; ++i) {
                   q.push(++src_counter);
                }
        });
        producer.detach();
    }
    for (int num = 0; num < num_consumers; ++num) {
         std::thread consumer([&q, &dest_counter, &_mutex, &_cond, &status]() {
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
    _cond.wait_until(mlock, now + std::chrono::seconds(20),
                    [&dest_counter, expected_count](){ return dest_counter <= 0;});
    EXPECT_EQ(dest_counter, 0);
    EXPECT_EQ(status, true);
}

#endif
