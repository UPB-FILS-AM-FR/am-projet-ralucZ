// Raluca Zarnescu. Project: Clock with RTC1307, ITU calculator with DHT11, Air Quality Monitor with MQ135, and bargraphs with LED WS2812 (addressable LED Neopixel).
// ITU: " indice de confort termic (thermal confort index) based on values detected by DHT11 sensor for temperature and relative humidity.
// ITU below 65 units: safe.
//ITU between 65 and 79 units: alert.
// ITU 80 units and above: critical, unsafe.

//AQ: air quality (not AQI). Based on values detected by MQ135 sensor.
//MQ 135 detection capabilities:
//- Toxic or potential toxic gases (ammonia, hydrogen sulphide, carbon dioxide, nitrogen oxides etc)
//- Combustible and explosive gases (LPG, benzene, toluene, propane, butane and other VOCs)
//- Smoke and other fine particles in suspension.

#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

constexpr uint8_t DHTPIN = 2;
constexpr uint8_t DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

#define LED_PIN     3
#define LED_COUNT   12

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

RTC_DS1307 RTC;

constexpr uint8_t LED_VERDE = 11;
constexpr uint8_t LED_GALBEN = 12;
constexpr uint8_t LED_ROSU = 13;
constexpr uint8_t LED_AIR_QUALITY = 10;

constexpr byte SW0 = A0;
constexpr byte SW1 = A1;
constexpr byte SW2 = A2;

float t = 0.0, h = 0.0, ta = 0.0;
int itu = 0;

constexpr uint8_t MQ135_PIN = A3;
int airQuality = 0;
int previousAirQuality = -1;

unsigned long previousMillis = 0;
unsigned long airQualityPreviousMillis = 0;
unsigned long displayTogglePreviousMillis = 0;
const long displayToggleInterval = 2000;
const long rtcUpdateInterval = 1000;
int toggleCounter = 0;

const long airQualityLEDInterval = 15000;
bool displayITU = true;

bool timeSettingMode = false;
byte hours = 0, minutes = 0, seconds = 0;

void setup() {
  strip.begin();
  strip.show();

  functionalCheck();

  dht.begin();
  
  lcd.begin(16, 2);
  lcd.print("Raluca  Zarnescu");
  lcd.setCursor(0, 1);
  lcd.print("Clock_RTC_ITU_AQ");
  delay(2500);
  lcd.clear();

  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x07);
  Wire.write(0x10);
  Wire.endTransmission();
  RTC.begin();
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  pinMode(SW0, INPUT_PULLUP);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_GALBEN, OUTPUT);
  pinMode(LED_ROSU, OUTPUT);
  pinMode(LED_AIR_QUALITY, OUTPUT);

  testLEDs();
  
  Serial.begin(9600);

  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (timeSettingMode) {
    lcd.setCursor(0, 1);
    if (hours < 10) lcd.print("0");
    lcd.print(hours);
    lcd.print(":");
    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print(":00");

    if (digitalRead(SW2) == LOW) {  // Increment hours on SW2 press
      hours = (hours + 1) % 24;
      delay(200);  // Debounce delay
    }
    if (digitalRead(SW1) == LOW) {  // Increment minutes on SW1 press
      minutes = (minutes + 1) % 60;
      delay(200);  // Debounce delay
    }
  } else {
    if (currentMillis - previousMillis >= rtcUpdateInterval) {
      previousMillis = currentMillis;
      updateRTCDisplay();
    }

    if (currentMillis - displayTogglePreviousMillis >= displayToggleInterval) {
      displayTogglePreviousMillis = currentMillis;
      toggleCounter++;
      updateDisplayAndCalculateValues();
    }

    if (airQuality > 250) {
      digitalWrite(LED_AIR_QUALITY, HIGH);
      airQualityPreviousMillis = currentMillis;
    } else if (currentMillis - airQualityPreviousMillis <= airQualityLEDInterval) {
      digitalWrite(LED_AIR_QUALITY, HIGH);
    } else {
      digitalWrite(LED_AIR_QUALITY, LOW);
    }
  }

  if (digitalRead(SW0) == LOW) {
    delay(200);  // Debounce delay
    if (timeSettingMode) {
      RTC.adjust(DateTime(2024, 5, 25, hours, minutes, 0));
      timeSettingMode = false;
      lcd.clear();
    } else {
      timeSettingMode = true;
      hours = 0;
      minutes = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Setare ceas:");
      lcd.setCursor(0, 1);
      lcd.print("00:00:00");
    }
  }
}

void testLEDs() {
  digitalWrite(LED_VERDE, HIGH);
  delay(300);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_GALBEN, HIGH);
  delay(300);
  digitalWrite(LED_GALBEN, LOW);
  digitalWrite(LED_ROSU, HIGH);
  delay(300);
  digitalWrite(LED_ROSU, LOW);
  digitalWrite(LED_AIR_QUALITY, HIGH);
  delay(300);
  digitalWrite(LED_AIR_QUALITY, LOW);
}

void updateRTCDisplay() {
  DateTime now = RTC.now();
  lcd.setCursor(0, 0);
  if (now.hour() < 10) lcd.print(" ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  if (now.second() < 10) lcd.print("0");
  lcd.print(now.second(), DEC);
}

void updateDisplayAndCalculateValues() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  airQuality = analogRead(MQ135_PIN);
  
  Serial.print("Air Quality: ");
  Serial.println(airQuality);

  if (isnan(h) || isnan(t)) {
    lcd.setCursor(0, 1);
    lcd.print("Sensor error");
    return;
  }

  float i = (t * 1.8 + 32) - (0.55 - 0.0055 * h) * ((t * 1.8 + 32) - 58);
  itu = int(i);
  ta = dht.computeHeatIndex(t, h, false);

  if (toggleCounter % 2 == 0) {
    lcd.setCursor(10, 0);
    lcd.print("ITU=");
    lcd.print(itu);
  } else {
    lcd.setCursor(10, 0);
    lcd.print("AQ=    ");
    lcd.setCursor(13, 0);
    lcd.print(airQuality);

    Serial.print("Displayed AQ: ");
    Serial.println(airQuality);
  }
  
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(t, 1);
  lcd.write(0b11011111);
  lcd.print("C  RH:");
  lcd.print(int(h));
  lcd.print("%");

  updateLEDs();
  updateNeoPixels();
}

void updateLEDs() {
  if (itu <= 65) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_GALBEN, LOW);
    digitalWrite(LED_ROSU, LOW);
  } else if (itu <= 79) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_GALBEN, HIGH);
    digitalWrite(LED_ROSU, LOW);
  } else {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_GALBEN, LOW);
    digitalWrite(LED_ROSU, HIGH);
  }
}

void updateNeoPixels() {
  // Clear all NeoPixels before setting new values
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off all NeoPixels
  }

  // Colors for each NeoPixel
  uint32_t colors[LED_COUNT] = {
    strip.Color(48, 0, 48),    // Purple
    strip.Color(96, 0, 0),     // Red
    strip.Color(96, 16, 0),    // Orange
    strip.Color(72, 48, 0),    // Yellow
    strip.Color(48, 72, 0),    // Yellowish Green
    strip.Color(0, 96, 0),     // Green
    strip.Color(0, 96, 0),     // Green
    strip.Color(48, 72, 0),    // Yellowish Green
    strip.Color(72, 48, 0),    // Yellow
    strip.Color(96, 16, 0),    // Orange
    strip.Color(96, 0, 0),     // Red
    strip.Color(48, 0, 48)     // Purple
  };

  // ITU value determines how many LEDs are lit from NeoPixel 5 to NeoPixel 0
  int itulimit = 0;
  if (itu >= 88) {
    itulimit = 6;
  } else if (itu >= 80) {
    itulimit = 5;
  } else if (itu >= 73) {
    itulimit = 4;
  } else if (itu >= 66) {
    itulimit = 3;
  } else if (itu >= 58) {
    itulimit = 2;
  } else {
    itulimit = 1;
  }

  // Update ITU NeoPixels from 5 to 0
  for (int i = 5; i >= 6 - itulimit; i--) {
    strip.setPixelColor(i, colors[i]);
  }

  // Air quality value determines how many LEDs are lit from NeoPixel 6 to NeoPixel 11
  int aqlimit = 0;
  if (airQuality > 500) {
    aqlimit = 6;
  } else if (airQuality > 400) {
    aqlimit = 5;
  } else if (airQuality > 300) {
    aqlimit = 4;
  } else if (airQuality > 200) {
    aqlimit = 3;
  } else if (airQuality > 100) {
    aqlimit = 2;
  } else {
    aqlimit = 1;
  }

  // Update Air Quality NeoPixels from 6 to 11
  for (int i = 6; i <= 5 + aqlimit; i++) {
    strip.setPixelColor(i, colors[i]);
  }

  strip.show();  // Update the NeoPixels with the new colors
}

void functionalCheck() {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(128, 0, 0)); // Red
    strip.show();
    delay(100);
  }
  delay(500);
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 128, 0)); // Green
    strip.show();
    delay(100);
  }
  delay(500);
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 128)); // Blue
    strip.show();
    delay(100);
  }
  delay(500);
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off all NeoPixels
  }
  strip.show();
}

// Credits and footnotes:
// https://arduinotehniq.blogspot.com/2014/09/weather-station-manual-adjust-for-rtc.html#google_vignette
// https://ro.onetransistor.eu/2017/12/indice-confort-termic-dht11-arduino.html
// http://learn.adafruit.com/ds1307-real-time-clock-breakout-board-kit/
// http://nicuflorica.blogspot.ro/
// https://docs.arduino.cc/
// http://pesd.ro/articole/nr.1/Teodoreanu.pdf
// https://starea-vremii.ro/blog/ce-este-indicele-de-confort-termic-heat-index-si-cum-se-calculeaza
// https://www.winsen-sensor.com/product/mq135.html
// https://www.hackster.io/m_karim02/arduino-and-mq-135-gas-sensor-with-arduino-code-a8c1c6
// https://www.makerguides.com/air-pollution-monitoring-and-alert-system-using-arduino-and-mq135/
// https://www.instructables.com/How-to-Control-WS2812-RGB-LED-NeoPixel-W-Arduino-T/
// https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use