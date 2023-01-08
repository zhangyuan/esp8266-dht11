#include <Arduino.h>
#include <DHT.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "main.h"

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define TEXT_SIZE 2
#define ROW_HEIGHT 12 // row height = TEXT_SIZE * 6
#define LINE_SPACING 10

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup()
{
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) ;
  }

  display.clearDisplay();
  display.setTextSize(TEXT_SIZE);
  display.setTextColor(WHITE);
  display.cp437(true);
  display.display();

  delay(2000);

  dht.begin();
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("ERROR");
    display.display();

    return;
  }

  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");

  display.clearDisplay();
  display.setCursor(0, 0);

  display.println("H & T");
  
  display.setCursor(0, ROW_HEIGHT+LINE_SPACING+4);
  display.print(h);
  display.print(" %");

  display.setCursor(0, (ROW_HEIGHT+LINE_SPACING) * 2);
  display.print(t);
  display.print(" *C ");

  display.display();

  delay(2000);
}