#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define I2C_SDA 33
#define I2C_SCL 32

Adafruit_BMP280 bmp;

void setup()
{
  Serial.begin(115200);
  
  Wire.begin(I2C_SDA, I2C_SCL, 400000);
  if (!bmp.begin(0x76))
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  
  // Default settings from datasheet
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, Adafruit_BMP280::SAMPLING_X2,
      Adafruit_BMP280::SAMPLING_X16, Adafruit_BMP280::FILTER_X16, Adafruit_BMP280::STANDBY_MS_500);
}

void loop()
{ 
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  delay(1000);
}
