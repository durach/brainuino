#include "Lamps.h"

Lamps::Lamps(byte pinStart, byte pinTable[]) {
  this->_pinStart = pinStart;
  this->_pinTable = pinTable;
}

void Lamps::setup() {
  pinMode(this->_pinStart, OUTPUT);
  for (byte i = 0; i < MAX_TABLES; i++) {
    pinMode(this->_pinTable[i], OUTPUT); 
  }
}

void Lamps::onStart() {
  digitalWrite(this->_pinStart, HIGH);
  this->isStartOn = true;
}

void Lamps::offStart() {
  digitalWrite(this->_pinStart, LOW);
  this->isStartOn = true;
}

void Lamps::onTable(byte table) {
  digitalWrite(this->_pinTable[table], HIGH);
}

void Lamps::allOff() {
  this->offStart();
  for (byte i = 0; i < MAX_TABLES; i++) {
    digitalWrite(this->_pinTable[i], LOW);
  }
}
