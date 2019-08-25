#include "Lamps.h"

Lamps::Lamps(byte pinStart, byte pinTable[]) {
  _pinStart = pinStart;
  _pinTable = pinTable;
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
}

void Lamps::allOff() {
  digitalWrite(_pinStart, LOW);
  for (byte i = 0; i < MAX_TABLES; i++) {
    digitalWrite(_pinTable[i], LOW);
  }
}
