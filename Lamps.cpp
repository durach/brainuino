#include "Lamps.h"

Lamps::Lamps(int8_t pinStart, int8_t pinTable[]) {
  this->_pinStart = pinStart;
  this->_pinTable = pinTable;
}

void Lamps::setup() {
  pinMode(this->_pinStart, OUTPUT);
  for (int8_t i = 0; i < MAX_TABLES; i++) {
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

void Lamps::onTable(int8_t table) {
  digitalWrite(this->_pinTable[table], HIGH);
}

void Lamps::allOff() {
  this->offStart();
  for (int8_t i = 0; i < MAX_TABLES; i++) {
    digitalWrite(this->_pinTable[i], LOW);
  }
}
