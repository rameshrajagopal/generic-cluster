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

      void put(BufferType * buf, size_t len) 
      {
          size_t idx = buffers.size()-1;
          size_t rem = size - w_offset;
          if (rem < len) {
             memcpy((BufferType *)buffers[idx], buf, rem); 
             BufferType * tmpbuf = allocate(size);
             w_offset = len-rem;
             memcpy((BufferType *)tmpbuf, buf+rem, w_offset);
             buffers.push_back(tmpbuf);
          } else {
              memcpy((BufferType *)buffers[idx]+ w_offset, buf, len);
              w_offset += len;
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
