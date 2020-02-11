#include "Cloudchip.h"
WiFiClient   wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;
char token[30];
char server[15] ="13.235.106.101";
String terminal= "sendCommand";
int bc;
String debug_terminal = "delay";
String debug_value;
String widget;
String widget1;
String json1= "";
String methodName;
String Attribute="";
static Ticker task1;
String Attribute_value = "";

 #if defined(ESP8266)
 static ESP8266WiFiMulti wifiMulti;
 #else
static WiFiMulti wifiMulti;
#endif
static unsigned int msg_count = 1;


void Cloudchip::Credentials(char tok[])
{
	strcpy(token,tok);
	client.setServer(server, 1883);
	client.setCallback(Cloudchip::on_message1);
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

bool Cloudchip::WiFiInit(const char *wifi_ssid,const char *wifi_password)
{

	ssid = wifi_ssid;
	password = wifi_password;
	
	  if (strcmp(WiFi.SSID().c_str(), ssid) != 0)
	  {
		WiFi.begin(ssid, password);
	  }
	  while (WiFi.status() != WL_CONNECTED)
	  {
		delay(500);
	  }
	  
  return 1;
}

bool Cloudchip::WiFiMode(unsigned char mode){
	
	
	int count = 0, n=0;
	
	while(count < 10){
		
		if(mode == 0)
		{
			/* Mode 0 Connects the open wifi*/
			
			WiFi.mode(WIFI_STA);
			WiFi.disconnect();
			n = WiFi.scanNetworks();
			for (int i = 0; i < n; ++i) {
				
				ssid = WiFi.SSID(i).c_str();
				password = "";
				#if defined(ESP8266)
				if(WiFi.encryptionType(i) == ENC_TYPE_NONE) wifiMulti.addAP(ssid, password);
				#else
				if(WiFi.encryptionType(i) == WIFI_AUTH_OPEN) wifiMulti.addAP(ssid, password);
				#endif
			}
			if(wifiMulti.run() == WL_CONNECTED) return 1;
		}
		else if(mode == 1)
		{
			WiFi.begin(ssid,password);
			if(WiFi.status() == WL_CONNECTED) return 1;	
		} 
		else if(mode == 2)
		{
			wifiMulti.addAP(ssid, password);
			if(wifiMulti.run() == WL_CONNECTED) return 1;
		}
		
		
		delay(500);
		count++;
	}
	return 0;
	
}





bool Cloudchip::Publish(String data,int mode) 
{	

	char buffer[100];
	data.toCharArray(buffer, 100);
	
  if (!client.connected())
  {
    while (!client.connect("Cloudchip Device", token, NULL))
    {
      delay(100);
    }
  }
  
  if(mode==1) client.publish("v1/devices/me/telemetry", buffer); 			// Telemetry
  else if(mode==2) client.publish("v1/devices/me/attributes", buffer); 		// Attribute
  else if(mode == 3) client.publish("v1/devices/me/rpc/response/+", buffer); // Widget
  
  return 1;
  
}
int Cloudchip::deviceTelemetry(String parameter, int value)
{
  String data;
  const size_t bufferSize = JSON_OBJECT_SIZE(1);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  root[parameter] = String(value);
  root.printTo(data);
  Cloudchip::Publish(data,1);
}
void Cloudchip::deviceAttributes(String parameter, int value)
{
  String data;
  const size_t bufferSize = JSON_OBJECT_SIZE(1);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  root[parameter] = String(value);
  root.printTo(data);
  Cloudchip::Publish(data,2);
}
void Cloudchip::serverAttributes(String parameter, int value)
{
  String data;
  const size_t bufferSize = JSON_OBJECT_SIZE(1);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  root[parameter] = String(value);
  root.printTo(data);
  Cloudchip::Publish(data,3);
}
int Cloudchip::deviceTelemetry(String parameter, String value)
{
  String data;
  const size_t bufferSize = JSON_OBJECT_SIZE(1);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  root[parameter] = value;
  root.printTo(data);
  Cloudchip::Publish(data,1);
}
double Cloudchip::deviceTelemetry(String parameter, double value)
{
  String data;
  const size_t bufferSize = JSON_OBJECT_SIZE(1);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  root[parameter] = String(value);
  root.printTo(data);
  Cloudchip::Publish(data,1);
}

void Cloudchip::reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    // Attempt to connect (clientId, username, password)
    if (client.connect("Device", token, NULL)) 
    {
      Serial.println( "[DONE]" );
      // Subscribing to receive RPC requests
      client.subscribe("v1/devices/me/rpc/request/+");
    }
    else 
    {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 5000 );
    }
  }
}
void Cloudchip::run() 
{
	if ( !client.connected() )
	Cloudchip::reconnect();
	client.loop();
}

void Cloudchip::on_message(const char* topic, byte* payload, unsigned int length) 
		{
			String responseTopic = String(topic);
			char json[length + 1];
			strncpy (json, (char*)payload, length);
			json[length] = '\0';
			json1 = (String)json;
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& data = jsonBuffer.parseObject((char*)json); 
			if (!data.success())
			{
				Serial.println("parseObject() failed");
			}
			methodName = String((const char*)data["method"]);
			String val = String((const char*)data["params"]);
			String val1 = String((const char*)data["params"]["command"]);
			String con1 ="true";
			String con2 ="false";
			if(val.compareTo(con1) || val.compareTo(con2))
			{
				widget = val;
			}
			if(methodName.equals(terminal))
			{
				widget1 = val1;
			}
			if(methodName.equals(debug_terminal))
			{
				debug_value = val;
			}
			int conv = val.toInt();
			bc = conv;
			
		}


void Cloudchip::updateWidget(String topic, String method,int val)
{
	String data = String(msg_count, DEC)+topic + method + String(val, DEC);
	Cloudchip::Publish(data,3);
}

String Cloudchip::getWidget()
{
	return json1;
}
String Cloudchip::getWidget(String method)
{
		String no_value = "0";
		if((methodName.equals(method)))
		{
			return widget;
		}
		else 
		{
			return no_value;
		}
}	
String Cloudchip::Terminal()
{
	String no_value = "0";
		if((methodName.equals(terminal)))
		{
			return widget1;
		}
		else 
		{
			return no_value;
		}
}
String Cloudchip::debugTerminal()
{
	String no_value = "0";
		if((methodName.equals(debug_terminal)))
		{
			return debug_value;
		}
		else 
		{
			return no_value;
		}
}	

//***************************************************************************************************************************************

void Cloudchip::run_attributes() 
{
	Cloudchip::reconnect_attributes();
	client.loop();
}

String Cloudchip::getAttributeValue(String attributes) 
{  
	Attribute    =  attributes;
    return Attribute_value;
}

String Cloudchip::Attribute_publishdata()
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  data["clientKeys"] = Attribute;
  char payload[256];
  data.printTo(payload, sizeof(payload));
  String strPayload = String(payload);
  return strPayload;
}

void Cloudchip::reconnect_attributes()
{ 
    if ( client.connect(" ", token, NULL) ) 
	{
      client.subscribe("v1/devices/me/attributes/response/+");
      client.publish("v1/devices/me/attributes/request/1",Attribute_publishdata().c_str());
    } 
	else 
	{
      Serial.print( "There is a issue in connecting to the device re-check detials provided");
      delay( 5000 );
    }
}

void Cloudchip::on_message1(const char* topic, byte* payload, unsigned int length)
 {
  char json[length + 1];
  strncpy (json, (char*)payload, length);
  json[length] = '\0';
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject((char*)json);
  if (!data.success()) {Serial.println("parseObject() failed");return;}
  String value_cloud = json;
  Attribute_value=value_cloud;
}




