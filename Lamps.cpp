#include "Lamps.h"

Lamps::Lamps(byte pinStart) {
   _pinStart = pinStart;
}

void Lamps::setup() {
   pinMode(_pinStart, OUTPUT);
}

void Lamps::onStart() {
   digitalWrite(_pinStart, HIGH);
}

void Lamps::onFalseStart() {
   
}

void Lamps::onTable(byte table) {
   
}

void Lamps::allOff() {
   digitalWrite(_pinStart, LOW);
}
