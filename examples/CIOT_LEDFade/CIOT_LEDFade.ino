#include <Cloudchip.h>

#define WiFi_SSID "xxxxxxxxxx"
#define WiFi_PASSWORD "xxxxxxxxxxxxx"
#define API_KEY "xxxxxxxxxxxxxxxxxxxx"
#define DEVICE_ID "xxxxxxxxxxx"
#define STATUS_LED1 2

#define LEDC_CHANNEL_0    0
#define LEDC_TIMER_8_BIT  8       // use 13 bit precission for LEDC timer
#define LEDC_BASE_FREQ    5000   // use 5000 Hz as a LEDC base frequency

Cloudchip cloudchip;

void setup()
{
  Serial.begin(115200);
  cloudchip.WiFiConfig(WiFi_SSID,WiFi_PASSWORD);
  cloudchip.deviceConfig(API_KEY,DEVICE_ID); 

  ledcAttachPin(STATUS_LED1, LEDC_CHANNEL_0); // assign led pinsto channels
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT); // channels 0-15, freq limits depend on resolution, resolution 1-16 bits
 
  cloudchip.init();
}

void loop()
{  
  cloudchip.update(CONTROL);
  ledcWrite(LEDC_CHANNEL_0, cloudchip.readControl(1)); // LED fade with control data for channel 0
  delay(2000);
}
