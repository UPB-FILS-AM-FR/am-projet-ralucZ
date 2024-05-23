// Raluca Zarnescu. Project: Clock with RTC1307 and ITU calculator with DHT11.

#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
#include <DHT.h>

// LCD pin configuration
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// DHT sensor configuration
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// RTC configuration
RTC_DS1307 RTC;

// LED pin configuration
#define LED_VERDE 11
#define LED_GALBEN 12
#define LED_ROSU 13

// Button and switch configuration
byte SW0 = A0;
byte SW1 = A1;
byte SW2 = A2;

// Variables for time setting
int zh, uh, ore;
int zm, um, miniti;

// Variables for ITU calculation
float t, h, ta;
int itu;

// Timing variables
unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Raluca  Zarnescu");
  lcd.setCursor(0, 1);
  lcd.print("Ceas cu RTC+ ITU");
  delay(2500);
  lcd.clear();

  // Initialize I2C communication
  Wire.begin();
  
  Wire.beginTransmission(0x68);
  Wire.write(0x07);  // Move pointer to SQW address
  Wire.write(0x10);  // 0x10 (hex) 00010000 (binary) to control register - turns on square wave
  Wire.endTransmission();

  // Initialize RTC
  RTC.begin();
  if (!RTC.isrunning()) 
  {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  // Configure buttons and switches
  pinMode(SW0, INPUT);  // For this, use a slide switch
  pinMode(SW1, INPUT);  // N.O. push button switch
  pinMode(SW2, INPUT);  // N.O. push button switch

  digitalWrite(SW0, HIGH);  // Pull-ups on
  digitalWrite(SW1, HIGH);
  digitalWrite(SW2, HIGH);

  // Configure LED pins
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_GALBEN, OUTPUT);
  pinMode(LED_ROSU, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_GALBEN, LOW);
  digitalWrite(LED_ROSU, LOW);

  // Test LEDs
  digitalWrite(LED_VERDE, HIGH);
  delay(300);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_GALBEN, HIGH);
  delay(300);
  digitalWrite(LED_GALBEN, LOW);
  digitalWrite(LED_ROSU, HIGH);
  delay(300);
  digitalWrite(LED_ROSU, LOW);
  
  delay(1000);
  lcd.clear();
}

void loop() 
{
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;

    DateTime now = RTC.now();
    h = dht.readHumidity();
    t = dht.readTemperature();
    float i = (t * 1.8 + 32) - (0.55 - 0.0055 * h) * ((t * 1.8 + 32) - 58);
    itu = int(i);
    ta = dht.computeHeatIndex(t, h, false); // Apparent temperature (heat index)

    // Display current time
    lcd.setCursor(0, 0);
    if (now.hour() < 10) lcd.print(" ");
    lcd.print(now.hour(), DEC);
    lcd.print(":");
    if (now.minute() < 10) lcd.print("0");
    lcd.print(now.minute(), DEC);
    lcd.print(":");
    if (now.second() < 10) lcd.print("0");
    lcd.print(now.second(), DEC);

    // Display ITU value
    lcd.setCursor(10, 0);
    lcd.print("ITU=");
    lcd.print(itu);

    // Display temperature and humidity
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(t, 1);  // Display temperature with one decimal place
    lcd.write(0b11011111);  // Degree symbol
    lcd.print("C  RH:");
    lcd.print(int(h));  // Display humidity without decimal places
    lcd.print("%");

    // LED indicators for ITU
    if (itu <= 65) 
    {
      // Comfort, turn on green LED
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_GALBEN, LOW);
      digitalWrite(LED_ROSU, LOW);
    } else if (itu <= 79) {
      // Alert, turn on yellow LED
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_GALBEN, HIGH);
      digitalWrite(LED_ROSU, LOW);
    } else {
      // Critical, turn on red LED
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_GALBEN, LOW);
      digitalWrite(LED_ROSU, HIGH);
    }
  }

  if (!digitalRead(SW0)) set_time();  // Hold the switch to set time
}

void set_time() 
{
  byte minutes = 0;
  byte hours = 0;
  while (!digitalRead(SW0)) 
  {  // Set time switch must be released to exit
    while (!digitalRead(SW1)) 
    {  // Set minutes
      minutes++;
      if ((minutes & 0x0F) > 9) minutes += 6;
      if (minutes > 0x59) minutes = 0;
      display_time(hours, minutes);
      delay(150);
    }

    while (!digitalRead(SW2)) 
    {  // Set hours
      hours++;
      if ((hours & 0x0F) > 9) hours += 6;
      if (hours > 0x23) hours = 0;
      display_time(hours, minutes);
      delay(150);
    }

    // Set the RTC with the new time
    Wire.beginTransmission(0x68);  // Activate DS1307
    Wire.write(0);                 // Where to begin
    Wire.write(0x00);              // Seconds
    Wire.write(minutes);           // Minutes
    Wire.write(0x80 | hours);      // Hours (24hr time)
    Wire.write(0x06);              // Day 01-07
    Wire.write(0x01);              // Date 0-31
    Wire.write(0x05);              // Month 0-12
    Wire.write(0x09);              // Year 00-99
    Wire.write(0x10);              // Control 0x10 produces a 1 HZ square wave on pin 7
    Wire.endTransmission();
  }
}

void display_time(byte hours, byte minutes) 
{
  int zh = hours / 16;
  int uh = hours - 16 * zh;
  int ore = 10 * zh + uh;
  int zm = minutes / 16;
  int um = minutes - 16 * zm;
  int miniti = 10 * zm + um;

  lcd.setCursor(0, 0);
  lcd.print(ore < 10 ? " " : "");
  lcd.print(ore);
  lcd.print(":");
  lcd.print(miniti < 10 ? "0" : "");
  lcd.print(miniti);
  lcd.print(":00");
}

// https://arduinotehniq.blogspot.com/2014/09/weather-station-manual-adjust-for-rtc.html#google_vignette
// https://ro.onetransistor.eu/2017/12/indice-confort-termic-dht11-arduino.html
// Credits and footnotes:
// http://learn.adafruit.com/ds1307-real-time-clock-breakout-board-kit/
// http://nicuflorica.blogspot.ro/
// https://docs.arduino.cc/
// http://pesd.ro/articole/nr.1/Teodoreanu.pdf
// https://starea-vremii.ro/blog/ce-este-indicele-de-confort-termic-heat-index-si-cum-se-calculeaza