#ifndef PREFERENCE_H_
#define PREFERENCE_H_

// ===== GSM (SIM800/900) =====
#include <SoftwareSerial.h>
#include <String.h>
static SoftwareSerial gprsSerial(36, 37);   // RX, TX (Mega)

// ===== TFT + Touch (shield 3.2" MCUFRIEND) =====
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <MCUFRIEND_kbv.h>
static MCUFRIEND_kbv tft;

// Touch calib (exemple – à recaler si besoin)
#define TS_MINX 122
#define TS_MINY 111
#define TS_MAXX 942
#define TS_MAXY 890

// Brochage tactile du shield
#define YP A3
#define XM A2
#define YM 9
#define XP 8
static TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Couleurs 16 bits
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// ===== RTC + SD =====
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
static RTC_DS1307 rtc;
static File data_DHT;
static int  k = 1;

// ===== Débitmètre (hall) =====
// Mega interruptions valides: 2,3,18,19,20,21
#define SENSOR_INTERRUPT_PIN 18           // entrée impulsions
#define SENSOR_ENABLE_PIN    18           // si tu as besoin d’alimenter/activer le capteur
#define sensorPinOn          digitalWrite(SENSOR_ENABLE_PIN, HIGH)

static volatile byte pulseCount = 0;
static float  calibrationFactor = 4.5f;   // ~4.5 pulses/s par L/min
static float  flowRate = 0.0f;
static unsigned int  flowMilliLitres = 0;
static unsigned long oldTime = 0;

// ===== DS18B20 (temp eau) =====
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 48
static OneWire oneWire(ONE_WIRE_BUS);
static DallasTemperature sensors(&oneWire);

// ===== pH & EC =====
#define EC_PIN A14
#define PH_PIN A13
static float ecVoltage, phVoltage, phValue, ecValue, temperature;
static float ec_mean_value, ph_mean_value;
static float acidVoltage    = 2373.0f;    // à calibrer
static float neutralVoltage = 1777.3f;    // à calibrer
static float pH_max = 7.0f, pH_min = 6.0f, EC_max = 3.0f, EC_min = 2.0f;
static unsigned long timepoint = 0, previousTime = 0, pause_time = 0;
static int z = 0;

// ===== DHT22 (air) =====
#include <DHT.h>
#define DHT_PIN 49
#define DHTTYPE DHT22
static DHT dht(DHT_PIN, DHTTYPE);
static float h = 0.0f, t = 0.0f;

// ===== Flotteurs =====
#define SENSOR_LOW   23
#define SENSOR_HIGH  22
#define down_motion  (digitalRead(SENSOR_LOW)  == LOW)
#define up_motion    (digitalRead(SENSOR_HIGH) == LOW)

// ===== Actionneurs =====
#define PUMP_PIN   33
#define pumpOn     digitalWrite(PUMP_PIN, LOW)
#define pumpOff    digitalWrite(PUMP_PIN, HIGH)

#define MIXER_PIN  29
#define mixerOn    digitalWrite(MIXER_PIN, LOW)
#define mixerOff   digitalWrite(MIXER_PIN, HIGH)

#define VALVE_PIN  32
#define valveOn    digitalWrite(VALVE_PIN, LOW)
#define valveOff   digitalWrite(VALVE_PIN, HIGH)

#define ACID_PIN   26
#define acidOn     digitalWrite(ACID_PIN, LOW)
#define acidOff    digitalWrite(ACID_PIN, HIGH)

#define BASE_PIN   25
#define baseOn     digitalWrite(BASE_PIN, LOW)
#define baseOff    digitalWrite(BASE_PIN, HIGH)

#define WAT_PIN    27
#define watOn      digitalWrite(WAT_PIN, LOW)
#define watOff     digitalWrite(WAT_PIN, HIGH)

#define NUT_PIN    24
#define nutOn      digitalWrite(NUT_PIN, LOW)
#define nutOff     digitalWrite(NUT_PIN, HIGH)

// ===== ISR compteur d’impulsions =====
inline void pulseCounter() {
  pulseCount++;
}

#endif // PREFERENCE_H_
