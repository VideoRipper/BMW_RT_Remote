#include "Version.h"

#ifndef RunClass_h
#define RunClass_h

#ifdef _USELED
#include <Adafruit_NeoPixel.h>

#define COLOR_BLACK   0x000000
#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#endif

#ifdef _USEPOWERMONITOR
String currentVoltage(int ADC_PIN);
#endif

class RunClass {
    public:  
      virtual void setup();
      virtual void loop();  
#ifdef _USEPOWERMONITOR
      virtual int getADCValue(int ADC_PIN);
      virtual float getVoltage(int ADC_PIN);
#endif      
#ifdef _USEDEEPSLEEP    
      virtual void gotoSleep();
      virtual void setSleep();
      virtual void resetSleep();
      virtual void checkSleep();
#endif                
#ifdef _USELED 
      virtual void setLED(uint16_t n = NUM_LEDS, int16_t pin = GPIO_LED,
                          neoPixelType type = TYPE_LED);
      virtual void setLEDColor(uint32_t theColor);                          
#endif      
    protected: 
#ifdef _USEDEEPSLEEP
      unsigned long SleepTime = 0;
#endif        
#ifdef _USELED 
      Adafruit_NeoPixel* pixel; 
#endif
};

#endif
