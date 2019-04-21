#include <Cloudchip.h>
#define SSID             "SSID" //WIFI Username
#define PASSWORD       	 "Passowrd" //WIFi Password
#define TOKEN            "Access Token" //Access Token

#define LEDPIN 2

Cloudchip cloudchip;
int change;

void setup()
{
  Serial.begin(115200);
  pinMode(LEDPIN,OUTPUT);
  cloudchip.WiFiInit(SSID,PASSWORD);
  cloudchip.Credentials(TOKEN);  
}

void loop()
{
  cloudchip.run();
  
  //Change the "getvalue"  with your name of the switch widget.
  
  int control = cloudchip.getWidget("getValue").toInt(); 
  if(control != change){
	
	change = control;
	Serial.println("LED Status : " + String(control, DEC));
	digitalWrite(LEDPIN,control);
  }
  
}

