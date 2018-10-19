#ifndef Cloudchip_h
#define Cloudchip_h

#include "Arduino.h"
#include <String.h>

//#include "WiFiManager.h"
#include <Ticker.h>

#include "lib/json/ArduinoJson.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#else
#include <WiFi.h>
#include <WebServer.h>
#endif
#include <DNSServer.h>
#include <memory>

#if defined(ESP8266)
extern "C" {
  #include "user_interface.h"
}
#define ESP_getChipId()   (ESP.getChipId())
#else
#include <esp_wifi.h>
#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())
#endif

#include <WiFiMulti.h>

#define LED_PIN_ON 1
#define LED_PIN_OFF 0

#define STREAM 0
#define CONTROL 1

class Cloudchip
{
	
	public:
		
		int init();
		
		void deviceConfig(const char* api,const char* Deviceid);
		//void config(const char* ssid,const char* pass,const char* api,const char* device_id);
		//void config(const char* ssid,const char* pass,const char* api,const char* device_id, unsigned int ledpin);
		
		int WiFiConfig(const char* ssid,const char* pass);
		
		int readControl(int val);
		double readStream(int val);
		int writeControl(int h,int j,int k,int l);
		int writeStream(double s1 ,double s2,double s3,double s4);
		int update(int val);
		
		boolean startConfigPortal();
		boolean startConfigPortal(char const *apName, char const *apPassword = NULL);
		
		
		int WiFiConnect(unsigned char mode);
		
		String getWiFiSSID();
		String getWiFiPassword();
		int getWiFiRSSI();
		
	private:
		int mode;
		int ledPin;

		String api;
		String device_id;
		
		const char* srv1;
		const char* ssid;
		const char* pass;
		
		int control[4];
		double stream[4];
		
		int jsonDecode(String json_data, int w);
		int sendToServer(const char* url);
		int readData(const char* url, int w);
		int getRSSIasQuality(int RSSI);
		
		void LED_ON();
		void LED_OFF();
		void LED_Toggle();
		void LED_Toggle_Delay(unsigned int delay_ms);
		void LED_Toggle_Duty(unsigned int delay_ms, unsigned char duty);
		void LED_Init(unsigned int ledPin);
		
		//int configModeCallback (WiFiManager *myWiFiManager); 
		//int wifiConfig(const char* ssid,const char* pass);
		//int wifiConfig();
};
#endif
