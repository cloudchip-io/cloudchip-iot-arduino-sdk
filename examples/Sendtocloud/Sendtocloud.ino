#include <Cloudchip.h>
Cloudchip cloudchip;
int Sensor1=1;
int Sensor1=2;
int Sensor1=3;
int Sensor1=4;
void setup()
{
  Serial.begin(115200);
  cloudchip.init("ssid","password","API KEY","Device ID");  
}

void loop()
{  
  cloudchip.update();//To update to the values from the server
  cloudchip.sendtoServer(Sensor1,Sensor2,Sensor3,Sensor4);//To send the sensor values to the Cloudchip 
  
}
