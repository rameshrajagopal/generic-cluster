#ifndef __SERIALIZE_H_INCLUDED__
#define __SERIALIZE_H_INCLUDED__

class SerializeDeserializeIf {
public:
  virtual int serialize(const shared_ptr<Buffer>  buffer) = 0;
  virtual int deserialize(const shared_ptr<Buffer> & buffer) = 0; 
};


#endif /*__SERIALIZE_H_INCLUDED__*/
