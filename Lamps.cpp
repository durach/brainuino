#include "Lamps.h"

Lamps::Lamps(byte pinStart, byte pinTable[]) {
   _pinStart = pinStart;
   
   // TODO find out how to copy
  for (byte i = 0; i < MAX_TABLES; i++) {
    _pinTable[i] = pinTable[i];
  }
}

void Lamps::setup() {
  pinMode(_pinStart, OUTPUT);
  for (byte i = 0; i < MAX_TABLES; i++) {
    pinMode(_pinTable[i], OUTPUT); 
  }
}

void Lamps::onStart() {
   digitalWrite(_pinStart, HIGH);
}

void Lamps::onTable(byte table) {
  digitalWrite(_pinTable[table-1], HIGH);
  Serial.print(" On");
  Serial.println(_pinTable[table-1]);
}

void Lamps::allOff() {
  digitalWrite(_pinStart, LOW);
  for (byte i = 0; i < MAX_TABLES; i++) {
    Serial.print(" Off");
    Serial.println(_pinTable[i]);
    digitalWrite(_pinTable[i], LOW);
  }
}