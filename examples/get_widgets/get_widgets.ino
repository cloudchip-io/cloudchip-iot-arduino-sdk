#include <Cloudchip.h>

#define SSID             "SSID" //WIFI Username
#define PASSWORD         "PASSOWRD" //WIFi Password
#define TOKEN            "ACCESS TOKEN" //Access Token

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
  String WidgetValue = cloudchip.getWidget("Method Name");
  Serial.println("Value of Widget ="+ WidgetValue);
  delay(500);
}

