#include "Lamps.h"

Lamps::Lamps(int8_t pinStart, int8_t pinStart60, int8_t pinStart20, int8_t pinTable[]) {
  this->_pinStart = pinStart;
  this->_pinStart60 = pinStart60;
  this->_pinStart20 = pinStart20;
  this->_pinTable = pinTable;
}

void Lamps::setup() {
  pinMode(this->_pinStart, OUTPUT);
  pinMode(this->_pinStart60, OUTPUT);
  pinMode(this->_pinStart20, OUTPUT);
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

void Lamps::onStart60() {
  digitalWrite(this->_pinStart60, HIGH);
}

void Lamps::offStart60() {
  digitalWrite(this->_pinStart60, LOW);
}

void Lamps::onStart20() {
  digitalWrite(this->_pinStart20, HIGH);
}

void Lamps::offStart20() {
  digitalWrite(this->_pinStart20, LOW);
}

void Lamps::onTable(int8_t table) {
  digitalWrite(this->_pinTable[table], HIGH);
}

void Lamps::allOff() {
  this->offStart();
  this->offStart60();
  this->offStart20();
  for (int8_t i = 0; i < MAX_TABLES; i++) {
    digitalWrite(this->_pinTable[i], LOW);
  }
}
