#ifndef device_h
#define device_h

#include "Arduino.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#else
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiMulti.h>
#endif

#include <DNSServer.h>
#include <memory>
#include <Ticker.h>


#if defined(ESP8266)
extern "C" {
  #include "user_interface.h"
}
#define ESP_getChipId()   (ESP.getChipId())
#else
#include <esp_wifi.h>
#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())
#endif

//typedef unsigned int u32;

#endif