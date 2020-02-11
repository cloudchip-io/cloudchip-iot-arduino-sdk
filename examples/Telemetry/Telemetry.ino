#include <Cloudchip.h>

#define SSID             "SSID" //WIFI Username
#define PASSWORD         "Password" //WIFi Password
#define TOKEN            "Access Token" //Access Token

Cloudchip cloudchip;

int val;

void setup()
{
  Serial.begin(115200);
  cloudchip.WiFiInit(SSID,PASSWORD);
  cloudchip.Credentials(TOKEN);  
}
void loop()
{
  cloudchip.run();
  val =  random(1,100);
  cloudchip.deviceTelemetry("Sensor 1",val);
  delay(5000); //uploads data every 5 sec
}

