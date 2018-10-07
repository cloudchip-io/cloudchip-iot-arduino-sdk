#include "Cloudchip.h"

String ciot_http_api1;
String ciot_http_api2;
String ciot_http_api3;
String ciot_http_api4;

static Cloudchip ciot;

WiFiClient client;

int z=0;
int t=0;

const char* iot_server1 = "iot.cloudchip.in";
int Cloudchip::init(const char* ssid,const char* pass,String API,String Deviceid)
{
	int count = 0;
	pinMode(2, OUTPUT);

	//ciot.ssid = ssid;
	//ciot.pass = pass;
	ciot.api = API;
	ciot.device_id = Deviceid;
	
    WiFi.begin(ssid,pass); 
	
    while (WiFi.status() != WL_CONNECTED) 
    {
        if(count > 20){
			digitalWrite(2,0);
			return 0;
		} 
		 
		digitalWrite(2,!digitalRead(2)); delay(500);
		count ++;
	}
  digitalWrite(2,1);
  

  
  ciot_http_api1 = "GET /api/user/ukey="+ciot.api+"&mode=249&dkey="+ciot.device_id+"/switches HTTP/1.1";
  ciot_http_api2 = "GET /api/user/ukey="+ciot.api+"&mode=248&skey="+ciot.device_id+"/";
  ciot_http_api3 = "GET /api/user/ukey="+ciot.api+"&mode=249&skey="+ciot.device_id+"/stream HTTP/1.1";
  ciot_http_api4 = "GET /api/user/ukey="+ciot.api+"&mode=250&dkey="+ciot.device_id+"/";
  
  return 1;
}

int Cloudchip::update(){
	

	ciot.readData(ciot_http_api1, 1);
	ciot.readData(ciot_http_api3, 0);
	
	return 1;
	
}

int Cloudchip::sendtoServer(int sensor1,int sensor2,int sensor3,int sensor4)
{
  if (!client.connect(iot_server1, 80)) 
  {
    Serial.println("Connection failed @1");
    return 0;
  }
    if (client.connect(iot_server1,80) )
	{
		String url1 = String(ciot_http_api4)+"sw1="+String(sensor1, DEC)+"&sw2="+String(sensor2, DEC)+"&sw3="+String(sensor3, DEC)+"&sw4="+String(sensor4, DEC)+" HTTP/1.1";
		String url = String(ciot_http_api2)+"in1="+String(sensor1, DEC)+"&in2="+String(sensor2, DEC)+"&in3="+String(sensor3, DEC)+"&in4="+String(sensor4, DEC)+" HTTP/1.1";
		if(t==1)
		{
			client.println(url1);
			client.println("Host: " + String(iot_server1)); 
      		client.println("Connection: close");
      		client.println();
      		Serial.println("Updated sensor data to server");
		}
		if(t==0)
		{
			client.println(url);
			client.println("Host: " + String(iot_server1)); 
      		client.println("Connection: close");
      		client.println();
      		Serial.println("Updated sensor data to server");
		}
		t=0;
    }
	return 1;
}

int Cloudchip::readData(String a, int w)
{
	// Send HTTP request
	if (!client.connect(iot_server1, 80)) {
    Serial.println("Connection failed @2");
    return 0;}
	// Send HTTP request
  	client.println(a);
  	client.println("Host: "+ String(iot_server1));
 	client.println("Connection: close");  
 	
  	if (client.println() == 0) 
	  {
    	Serial.println("Failed to send request");
    	return 0;
	  }
    	
  
  
  char status[32] = {0}; 
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) 
  {
    Serial.print("Unexpected response: ");
    Serial.println(status);
    return 0;
  }
  

  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return 0;
  }
  
  ciot.jsonFun(w);
  /*if(z==1){
  int control1= root["IP_VALUE1"];
  int control2= root["IP_VALUE2"];
  int control3= root["IP_VALUE3"];
  int control4= root["IP_VALUE4"];
  Serial.println("Switch 1= "+String(control1));
  Serial.println("Switch 2= "+String(control2));
  Serial.println("Switch 3= "+String(control3));
  Serial.println("Switch 4= "+String(control4));
  delay(5000);}
  if(z==2){
  int Stream1= root["input1"];
  int Stream2= root["input2"];
  int Stream3= root["input3"];
  int Stream4= root["input4"];
  Serial.println("Stream 1= "+String(Stream1));
  Serial.println("Stream 2= "+String(Stream2));
  Serial.println("Stream 3= "+String(Stream3));
  Serial.println("Stream 4= "+String(Stream4));
  delay(5000);}*/
  return 1;
}
int Cloudchip::setControl(int control1 ,int control2,int control3,int control4)
{	t=1;
	Cloudchip::sendtoServer(control1,control2,control3,control4);
	return 1;
}


int Cloudchip::getControl(int val)
{
	
	return ciot.control[val];
}


float Cloudchip::getStream(int val)
{
	return ciot.stream[val];
}


int Cloudchip::jsonFun(int w)
  {
  	
	DynamicJsonBuffer jsonBuffer;
	
	String json_data = client.readStringUntil('\r');
  	JsonObject& root = jsonBuffer.parseObject(json_data); 
  	
 	if (!root.success()) return 0;
	 
	if(w==1){
  	ciot.control[1]= root["IP_VALUE1"];
  	ciot.control[2]= root["IP_VALUE2"];
  	ciot.control[3]= root["IP_VALUE3"];
	ciot.control[4]= root["IP_VALUE4"];}
	if(w==0){
	ciot.stream[1]= root["input1"];
	ciot.stream[2]= root["input2"];
	ciot.stream[3]= root["input3"];
	ciot.stream[4]= root["input4"];}
	return 1;
  }
  
