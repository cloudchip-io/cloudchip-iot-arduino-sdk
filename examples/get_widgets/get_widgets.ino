#include <Cloudchip.h>
#define SSID             "SSID" //WIFI Username
#define PASSWORD         "PASSOWRD" //WIFi Password
#define TOKEN            "ACCESS TOKEN" //Access Token
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
  String WidgetValue = cloudchip.getWidget();
  Serial.println("Value of Widget ="+String(WidgetValue));
  delay(2000);
}

