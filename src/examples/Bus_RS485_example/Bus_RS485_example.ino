//#include "NextPM.h"
#include <NextPM_RS485.h>


NextPM_RS485 pm(Serial1, 1, 22);   // Serial1, slaveId=1, RE/DE sur D8

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pm.configure();
}

void loop() {
  PM_DATA data;
  uint16_t status = 0;

  if (pm.read_1min_rs485(data)) {
    Serial.print("PM1.0 => ");
    Serial.print(data.PM_UG_1_0, 3);
    Serial.print("  PM2.5 => ");
    Serial.print(data.PM_UG_2_5, 3);
    Serial.print("  PM10 => ");
    Serial.println(data.PM_UG_10_0, 3);
  } else {
    Serial.println("Lecture 1 min echouee");
  }

  if (pm.readStatus(status)) {
    Serial.print("Status = 0x");
    Serial.println(status, HEX);
  } else {
    Serial.println("Lecture status echouee");
  }

  delay(2000);
}