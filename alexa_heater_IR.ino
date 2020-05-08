#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define WIFI_SSID "upstairs"
#define WIFI_PASS "notdownstairs"
#define SERIAL_BAUDRATE                 115200

fauxmoESP fauxmo;
IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4 (D2 )

uint16_t rawData[99] = {1170,494,1234,426,398,1294,1170,490,1238,430,402,1298,
                        390,1310,394,1310,390,1310,358,1342,390,1310,1170,7042,
                        1226,438,1234,426,394,1298,1226,434,1234,434,398,1302,
                        394,1306,394,1306,394,1310,366,1334,390,1306,1170,7026,
                        1166,494,1266,394,430,1262,1170,490,1270,398,438,1262,
                        358,1342,358,1342,358,1342,358,1346,358,1338,1170,7042,
                        1170,494,1242,418,406,1282,1170,494,1246,422,410,1290,
                        358,1342,406,1294,362,1338,362,1338,362,1338,1170,
                        7022,1194,466,1210};
                        
// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  irsend.sendRaw(rawData, 99, 38);  // Send a raw data capture at 38kHz.
    
  if (state) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
}

void setup() {
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("FauxMo demo sketch");
    Serial.println("After connection, ask Alexa/Echo to 'turn <devicename> on' or 'off'");
    irsend.begin();
    // Wifi
    wifiSetup();

    // Fauxmo
    fauxmo.addDevice("Heater");
    fauxmo.onMessage(callback);
}

void loop() {
  fauxmo.handle();
}
