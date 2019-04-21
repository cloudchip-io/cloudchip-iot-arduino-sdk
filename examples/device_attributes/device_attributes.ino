

#include <Cloudchip.h>

#define SSID             "SSID" //WIFI Username
#define PASSWORD         "Passowrd" //WIFi Password
#define TOKEN            "Access Token" //Access Token

Cloudchip cloudchip;

void setup()
{
  Serial.begin(115200);
  cloudchip.WiFiInit(SSID,PASSWORD);
  cloudchip.Credentials(TOKEN);  
}
void loop()
{
  cloudchip.run();
  cloudchip.deviceAttributes("Device Id",1234);
  delay(3000);
}




