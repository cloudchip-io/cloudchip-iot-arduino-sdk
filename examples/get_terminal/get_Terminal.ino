#include <Cloudchip.h>

Cloudchip cloudchip;
#define WIFI_SSID		"SSID"          //Enter Your WIFI Username
#define WIFI_PASSWORD	"Password"      //Enter WIFI Passsword
#define TOKEN			"Access Token" 	//Enter the Acces Token 

void setup()
{
  Serial.begin(115200);
  cloudchip.WiFiInit(WIFI_SSID ,WIFI_PASSWORD);
  cloudchip.Credentials(TOKEN);  
}

void loop(){
	
  cloudchip.run();
  String TerminalCommands = cloudchip.Terminal();
  Serial.println("Termianl command = "+TerminalCommands);
  
}

