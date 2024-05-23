#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Update.h>

#include "RTUpdate.h"

#include "Web_Index.h"
#include "Web_Settings.h"
#include "Web_FWUpdate.h"

WebServer server(80);

#ifdef _USEPOWERMONITOR
float currVoltage(int ADC_PIN) {
  return (float)analogRead(ADC_PIN) / (float)4096 * (float)POWER_MAX * ((float)ADC_NUM / (float)ADC_DEN);
}
#endif

String parseTemplate(String Template) {
  String result = Template;
  result.replace("%_CURRENT_VERSION_%", RemoteVersion);  
  result.replace("%_DEVICE_NAME_%", DeviceName);
   
  String options = "";
#ifdef _USEPOWERMONITOR
  options += "- Power monitor<br/>";
#endif
#ifdef _USELED
  options += "- Status LED<br/>";
#endif
#ifdef _USEDEEPSLEEP
  options += "- Deep sleep<br/>";
#endif    
  result.replace("%_CURRENT_OPTIONS_%", options);  

  String buttons = "";
  buttons += "- Mute: GPIO" + String(BUTTON_MUTE) + "<br/>";
  buttons += "- Left: GPIO" + String(BUTTON_LEFT) + "<br/>";
  buttons += "- Up: GPIO" + String(BUTTON_UP) + "<br/>";
  buttons += "- Down: GPIO" + String(BUTTON_DOWN) + "<br/>";
  buttons += "- Right: GPIO" + String(BUTTON_RIGHT) + "<br/>";
  result.replace("%_CURRENT_BUTTONS_%", buttons);

  String various = "";
  various += "- Debounce time: " + String(DEBOUNCE) + "<br/>";
  various += "- Hold delay: " + String(HOLD_DELAY) + "<br/>";
  various += "- Internal pull-up: " + String(BTN_PULLUP) + "<br/>";
  various += "- GPIO inverted: " + String(BTN_INVERT) + "<br/>";
#ifdef _USEPOWERMONITOR
  various += "- Internal ADC: GPIO" + String(POWER_INT) + " (" + String(currVoltage(POWER_INT), 2) + "V)<br/>";
  various += "- External ADC: GPIO" + String(POWER_EXT) + " (" + String(currVoltage(POWER_EXT), 2) + "V)<br/>";
#endif  
#ifdef _USEDEEPSLEEP
  various += "- Deep sleep delay: " + String(SLEEP_DELAY) + "<br/>";
#endif  
  result.replace("%_CURRENT_VARIOUS_%", various); 

  return result;
}

void RTUpdate::setup(void) {
  Serial.begin(115200);

#ifdef _USELED
  setLED();
  setLEDColor(COLOR_GREEN);
#endif  

  WiFi.softAP(DeviceId); 
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  Serial.println();
  Serial.print("Software version: ");
  Serial.println(RemoteVersion);
  Serial.print("SSID: ");
  Serial.println(DeviceId);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("URL: http://");
  Serial.println(IP);

  server.on("/BMW", HTTP_GET, []() {
    Serial.println("server.on(/BMW)");
    server.setContentLength(BMW_svg_gz_len);
    server.sendHeader(F("Content-Encoding"), F("gzip"));
    server.send_P(200, "image/svg+xml", BMW_svg_gz, BMW_svg_gz_len);
  });
  
  server.on("/", HTTP_GET, []() {
    Serial.println("server.on(/)");
    server.sendHeader(F("Connection"), F("close"));
    server.send(200, "text/html", parseTemplate(WebIndex)); 
  });    
    
  server.on("/settings", HTTP_GET, []() {
    Serial.println("server.on(/settings)");
    server.sendHeader(F("Connection"), F("close"));
    server.send(200, "text/html", parseTemplate(WebSettings));   
  });  

  /* Return javascript jquery */  
  server.on("/jquery.min", HTTP_GET, []() {
    Serial.println("server.on(/jquery.min)");
    server.setContentLength(jquery_min_js_v3_2_1_gz_len);
    server.sendHeader(F("Content-Encoding"), F("gzip"));
    server.send_P(200, "text/javascript", jquery_min_js_v3_2_1_gz, jquery_min_js_v3_2_1_gz_len);    
  });  

  server.on("/fwupdate", HTTP_GET, []() {
    Serial.println("server.on(/fwupdate)");
    server.sendHeader(F("Connection"), F("close"));
    server.send(200, "text/html", parseTemplate(FWUpdate));  
  });          
      
  /* Return version number */
  server.on("/fwversion", HTTP_GET, []() {
    Serial.println("server.on(/fwversion)");
    server.sendHeader(F("Connection"), F("close"));
    server.send(200, "text/plain", RemoteVersion);
  }); 
   
#ifdef _USEPOWERMONITOR
  /* Return internal supply voltage */
  server.on("/intvoltage", HTTP_GET, []() {
    Serial.println("server.on(/intvoltage)");
    server.sendHeader(F("Connection"), F("close"));
    server.send(200, "text/plain", String(analogRead(POWER_INT))); // String(currVoltage(POWER_INT), 2));
  });  

  /* Return external supply voltage */
  server.on("/extvoltage", HTTP_GET, []() {
    Serial.println("server.on(/extvoltage0)");
    server.sendHeader(F("Connection"), F("close"));
    server.send(200, "text/plain", String(analogRead(POWER_EXT))); // String(currVoltage(POWER_EXT), 2));
  });  
#endif

  /* Restarting ESP */
  server.on("/restart", HTTP_GET, []() {
    Serial.println("server.on(/restart)");
    server.sendHeader(F("Connection"), F("close"));
    server.send(200, "text/html", parseTemplate(WebRestart));
    ESP.restart();
  }); 
 
  /* Handling uploading firmware file */  
  server.on("/update", HTTP_POST, []() {
    Serial.println("server.on(/update)");
    server.sendHeader(F("Connection"), F("close"));
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update success: %u bytes\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}
 
void RTUpdate::loop(void) {
  server.handleClient();  
  yield();
}
