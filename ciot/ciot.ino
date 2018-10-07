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
  cloudchip.sendtoServer(Sensor1,Sensor2,Sensor3,Sensor4);//To send the sensor values to the required Stream in the cloud
  cloudchip.setControl(Control1,Control2,Control3,Control4);//To  set the control's values in of the device
   for(char i = 1; i<5; i++){
     Serial.println("control" + String(i ,DEC) + " =" + String(cloudchip.getControl(i)));
   }
    delay(2000);
}
