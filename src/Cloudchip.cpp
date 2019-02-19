#include "Cloudchip.h"
WiFiClient   wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;
char token[30];
char server[15] ="13.127.121.252";
String terminal= "sendCommand";
int bc;
String debug_terminal = "delay";
String debug_value;
String widget;
String widget1;
String json1= "";
String methodName;
String widget_message;

void Cloudchip::Credentials(char tok[])
{
	strcpy(token,tok);
	client.setServer(server, 1883);
	client.setCallback(Cloudchip::on_message);
}
void Cloudchip::WiFiInit(const char *ssid,const char *password)
{
	
  if (strcmp(WiFi.SSID().c_str(), ssid) != 0)
  {
    WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.println("Connected to WIFI");
}
bool Cloudchip::Publish(String data,int att) 
{	
  if (!client.connected())
  {
    while (!client.connect("ESP8266 Device", token, NULL))
    {
      delay(100);
    }
  }
  if(att==1)
  {
	String payload = data;
	char   attributes[100];
	payload.toCharArray(attributes, 100);
	client.publish("v1/devices/me/telemetry", attributes);
  }
  if(att==2)
  {
	String payload = data;
	char   attributes[100];
	payload.toCharArray(attributes, 100);
	client.publish("v1/devices/me/attributes", attributes);
  }
  
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
    if (client.connect("ESP8266 Device", token, NULL)) 
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
			//strncpy (json1, (char*)payload, length);
			json[length] = '\0';
			json1 = (String)json;
			//Serial.println(json);
			// Decode JSON request
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
			//ciot.sendToWidget("gesda",50);
			int conv = val.toInt();
			bc = conv;
			
		}
/*void Cloudchip::sendToWidget(topic, String method,int val)
{
	update = method;
	updateval = val;
}*/	

/*int Cloudchip::widgetValue()
{
	return bc;
}*/
String Cloudchip::getWidget()
{
	return json1;
}
String Cloudchip::getWidget(String method)
{
		String ntg = "0";
		if((methodName.equals(method)))
		{
			return widget;
		}
		else 
		{
			return ntg;
		}
}	
String Cloudchip::Terminal()
{
	String ntg = "0";
		if((methodName.equals(terminal)))
		{
			return widget1;
		}
		else 
		{
			return ntg;
		}
}
String Cloudchip::debugTerminal()
{
	String ntg = "0";
		if((methodName.equals(debug_terminal)))
		{
			return debug_value;
		}
		else 
		{
			return ntg;
		}
}	
