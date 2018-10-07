#ifndef Cloudchip_h
#define Cloudchip_h
#include "Arduino.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <String.h>
class Cloudchip
{
	public:
		
		int init(const char* ssid,const char* pass,String API,String Deviceid);
		int sendtoServer(int sensor1,int sensor2,int sensor3,int sensor4);
		int getControl(int val);
		float getStream(int val);
		int setControl(int h,int j,int k,int l);
		int readData(String a, int w);
		int update();
		
	private:
		int mode;
		String api;
		String device_id;
		char* ssid;
		char* pass;
		int control[4];
		float stream[4];
		int jsonFun(int w);
		
};
#endif
