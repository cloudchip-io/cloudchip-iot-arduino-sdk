#include <Cloudchip.h>
#define SSID             "SSID" //WIFI Username
#define PASSWORD         "Passowrd" //WIFi Password
#define TOKEN            "Access Token" //Access Token
Cloudchip cloudchip;
int i = 40;
void setup()
{
  Serial.begin(115200);
  cloudchip.WiFiInit(SSID,PASSWORD);
  cloudchip.Credentials(TOKEN);  
}
void loop()
{
  cloudchip.run();
  i++;
   cloudchip.deviceTelemetry("KEY",i);
   delay(2000);
}

