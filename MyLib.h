#pragma once
#include <Arduino.h>

class MyLib_ {
public:
  static MyLib_& getInstance();

  void begin();
  void datalog();
  void water_quality();
  void ShowSerialData();
  void card();
  void serial();
  void touch();
  void online();
  void pause();
  void mixing();

  float wt_sensor();
  float Flow_meter();
  float ec_sensor();
  float ph_sensor();

private:
  MyLib_() = default;
  MyLib_(const MyLib_&) = delete;
  MyLib_& operator=(const MyLib_&) = delete;
};

extern MyLib_& MyLib;
