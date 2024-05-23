#ifndef RTRemote_h
#define RTRemote_h

#include "Version.h"
#include "RunClass.h"

class RTRemote : public RunClass {
  public:
    virtual void setup();
    virtual void loop();  
  private:
    bool oldConnected = true;
};

#endif
