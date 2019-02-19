#ifndef Cloudchip_h
#define Cloudchip_h
#include "Arduino.h"
#include "lib\mqtt\PubSubClient.h" 
#include "lib\json\ArduinoJson.h"
#include <WiFi.h>
class Cloudchip
{
	public:
		void WiFiInit(const char* ssid,const char* pass);
		int widgetValue();
		void Credentials(char tok[]);
		int deviceTelemetry(String parameter,int value);
		double deviceTelemetry(String parameter,double value);
		int deviceTelemetry(String parameter, String value);
		void run();
		void Cloudchip_Widget(String val, int num);
		void deviceAttributes(String parameter, int value);
		void serverAttributes(String parameter, int value);
		String getWidget();
		String getWidget(String method);
		String Terminal();
		String debugTerminal();
		//void sendToWidget(String method,int val);		
		bool Publish(String data,int att);
	private:
		void reconnect();
		static void on_message(const char* topic, byte* payload, unsigned int length);
		
};

#endif

