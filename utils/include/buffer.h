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
      /* understand move constructor and put the implementation */

      size_t get(std::unique_ptr<BufferType>  & buf) 
      {
          size_t ret = 0;
          if (buffers.size() > 0) {
              buf = std::move(buffers[0]);
              buffers.erase(buffers.begin());
              ret = size;
              if (buffers.size() == 0) {
                  ret = offset;
              }
          }
          return ret;
      }

      void put(BufferType * buf, size_t len) 
      {
          size_t idx = buffers.size()-1;
          size_t rem = size - offset;
          if (rem < len) {
             memcpy((BufferType *)buffers[idx].get(), buf, rem); 
             std::unique_ptr<BufferType> tmpbuf = std::move(allocate(size));
             offset = len-rem;
             memcpy((BufferType *)tmpbuf.get(), buf+rem, offset);
             buffers.push_back(std::move(tmpbuf));
          } else {
              memcpy(((BufferType *)buffers[idx].get())+offset, buf, len);
              offset += len;
          }
      }

      int get_num_buffers()
      {
         return buffers.size();
      }
private:
      inline std::unique_ptr<BufferType> allocate(size_t size) {
          return std::unique_ptr<BufferType>(new BufferType[size]);
      }
      size_t size {1024};
      size_t offset {0};
      vector<std::unique_ptr<BufferType>> buffers;
};

#endif /*__BUFFER_H_INCLUDED__*/
