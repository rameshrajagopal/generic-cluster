#define GTEST_HAS_TR1_TUPLE 0
#include "gtest/gtest.h"
#include <string.h>

#include "buffer.h"

using namespace std;

TEST(BufferTest, bufferTestLenZero) {
    Buffer<uint8_t> buffer(16);
    unique_ptr<uint8_t> buf;
    EXPECT_EQ(buffer.get(buf), 0);
}

TEST(BufferTest, bufferTestCount) {
    Buffer<uint8_t> buffer(1024);
    unique_ptr<uint8_t> buf;

    size_t len = 1024;
    uint8_t source[len];
    buffer.put(source, len);

    int num_buffers = 0;
    while ((buffer.get(buf)) != 0) {
        ++num_buffers;
    }
    EXPECT_EQ(num_buffers, 1);
}

TEST(BufferTest, twoBufsTestCount) {
    Buffer<uint8_t> buffer(1024);
    unique_ptr<uint8_t> buf;

    size_t len = 1024;
    uint8_t source[len];
    buffer.put(source, len);
    buffer.put(source, 1);
    int num_buffers = 0;
    size_t ret_len = 0;
    while ((ret_len = buffer.get(buf)) != 0) {
        ++num_buffers;
        if (ret_len < len) break;
    }
    EXPECT_EQ(num_buffers, 2);
    EXPECT_EQ(ret_len, 1);
}

TEST(BufferTest, bufferCleanupCountTest) {
    Buffer<uint8_t> buffer(1024);
    unique_ptr<uint8_t> buf;

    size_t len = 1024;
    uint8_t source[len];
    buffer.put(source, len);
    buffer.put(source, 1);
    size_t ret_len = 0;
    while ((ret_len = buffer.get(buf)) != 0) {
        if (ret_len < len) break;
    }
    EXPECT_EQ(buffer.get_num_buffers(), 0);
}

TEST(BufferTest, bufferBoundaryConditionsTest) {
    Buffer<uint8_t> buffer(2000);

    int num_buffers = 10;
    uint8_t source[2000];
    int i = 0;
    while (i < num_buffers) {
        memset(source, 'a', 200);
        buffer.put(source, 200);
        ++i;
    }
    i = 0;
    while (i < num_buffers) {
        memset(source, 'b', sizeof(source));
        buffer.put(source, 200);
        ++i;
    }
    EXPECT_EQ(buffer.get_num_buffers(), 2);
    unique_ptr<uint8_t> dest;
    size_t ret_len = 0;
    i = 0;
    while ((ret_len = buffer.get(dest)) != 0) {
        memset(source, 'a' + i, sizeof(source));
        EXPECT_EQ(ret_len, 2000);
        EXPECT_EQ(memcmp(source, dest.get(), ret_len), 0);
        ++i;
    }
}

TEST(BufferTest, bufferBoundaryConditionsNumbuffersTest) {
    Buffer<uint8_t> buffer(2000);

    int num_buffers = 60;
    int buf_size = 2000;
    uint8_t source[buf_size];
    int i = 0;
    while (i < num_buffers) {
        memset(source, 'a', 176);
        buffer.put(source, 176);
        ++i;
    }
    /* total buffer size 100 * 176 = 17600 */
    /* num of buffers expected  = ceil(total_buff_size/one_buf_size) =>
     * 17600/2000 -> 9
     */
    num_buffers = (num_buffers * 176)/buf_size;
    if (((num_buffers * 176) % buf_size) != 0) ++num_buffers;
    EXPECT_EQ(buffer.get_num_buffers(), num_buffers);
    unique_ptr<uint8_t> dest;
    size_t ret_len = 0;
    i = 0;
    while ((ret_len = buffer.get(dest)) != 0) {
        ++i;
    }
    EXPECT_EQ(i, num_buffers);
}

TEST(BufferTest, bufferIntegrityTest) {
    int buf_size = 2000;
    Buffer<uint8_t> buffer(buf_size);

    int total_num_chunks = 26;
    int chunk_size = 200;
    uint8_t source[buf_size];
    int i = 0;
    while (i < total_num_chunks) {
        memset(source, 'a' + i, chunk_size);
        buffer.put(source, chunk_size);
        ++i;
    }
    int num_buffers = (total_num_chunks * chunk_size)/buf_size;
    if (((num_buffers * chunk_size) % buf_size) != 0) ++num_buffers;
    EXPECT_EQ(buffer.get_num_buffers(), num_buffers);
    /* verify the actual out from buffers */
    unique_ptr<uint8_t> dest;
    size_t ret_len = 0;
    size_t cnt = 0;
    size_t offset = 0;
    i = 0;
    while ((ret_len = buffer.get(dest)) != 0) {
        cnt = 0;
        offset = 0;
        while (cnt < (ret_len/chunk_size)) {
            memset(source, 'a' + i, chunk_size);
            EXPECT_EQ(memcmp(dest.get()+offset, source, chunk_size), 0);
            offset += chunk_size;
            ++cnt;
            ++i;
        }
    }
    EXPECT_EQ(i, total_num_chunks);
}


