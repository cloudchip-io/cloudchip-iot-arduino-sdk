#include "Cloudchip.h"


String ciot_http_api1;
String ciot_http_api2;

static Cloudchip ciot;
static WiFiClient client;

static Ticker update;
static WiFiMulti wifiMulti;
const char* iot_server1 = "iot.cloudchip.in";

static int WiFiMode = 0, precision=8;

int Cloudchip::WiFiConfig(const char* ssid,const char* pass){
	
	ciot.ssid = ssid;
	ciot.pass = pass;
	
	WiFiMode = 1;
}

int Cloudchip::getRSSIasQuality(int RSSI) {
  int quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

int Cloudchip::WiFiConnect(unsigned char mode){
	
	
	int count = 0, n=0;
	
	while(count < 10){
		
		if(mode == 0){
			
			WiFi.mode(WIFI_STA);
			WiFi.disconnect();
			n = WiFi.scanNetworks();
			for (int i = 0; i < n; ++i) {
				
				ciot.ssid = WiFi.SSID(i).c_str();
				ciot.pass = "";
				if(WiFi.encryptionType(i) == WIFI_AUTH_OPEN) wifiMulti.addAP(ciot.ssid, ciot.pass);
			}
			if(wifiMulti.run() == WL_CONNECTED) return 1;
		}
		else if(mode == 1){
			WiFi.begin(ciot.ssid,ciot.pass);
			if(WiFi.status() == WL_CONNECTED) return 1;	
		} 
		else if(mode == 2){
			wifiMulti.addAP(ciot.ssid, ciot.pass);
			if(wifiMulti.run() == WL_CONNECTED) return 1;
		}
		
		
		delay(500);
		count++;
	}
	return 0;
	
}

int Cloudchip::init(){
	
	
	//if(ciot.ledpin != -1) ciot.LED_Init(ciot.ledPin);
	
	ciot.WiFiConnect(WiFiMode);
	ciot_http_api1 = "GET /api/user/ukey="+String(ciot.api)+"&mode=249&dkey="+String(ciot.device_id)+"/switches HTTP/1.1";
	ciot_http_api2 = "GET /api/user/ukey="+String(ciot.api)+"&mode=249&skey="+String(ciot.device_id)+"/stream HTTP/1.1";
	
	return 1;
	
}

/*void Cloudchip::config(const char* ssid,const char* pass,const char* api,const char* device_id, unsigned int ledpin = -1)
{
	ciot.api = api;
	ciot.device_id = device_id;
	ciot.ledPin = ledpin;
	ciot.ssid = ssid;
	ciot.pass = pass;
	ciot.srv1 = iot_server1;
}*/

void Cloudchip::deviceConfig(const char* api,const char* device_id)
{
	ciot.api = api;
	ciot.device_id = device_id;
	ciot.srv1 = iot_server1;
	//ciot.ledpin = 2;
}

int Cloudchip::update(int val){

	if(val == 1 ) ciot.readData(ciot_http_api1.c_str(), 1); 
	else if(val == 0) ciot.readData(ciot_http_api2.c_str(), 0);
	else return 0;
	
	//ciot.LED_Toggle();
	return 1;
	
}

int Cloudchip::sendToServer(const char* url)
{
    if (!client.connect(iot_server1, 80)){
    //Serial.println("Connection failed @2");
    return -1;
	}
	
	//Serial.println(url);
	client.println(url);
	client.println("Host: " + String(iot_server1)); 
    client.println("Connection: close");
    client.println();
	return 1;
}

int Cloudchip::readData(const char* url, int w)
{
	
	if (!client.connect(iot_server1, 80)){
    //Serial.println("Connection failed @2");
    return -1;
	}
	
	//Serial.println(url);
	
	client.println(String(url));
	client.println("Host: " + String(iot_server1));	
	client.println("Connection: close");
	//client.println();

  	if (client.println() == 0) 
	  {
    	//Serial.println("Failed to send request");
    	return -1;
	  }
	   
	  
  char status[32] = {0}; 
  client.readBytesUntil('\r', status, sizeof(status));
  
  
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) 
  {
    //Serial.print("Unexpected response: ");
    return -1;
  }
  
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    //Serial.println(F("Invalid response"));
    return 0;
  }
  
  String json_data = client.readStringUntil('\r');
  
  ciot.jsonDecode(json_data, w);
  client.stop();
  return 1;
}
int Cloudchip::writeControl(int c1 ,int c2,int c3,int c4)
{
	
	String url = "GET /api/user/ukey="+ciot.api+"&mode=250&dkey="+ciot.device_id+"/";
	
	url += "sw1="+String(c1, DEC);
	url += "&sw2="+String(c2, DEC);
	url += "&sw3="+String(c3, DEC);
	url += "&sw4="+String(c4, DEC);
	url += " HTTP/1.1";
	ciot.sendToServer(url.c_str());
	
	return 1;
}

int Cloudchip::writeStream(double s1 ,double s2,double s3,double s4)
{
	String url = "GET /api/user/ukey="+ciot.api+"&mode=248&skey="+ciot.device_id+"/";
	
	url += "in1="+String(s1, precision);
	url += "&in2="+String(s2, precision);
	url += "&in3="+String(s3, precision);
	url += "&in4="+String(s4, precision);
	url += " HTTP/1.1";
	ciot.sendToServer(url.c_str());
	
	return 1;
}

int Cloudchip::readControl(int val)
{
	return ciot.control[val];
}


double Cloudchip::readStream(int val)
{
	return ciot.stream[val];
}


int Cloudchip::jsonDecode(String json_data, int w)
  {
  	
	DynamicJsonBuffer jsonBuffer;
  	JsonObject& root = jsonBuffer.parseObject(json_data); 
	
	int num = 0;
  	
 	if (!root.success()) return 0;
	 
	if(w==1){
  	ciot.control[1]= root["IP_VALUE1"];
  	ciot.control[2]= root["IP_VALUE2"];
  	ciot.control[3]= root["IP_VALUE3"];
	ciot.control[4]= root["IP_VALUE4"];
	}
	if(w==0){
		
	ciot.stream[1]= root["input1"];
	ciot.stream[2]= root["input2"];
	ciot.stream[3]= root["input3"];
	ciot.stream[4]= root["input4"];
	}
	return 1;
  }
  

void Cloudchip::LED_Init(unsigned int ledPin){
	
	pinMode(ledPin, OUTPUT);
	ciot.ledPin = ledPin;
	ciot.LED_OFF();
	
	
	
}  
void Cloudchip::LED_ON(){
	
	digitalWrite(ciot.ledPin, LED_PIN_ON);
	  
}
void Cloudchip::LED_OFF(){
	
	digitalWrite(ciot.ledPin, LED_PIN_OFF);
}
void Cloudchip::LED_Toggle(){
	
	digitalWrite(ciot.ledPin, !digitalRead(ciot.ledPin));
}
void Cloudchip::LED_Toggle_Delay(unsigned int delay_ms){
	
}

void Cloudchip::LED_Toggle_Duty(unsigned int delay_ms, unsigned char duty){
	
	
}

  
