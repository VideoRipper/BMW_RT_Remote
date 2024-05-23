/* BMW RT11xxRT remote control to Bluetooth adapter version 1.2 by Peter Luijer <peter@luijer.org>

   Arduino script for a Bluetooth remote control connected designed to the external radio buttons of a BMW R11xxRT series of motorcycles.
   The script could also serve other uses, since it's actually nothing more than a programmable Bluetooth BLE keyboard controller.

   NOTE: This script has successfully been tested with "Normal" and "-C3" ESP32's.
   Boards like the NodeMCU and Wemos with an ESP32, as well as the M5Stamps should work.

   This script was written with the M5Stamp-Pico in mind; when using another board, then don't forget to change (or disable)
   the parts for the LED in the code by commenting out the _USELED define.

   TIP: When serial (debug) output is not working, make sure you've selected the correct board in the board manager.

   Current button assignments:

   Button  | Press       | Long press
   --------+-------------+-----------------------------------------
   MUTE/TP | Play/pause  | Unlock screen / close active application
   LEFT    | Prev. track | Start music player
   UP      | Vol. up     | Take screenshot
   RIGHT   | Next track  | Start Google maps
   DOWN    | Vol. down   | Lock screen

   Usage:
   Simply make sure everything is connected up, open your phone's Bluetooth settings and connect it to "BMW RT Remote"; that's it!

   Extra tip:
   Normally you can't fully unlock your phone, but if you add this remote to your "Trusted devices", you can:
   - On Android: Settings -> Lock Screen -> Smart Lock -> [ENTER PIN] -> Trusted Devices -> Add Trusted Device -> Select "BMW RT Remote" and confirm
   - On iOS: <To-be-investigated>

   v1.0 - Initial release
   v1.1 - Made code work on ESP32-C3 (and possible -S series) controlers; for this a modified version of EasyButton and BlueKeyboard is needed
        - Made use of Adafruit's NeoPixel library instead of FastLED, because of compatibility issues with C3 ESP's
   v1.2 - Added deep sleep, to preserve power, when remote is not connected for 60 seconds
          Waking the remote up again can be achieved by pressing the "Mute" button
          
          Current consumption, using an M5 Stamp C3 with LED enabled (using the on-board NeoPixel LED):
          - Not connected: 74.5mA
          - Connected: 72.8mA
          - Sleep: 0.41mA

          Current consumption, using a Seeed Studio XIAO ESP32C3 with LED disabled (it doesn't have an on-board LED):
          - Not connected: 88.3mA
          - Connected: 86.0mA      
          - Sleep: 0.25mA              
   v1.5 - Added OTA update functionality   
          Moved original code into an inherited version of a newly created class, so it's easier to split code between Bluetooth 
          and WiFi modes
          
          Update mode can be enabled by holding the "Mute" and "Right" buttons while resetting the ESP32
          When update mode is enabled, connect to the AP called "BMW RT Remote" (disregard any warnings about not having internet
          access), open a web browser and open the URL: http://192.168.4.1 
          On the page shown, you can upload new firmware to the device; the ESP will restart when the upload is finished
          
          Mode LED colors (if LED-code is enabled):
          - Bluetooth: red (disconnected) and blue (connected)
          - Wifi: green
   v1.6 - Reorganized OTA mode by using iframes
          Included build information to OTA page
   v1.7 - Added internal and external power supply monitoring, to enable proper shutdown when power is turned off 
          For this option to function properly an external backup battery (with charger) has to be connected
          Both internal and external power supplies can be monitored (but only the external one is actually used)

*/

#include "RunClass.h"
#include "RTRemote.h"
#include "RTUpdate.h"

RunClass* ActiveClass;

void setup() {
  pinMode(BUTTON_MUTE, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  
  if (digitalRead(BUTTON_MUTE) == LOW && digitalRead(BUTTON_RIGHT) == LOW) {
    ActiveClass = new RTUpdate();  
  } else {    
    ActiveClass = new RTRemote();    
  }
  ActiveClass->setup();
}

void loop() {
  ActiveClass->loop();
}
