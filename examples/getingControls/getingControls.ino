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
   for(char i = 1; i<5; i++){
     Serial.println("control" + String(i ,DEC) + " =" + String(cloudchip.getControl(i)));
   }
    delay(2000);
}
