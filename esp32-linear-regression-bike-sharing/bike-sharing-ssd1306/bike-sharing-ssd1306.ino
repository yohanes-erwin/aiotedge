// Author: Erwin Ouyang, aiotedge.tech
// Date  : 4 Jan 2020

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Check library setting
#if (SSD1306_LCDHEIGHT != 64)    // 128 x 64 pixel display
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define I2C_SDA 33
#define I2C_SCL 32
#define BMP280_ADDR 0x76
#define OLED_ADDR 0x3C

static const unsigned char PROGMEM temperature_bmp[] =
{
  B00000011, B11001111,
  B00000110, B01100000,
  B00000100, B00101111,
  B00000100, B00100000,
  B00000100, B00101111,
  B00000101, B10100000,
  B00000101, B10101111,
  B00000101, B10100000,
  B00001101, B10110000,
  B00011011, B11011000,
  B00110111, B11101100,
  B00101111, B11110100,
  B00101111, B11110100,
  B00110111, B11101100,
  B00011000, B00011000,
  B00001111, B11110000
};
static const unsigned char PROGMEM humidity_bmp[] =
{
  B00000000, B00000000,
  B00000111, B11100000,
  B00011111, B11111000,
  B00111001, B10011100,
  B00110001, B10001100,
  B01100011, B11000110,
  B01100111, B11100110,
  B01111110, B01111110,
  B01111110, B01111110,
  B01100111, B11100110,
  B01100011, B11000110,
  B00110001, B10001100,
  B00111001, B10011100,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};

Adafruit_BMP280 bmp;
Adafruit_SSD1306 oled;

void setup()
{
  // Initialize serial and I2C
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL, 400000);

  // BMP280 initialization
  bmp.begin(BMP280_ADDR);
  // Default settings from datasheet
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, Adafruit_BMP280::SAMPLING_X2,
      Adafruit_BMP280::SAMPLING_X16, Adafruit_BMP280::FILTER_X16, Adafruit_BMP280::STANDBY_MS_500);

  // OLED initialization
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled.clearDisplay();
}

void loop()
{
  int temp = bmp.readTemperature();
  int users = bike_predict(temp);
  
  oled.clearDisplay();
  oled.drawBitmap(32, 16, temperature_bmp, 16, 16, 1);
  oled.drawBitmap(32, 36, humidity_bmp, 16, 16, 1);
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(55, 16);
  oled.printf("%d%cC", temp, (char)247);
  oled.setCursor(55, 36);
  oled.printf("%d", users);
  oled.display();
  
  delay(1000);
}

int bike_predict(int x)
{
  float theta_0 = 25;
  float theta_1 = 1.25;
  float h = theta_0 + theta_1*x;

  return round(h);
}

