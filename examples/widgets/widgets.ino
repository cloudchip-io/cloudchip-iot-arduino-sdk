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
  String widget_msg = cloudchip.getWidget();
  String widget_value = cloudchip.getWidget("setValue") //Can be Modifiable in Cloudchip.io
  Serial.println("Widget Json Data  =" + widget_msg);
  Serial.println("Widget Value = " + widget_value);
  delay(500);  
}

