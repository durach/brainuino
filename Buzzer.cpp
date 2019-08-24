#include "Buzzer.h"

Buzzer::Buzzer(byte pin) {
  _pin = pin;
}

void Buzzer::playStartSound() {
  tone(_pin, BUZZER_FREQUENCY_START, BUZZER_SOUND_DURATION);
}

void Buzzer::playTableSound() {
  tone(_pin, BUZZER_FREQUENCY_TABLE, BUZZER_SOUND_DURATION);
}

void Buzzer::playFalseStartSound() {
  tone(_pin, BUZZER_FREQUENCY_FALSE_START, BUZZER_SOUND_DURATION);
}

void Buzzer::off() {
  noTone(_pin);
}
