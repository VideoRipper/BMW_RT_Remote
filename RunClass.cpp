#include "RunClass.h"

void RunClass::setup() {
  // Do nothing in parent class
}

void RunClass::loop() {
  // Do nothing in parent class
}

#ifdef _USEPOWERMONITOR
int RunClass::getADCValue(int ADC_PIN) {
  return analogRead(ADC_PIN);  
}

float RunClass::getVoltage(int ADC_PIN) {
  return (float)getADCValue(ADC_PIN) / (float)4096 * (float)POWER_MAX * ((float)ADC_NUM / (float)ADC_DEN);
}
#endif

#ifdef _USEDEEPSLEEP
void RunClass::gotoSleep() {
  Serial.println("Going to sleep...");
#ifdef _USELED      
  setLEDColor(COLOR_BLACK);
#endif      
  if (BTN_INVERT) {
    esp_deep_sleep_enable_gpio_wakeup(1 << BUTTON_MUTE, ESP_GPIO_WAKEUP_GPIO_LOW);
  } else {
    esp_deep_sleep_enable_gpio_wakeup(1 << BUTTON_MUTE, ESP_GPIO_WAKEUP_GPIO_HIGH);
  }
  resetSleep();
  esp_deep_sleep_start();  
}

void RunClass::setSleep() {
  if (this->SleepTime == 0 && SLEEP_DELAY > 0)  {    
    this->SleepTime = millis() + (SLEEP_DELAY * 1000);    
  }
}

void RunClass::resetSleep() {
  this->SleepTime = 0;
}

void RunClass::checkSleep() {
  if (this->SleepTime > 0 && millis() > this->SleepTime) {
    gotoSleep();    
  }  
}

#endif 

#ifdef _USELED
void RunClass::setLED(uint16_t n, int16_t pin, neoPixelType type) {    
  pixel = new Adafruit_NeoPixel(n, pin, type);
  pixel->begin();
  setLEDColor(0x000000);
}

void RunClass::setLEDColor(uint32_t theColor) {
  if (pixel) {  
    pixel->setPixelColor(0, theColor);
    pixel->show();   
  }
}
#endif
