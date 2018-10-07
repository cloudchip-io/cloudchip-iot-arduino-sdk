#include <Cloudchip.h>
Cloudchip cloudchip;
int Control1=11;
int Control2=12;
int Control3=13;
int Control4=15;
void setup()
{
  Serial.begin(115200);
  cloudchip.init("ssid","password","API KEY","Device ID");  
}

void loop()
{  cloudchip.update();
  cloudchip.setControl(Control1,Control2,Control3,Control4);//To  set the control's values of the device
 
}
