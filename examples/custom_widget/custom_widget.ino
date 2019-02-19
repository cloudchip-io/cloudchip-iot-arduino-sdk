#include <Cloudchip.h>
#define SSID             "SSID" //WIFI Username
#define PASSWORD       	 "Passowrd" //WIFi Password
#define TOKEN            "Access Token" //Access Token
Cloudchip cloudchip;
String condition1 ="false";
String condition2 ="true";
void setup()
{
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  cloudchip.WiFiInit(SSID,PASSWORD);
  cloudchip.Credentials(TOKEN);  
}
void loop()
{
  cloudchip.run();
  String Relay1 = cloudchip.getWidget("Switch1"); //Change the "Switch1"  with you setValue name of the switch widget.
  String Relay2 = cloudchip.getWidget("Switch2"); //Change the "Switch2"  with you setValue name of the switch widget.
  if(Relay1.equals(condition1))
  {
    digitalWrite(2,HIGH);
  }
  if(Relay1.equals(condition2))
  {
    digitalWrite(2,LOW);
  }
   if(Relay2.equals(condition1))
  {
    digitalWrite(2,HIGH);
  }
  if(Relay2.equals(condition2))
  {
    digitalWrite(2,LOW);
  }
}

