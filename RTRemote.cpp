/* When using another board than a generic ESP32 (eg. ESP32C3), replace the define in "EasyButtonTouch.cpp"
   from "#if defined(ESP32)" to (depending on the type of ESP) "#if defined(ESP32C3)" */
#include <EasyButton.h>
/* Change the security mode in "BleKeyboard.cpp" from "pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND)"
   to "pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND)" when experiencing Bluetooth connection problems */
#include <BleKeyboard.h>

#include "RTRemote.h"

EasyButton btnMute(BUTTON_MUTE,   DEBOUNCE, BTN_PULLUP, BTN_INVERT);
EasyButton btnLeft(BUTTON_LEFT,   DEBOUNCE, BTN_PULLUP, BTN_INVERT);
EasyButton btnUp(BUTTON_UP,       DEBOUNCE, BTN_PULLUP, BTN_INVERT);
EasyButton btnDown(BUTTON_DOWN,   DEBOUNCE, BTN_PULLUP, BTN_INVERT);
EasyButton btnRight(BUTTON_RIGHT, DEBOUNCE, BTN_PULLUP, BTN_INVERT);

BleKeyboard bleKeyboard(DeviceId, "QuickFix", 100);

void onMutePressed() {
  Serial.println("Mute short press");
  bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
}

void onMuteLongPressed() {
  Serial.println("Mute long press");
  bleKeyboard.write(KEY_ESC);
}

void onLeftPressed() {
  Serial.println("Left short press");
  bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
}

void onLeftLongPressed() {
  Serial.println("Left long press");
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.print("p");
  bleKeyboard.releaseAll();
}

void onUpPressed() {
  Serial.println("Up short press");
  bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
}

void onUpLongPressed() {
  Serial.println("Up long press");
  bleKeyboard.write(KEY_PRTSC);
}

void onDownPressed() {
  Serial.println("Down short press");
  bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
}

void onDownLongPressed() {
  Serial.println("Down long press");
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.print("l");
  bleKeyboard.releaseAll();
}

void onRightPressed() {
  Serial.println("Right short press");
  bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
}

void onRightLongPressed() {
  Serial.println("Right long press");
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.print("m");
  bleKeyboard.releaseAll();
}

void RTRemote::setup() {  
  Serial.begin(115200);
  Serial.printf("BMW RT Remote v%s ", RemoteVersion);
  
#ifdef _USEDEEPSLEEP  
  if (esp_sleep_get_wakeup_cause() == 0) {
    Serial.println("starting...");
  } else {
    Serial.println("waking up...");
  }
#else
  Serial.println("starting...");
#endif

#ifdef _USELED
  setLED();
  setLEDColor(COLOR_RED);
#endif

  btnMute.begin();  
  btnMute.onPressed(onMutePressed);
  btnMute.onPressedFor(HOLD_DELAY, onMuteLongPressed);

  btnLeft.begin();  
  btnLeft.onPressed(onLeftPressed);
  btnLeft.onPressedFor(HOLD_DELAY, onLeftLongPressed);

  btnUp.begin();  
  btnUp.onPressed(onUpPressed);
  btnUp.onPressedFor(HOLD_DELAY, onUpLongPressed);

  btnDown.begin();  
  btnDown.onPressed(onDownPressed);
  btnDown.onPressedFor(HOLD_DELAY, onDownLongPressed);

  btnRight.begin();  
  btnRight.onPressed(onRightPressed);
  btnRight.onPressedFor(HOLD_DELAY, onRightLongPressed);

  bleKeyboard.begin();  
  Serial.println("Remote control ready");
  setSleep();
}

void RTRemote::loop() {
  if (bleKeyboard.isConnected()) {
    
#ifdef _USEPOWERMONITOR    
    if (POWER_DOWN == 0 || getADCValue(POWER_EXT) > POWER_DOWN) {
      resetSleep(); 
    } else {
      setSleep();      
    }
#else
    resetSleep();
#endif

    if (oldConnected == false) {
      oldConnected = true;
      Serial.println("Bluetooth connected");
#ifdef _USELED      
      setLEDColor(COLOR_BLUE);
#endif      
    }

    btnMute.read();
    btnLeft.read();
    btnUp.read();       
    btnDown.read();
    btnRight.read();
  }
  else
  {
    if (oldConnected == true) {
      oldConnected = false;
      Serial.println("Bluetooth disconnected");
#ifdef _USELED      
      setLEDColor(COLOR_RED);
#endif      

#ifdef _USEPOWERMONITOR 
      setSleep();
#elif _USEDEEPSLEEP
      setSleep();
#endif      
    }  
    
  }  
  
#ifdef _USEDEEPSLEEP
  checkSleep();
#endif    
  
  yield();
}
