#include "MyLib.h"

void setup() {
  MyLib.begin();
}

void loop() {
  MyLib.datalog();
  delay(10000);   // attends 10 secondes avant de relancer
}