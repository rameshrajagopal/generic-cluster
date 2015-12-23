#ifndef __BUFFER_H_INCLUDED__
#define __BUFFER_H_INCLUDED__

#include <common_types.h>

/*
 * TODO: What do we do when memory allocation failures 
 */
template <typename BufferType>
class Buffer {
public:
      Buffer(size_t size): size(size)
      {
          buffers.push_back(std::move(allocate(size)));
      }

      ~Buffer() 
      {
          for (auto buf: buffers) {
              delete[] buf;
          }
      }
      /* copy of the buffer type is deleted */
      Buffer(const Buffer &) = delete;
      Buffer & operator=(const Buffer &) = delete;

      size_t get(BufferType*  & buf) 
      {
          size_t ret = 0;
          if (buffers.size() > 0) {
              buf = buffers[0];
              buffers.erase(buffers.begin());
              ret = size;
              if (buffers.size() == 0) {
                  ret = w_offset;
              }
          }
          return ret;
      }

      ssize_t copy(BufferType * buf, ssize_t len) 
      {
          ssize_t copied = 0;
          ssize_t ret = 0;
          while ((len > 0) && (buffers.size() > 0)) {
              /* prefix to be copied */
              if (buffers.size() > 1) {
                  ret = _ncopy(buf, copied, len, (size - r_offset));
              } else {
                  ret = _ncopy(buf, copied, len, w_offset - r_offset);
              }
              copied += ret;
              len -= ret;
          }
          return copied;
      }

      void put(const BufferType * buf, ssize_t len) 
      {
          ssize_t idx = buffers.size()-1;
          ssize_t rem = 0;
          ssize_t stored = 0;
          while (len > 0) {
              if (w_offset == size) {
                  BufferType * tmpbuf = allocate(size);
                  buffers.push_back(tmpbuf);
                  w_offset = 0;
                  ++idx;
              }
              rem = size - w_offset;
              rem = (rem <= len) ? (rem): (len);
              /* copy the remaining bytes */
              memcpy((BufferType *)buffers[idx] + w_offset, buf + stored, rem); 
              w_offset += rem;
              stored += rem;
              len -= rem;
          }
      }

      int get_num_buffers()
      {
         return buffers.size();
      }
private:
      inline size_t _ncopy(BufferType * buf, ssize_t offset, ssize_t len, ssize_t actual) {
          if (len < actual) {
              memcpy(buf + offset, buffers[0] + r_offset, len);
              r_offset += len;
              offset += len;
          } else {
              memcpy(buf + offset, buffers[0] + r_offset, actual);
              buffers.erase(buffers.begin());
              r_offset = 0;
              offset += actual;
          }
          return offset;
      }
      inline BufferType * allocate(size_t size) {
          return (new BufferType[size]);
      }
      size_t size {1024};
      size_t w_offset {0};
      size_t r_offset {0};
      vector<BufferType *> buffers;
};

#endif /*__BUFFER_H_INCLUDED__*/
