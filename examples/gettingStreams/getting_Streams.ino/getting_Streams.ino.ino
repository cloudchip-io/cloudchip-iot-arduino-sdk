#include <Cloudchip.h>
Cloudchip cloudchip;
void setup()
{
  Serial.begin(115200);
  cloudchip.init("ssid","password","API KEY","Device ID");  
}

void loop()
{  
  cloudchip.update();//To update to the values from the server
  cloudchip.getStream(1);//TO get the required Stream value 
    delay(2000);
}
