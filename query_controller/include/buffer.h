#ifndef __BUFFER_H_INCLUDED__
#define __BUFFER_H_INCLUDED__

#include <common_types.h>

//has 4k buffer allocation on demand
class Buffer {
public:
      Buffer() {};
      uint32_t get(uint8_t ** buf);
      void put(uint8_t * buf, uint32_t len);
private:
      size_t offset;
      vector<uint8_t *> buffers;
};

#endif /*__BUFFER_H_INCLUDED__*/
