#include <Cloudchip.h>
Cloudchip cloudchip;
#define WIFI_SSID                       "PSP"           //Enter Your WIFI Username
#define WIFI_PASSWORD                   "seagate123"      //Enter WIFI Passsword
#define TOKEN                           "5lxml5MjMqnM73MAO4wX" //Enter the Acces Token 
void setup()
{
  Serial.begin(115200);
  cloudchip.WiFiInit(WIFI_SSID ,WIFI_PASSWORD);
  cloudchip.Credentials(TOKEN);  
}
void loop(){
  cloudchip.run();
  String TerminalCommands = cloudchip.Terminal();
  Serial.println("Termianl commands = "+TerminalCommands);
  delay(1000);
}

