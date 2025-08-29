# Hydrauponi_tower
AI-based hydroponic tower monitoring and control system using Arduino Mega.

# 🌱 AI-based Hydroponic Tower

This project is an **Arduino Mega 2560**-based hydroponic tower for monitoring and controlling water quality, pH, EC, flow rate, air temperature, and humidity.  
It integrates **sensors, actuators, and an AI-based prediction model** for optimal plant growth.

---

## 🚀 Features
- pH and EC measurement and automatic correction
- Water temperature and flow rate monitoring
- Air temperature and humidity monitoring (DHT22)
- SD card logging (CSV format)
- TFT LCD display with real-time values
- GSM module for IoT data upload (ThingSpeak)
- AI module (ANN) for prediction of pH and EC

---

## 🛠️ Hardware
- Arduino Mega 2560
- DHT22 (air temperature and humidity)
- DS18B20 (water temperature)
- pH sensor
- EC sensor
- Flow meter
- Float switches
- Peristaltic pumps (acid, base, nutrients, water)
- TFT LCD with touch screen
- GSM module (SIM800/900)

---

## 📂 Project Structure
AI_based_hydroponic_tower/
│── ANNImplementationcode.ino # ANN prediction model
│── AI_based_hydroponic_tower.ino # Main Arduino sketch
 │── MyLib.cpp # Library implementation
 │── MyLib.h # Library header
 │── Preference.h # Pin mapping & config
 
## 📊 Data Logging
- Data is stored on SD card (`Data_DHT.csv`)
- Fields: `N, Date, Time, Humidity, Temperature, Water Temp, Flow, pH, EC`

## 📡 IoT Integration
- Data is uploaded to **ThingSpeak** via GSM module  
- API Key must be configured in `MyLib.cpp`
  
## 🤖 AI Prediction Module
- Implemented in `ANNImplementationcode.ino`
- Inputs: historical values of pH, EC, temperature, humidity, water temp, and flow
- Outputs: predicted **pH** and **EC**
- ANN equations are pre-trained and embedded as weight matrices.
  
## 🛠️ Installation & Usage
1. Clone this repository:
 ```bash
 git clone https://github.com/Ismael7434/Hydroponic_tower.git

Open AI_based_hydroponic_tower.ino in Arduino IDE.

Install required libraries:

MCUFRIEND_kbv

Adafruit_GFX

TouchScreen

DHT

OneWire

DallasTemperature

RTClib

SD

Select Arduino Mega 2560 as board.

Upload code and monitor output via Serial Monitor (9600 baud).

System Overview

Sensors collect environmental and water quality data.

Data is displayed on TFT screen and logged on SD card.

ANN predicts future pH and EC trends.

Actuators (pumps, mixer, valves) are triggered to maintain optimal levels.

Data is uploaded to ThingSpeak IoT platform.

👨‍💻 Author

Ismael Moule

Project: AI-based Hydroponic Tower

Year: 2024
+33 745441588
diakiteismael7434@gmail.com




