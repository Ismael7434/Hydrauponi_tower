#include "Preference.h"
#include "MyLib.h"

// ------------------------
//  Implémentation MyLib
// ------------------------

void MyLib_::begin() {
  gprsSerial.begin(9600);
  Serial.begin(9600);
  sensors.begin();
  dht.begin();
  rtc.begin();
  SD.begin(53);

  while (!Serial) { ; }               // pour les cartes USB natives
  Serial.print("Initializing SD card...");

  while (!SD.begin(53)) {
    Serial.println("Card failed, or not present");
    while (1) ;                        // bloque si pas de carte
  }
  Serial.println("card initialized.");

  // Fichier CSV
  data_DHT = SD.open("Data_DHT.csv", FILE_WRITE);
  data_DHT.println("N,Date,Time,Humidity,AirTemp,WaterTemp,Flow,pH,EC");
  data_DHT.close();

  // LCD
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(3);
  tft.setFont(&FreeSerif9pt7b);
  tft.fillScreen(RED);
  tft.setCursor(50, 40);
  tft.setTextSize(3);
  tft.println("Welcome \n to your \n hydroponic\n farm");

  // E/S
  pinMode(SENSOR_LOW,        INPUT_PULLUP);
  pinMode(SENSOR_HIGH,       INPUT_PULLUP);
  pinMode(SENSOR_INTERRUPT_PIN, INPUT_PULLUP);
  pinMode(SENSOR_ENABLE_PIN, OUTPUT);
  sensorPinOn;

  pinMode(PUMP_PIN,  OUTPUT);  pumpOff;
  pinMode(VALVE_PIN, OUTPUT);  valveOff;
  pinMode(MIXER_PIN, OUTPUT);  mixerOff;
  pinMode(ACID_PIN,  OUTPUT);  acidOff;
  pinMode(BASE_PIN,  OUTPUT);  baseOff;
  pinMode(WAT_PIN,   OUTPUT);  watOff;
  pinMode(NUT_PIN,   OUTPUT);  nutOff;

  delay(10000);

  if (up_motion) {
    Serial.println("Upper Float Switch down");
    pumpOn;
    while (up_motion) {
      Serial.println("Pump On");
      delay(2000);
    }
    pumpOff;
    Serial.println("Pump Off and mixer On");
  }

  delay(10000);
  mixerOn;
  delay(90000);
  Serial.println("Wait for a minute and half");
  mixerOff;
  Serial.println("Wait for a minute");
  delay(60000);

  water_quality();
  delay(10000);
}

// Température eau
float MyLib_::wt_sensor() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

// Débitmètre (mesure ~1s)
float MyLib_::Flow_meter() {
  pulseCount = 0;
  oldTime = millis();
  attachInterrupt(digitalPinToInterrupt(SENSOR_INTERRUPT_PIN), pulseCounter, FALLING);
  delay(1000);
  detachInterrupt(digitalPinToInterrupt(SENSOR_INTERRUPT_PIN));

  unsigned long dt = millis() - oldTime;           // ms
  if (dt == 0) dt = 1;

  // pulses/s
  float pulsesPerSecond = (1000.0f * pulseCount) / (float)dt;
  // L/min = pulses/s / calibrationFactor
  float litresPerMinute = pulsesPerSecond / calibrationFactor;
  // mL/s
  flowMilliLitres = (unsigned int)((litresPerMinute / 60.0f) * 1000.0f);
  return flowMilliLitres;
}

// EC
float MyLib_::ec_sensor() {
  int i = 0;
  ec_mean_value = 0;
  float ec_sum = 0;

  while (i < 10) {
    if (millis() - timepoint > 1000U) {
      timepoint = millis();
      temperature = wt_sensor();
      ecVoltage = analogRead(EC_PIN) * 5.0f / 1024.0f;
      ecValue   = ecVoltage + 0.027f * (temperature - 25.0f);   // compensation basique
      i++;
      ec_sum += ecValue;
    }
  }
  ec_mean_value = ec_sum / 10.0f;
  Serial.print("EC_Mean Value: ");
  Serial.println(ec_mean_value);
  return ec_mean_value;
}

// pH
float MyLib_::ph_sensor() {
  int j = 0;
  ph_mean_value = 0;
  float ph_sum = 0;

  while (j < 10) {
    if (millis() - timepoint > 2000U) {
      timepoint = millis();
      temperature = wt_sensor();
      phVoltage = analogRead(PH_PIN) / 1024.0f * 5000.0f;

      float slope     = (7.0f - 4.0f) / ((neutralVoltage - 1500.0f) / 3.0f - (acidVoltage - 1500.0f) / 3.0f);
      float intercept = 7.0f - slope * (neutralVoltage - 1500.0f) / 3.0f;
      phValue = slope * (phVoltage - 1500.0f) / 3.0f + intercept;

      Serial.print("\tpH: ");
      Serial.println(phValue);

      j++;
      ph_sum += phValue;
    }
  }
  ph_mean_value = ph_sum / 10.0f;
  Serial.print("PH_Mean Value: ");
  Serial.println(ph_mean_value);
  return ph_mean_value;
}

// Correction EC/pH
void MyLib_::water_quality() {
  if (z == 0) {
    flowRate = Flow_meter();
    delay(5000);
    ecValue = ec_sensor();
    delay(5000);
    phValue = ph_sensor();
    delay(2000);
    h = dht.readHumidity();
    t = dht.readTemperature();
    delay(5000);
    touch();
  }

  int p = 0;
  while (ecValue < EC_min || ecValue > EC_max || phValue < pH_min || phValue > pH_max) {
    if (phValue < pH_min) {
      baseOn; delay(5000); baseOff;
    } else if (phValue > pH_max) {
      acidOn; delay(5000); acidOff;
    } else if (ecValue < EC_min) {
      nutOn;  delay(60000); nutOff; p++;
    } else {
      watOn;  delay(60000); watOff; p++;
    }

    if (p == 8) {
      valveOn; delay(15000); valveOff; p = 0;
    }

    mixing();
    flowRate = Flow_meter();
    delay(5000);
    ecValue = ec_sensor();
    delay(5000);
    phValue = ph_sensor();
    delay(2000);
    h = dht.readHumidity();
    t = dht.readTemperature();
    delay(5000);
  }
}

void MyLib_::mixing() {
  mixerOn;  delay(150000);
  mixerOff; delay(120000);

  if (millis() - previousTime >= 300000U) {
    previousTime = millis();
    card();
    touch();
    serial();
    online();
    k++;
  }
}

void MyLib_::datalog() {
  if (millis() - previousTime >= 300000U) {
    previousTime = millis();

    if (!up_motion) {
      valveOn;
      Serial.println("Valve On");
    }

    flowRate = Flow_meter();  delay(5000);
    ecValue  = ec_sensor();   delay(5000);
    phValue  = ph_sensor();   delay(2000);
    h = dht.readHumidity();
    t = dht.readTemperature();  delay(5000);

    card();
    touch();
    serial();
    online();
    k++;

    pause();
    delay(5000);
  }
}

void MyLib_::pause() {
  if (down_motion) {
    Serial.println("Lower Float Switch down");
    valveOff; delay(5000);
    pumpOn;   Serial.println("Valve Off and pump On");

    while (up_motion) {
      if (millis() - previousTime >= 300000U) {
        previousTime = millis();
        flowRate = Flow_meter(); delay(5000);
        ecValue  = ec_sensor();  delay(5000);
        phValue  = ph_sensor();  delay(2000);
        h = dht.readHumidity();
        t = dht.readTemperature(); delay(5000);

        card(); touch(); serial(); online(); k++;
      }
    }

    pumpOff; Serial.println("Pump Off");
    z = 1;
    pause_time = millis();

    while (millis() - pause_time <= 1800000U) {
      if (millis() - previousTime >= 300000U) {
        previousTime = millis();
        flowRate = Flow_meter(); delay(5000);
        ecValue  = ec_sensor();  delay(5000);
        phValue  = ph_sensor();  delay(2000);
        h = dht.readHumidity();
        t = dht.readTemperature(); delay(5000);

        if (ecValue < EC_min || ecValue > EC_max || phValue < pH_min || phValue > pH_max) {
          water_quality();
        } else {
          card(); touch(); serial(); online(); k++;
        }
      }
    }
  }
}

void MyLib_::card() {
  data_DHT = SD.open("Data_DHT.csv", FILE_WRITE);
  DateTime now = rtc.now();

  data_DHT.print(k); data_DHT.print(',');
  data_DHT.print(now.year(), DEC); data_DHT.print('/');
  data_DHT.print(now.month(), DEC); data_DHT.print('/');
  data_DHT.print(now.day(), DEC); data_DHT.print(", ");
  data_DHT.print(now.hour(), DEC);   data_DHT.print(':');
  data_DHT.print(now.minute(), DEC); data_DHT.print(':');
  data_DHT.print(now.second(), DEC); data_DHT.print(',');

  data_DHT.print(h);            data_DHT.print(',');
  data_DHT.print(t);            data_DHT.print(',');
  data_DHT.print(temperature);  data_DHT.print(',');
  data_DHT.print(phValue);      data_DHT.print(',');
  data_DHT.println(flowMilliLitres, DEC);

  data_DHT.close();
}

void MyLib_::touch() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.fillRect(0, 0, 320, 40, RED);
  tft.setTextSize(3);
  tft.setCursor(35, 35);
  tft.println("Hydroponic");

  tft.setTextSize(2);
  tft.setCursor(0, 70);   tft.println("Air Temp ");
  tft.setCursor(260, 70); tft.println(" C ");

  tft.setCursor(0, 110);  tft.println("Air Hum ");
  tft.setCursor(260, 110);tft.println(" %");

  tft.setCursor(0, 150);  tft.println("W Temp ");
  tft.setCursor(260, 150);tft.println(" C ");

  tft.setCursor(0, 190);  tft.println("pH ");
  tft.setCursor(0, 230);  tft.println("EC ");

  tft.setCursor(180, 70);  tft.println(t);
  tft.setCursor(180, 110); tft.println(h);
  tft.setCursor(180, 150); tft.println(temperature);
  tft.setCursor(180, 190); tft.println(phValue);
  tft.setCursor(180, 230); tft.println(ecValue);
}

void MyLib_::serial() {
  Serial.print("Humidite: ");   Serial.print(h);
  Serial.print(" \tTemperature: "); Serial.println(t);
  Serial.print("Water temp: "); Serial.print(temperature);
  Serial.print(" C \tpH: ");     Serial.print(phValue);
  Serial.print(" \tEC: ");       Serial.println(ecValue);
  Serial.print("Flow Rate: ");   Serial.print(flowMilliLitres, DEC);
  Serial.println(" mL/s ");
}

void MyLib_::online() {
  if (gprsSerial.available()) Serial.write(gprsSerial.read());

  gprsSerial.println("AT");          delay(1000);
  gprsSerial.println("AT+CPIN?");    delay(1000);
  gprsSerial.println("AT+CREG?");    delay(1000);
  gprsSerial.println("AT+CGATT?");   delay(1000);
  gprsSerial.println("AT+CIPSHUT");  delay(1000);
  gprsSerial.println("AT+CIPSTATUS");delay(2000);
  gprsSerial.println("AT+CIPMUX=0"); delay(2000);
  ShowSerialData();

  gprsSerial.println("AT+CSTT=\"internet\""); delay(1000); ShowSerialData();
  gprsSerial.println("AT+CIICR");             delay(3000); ShowSerialData();
  gprsSerial.println("AT+CIFSR");             delay(2000); ShowSerialData();
  gprsSerial.println("AT+CIPSPRT=0");         delay(3000); ShowSerialData();

  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(6000); ShowSerialData();

  gprsSerial.println("AT+CIPSEND"); delay(4000); ShowSerialData();

  String str = String("GET https://api.thingspeak.com/update?api_key=K5JCYN4OO7KGGD1C")
               + "&field1=" + String(temperature)
               + "&field2=" + String(t)
               + "&field3=" + String(ecValue)
               + "&field4=" + String(phValue)
               + "&field5=" + String(h);

  Serial.println(str);
  gprsSerial.println(str);

  delay(4000); ShowSerialData();

  gprsSerial.println((char)26);   // CTRL+Z
  delay(5000);
  gprsSerial.println();
  ShowSerialData();
  gprsSerial.println("AT+CIPSHUT"); delay(100); ShowSerialData();
}

void MyLib_::ShowSerialData() {
  while (gprsSerial.available() != 0) {
    Serial.write(gprsSerial.read());
  }
}

MyLib_& MyLib_::getInstance() {
  static MyLib_ instance;
  return instance;
}

MyLib_& MyLib = MyLib_::getInstance();
