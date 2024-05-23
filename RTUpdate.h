#ifndef RTUpdate_h
#define RTUpdate_h

#include "Version.h"
#include "RunClass.h"
 
class RTUpdate : public RunClass {
  public:
    virtual void setup();
    virtual void loop();  
};

#endif
