#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin) {
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

void Buzzer::playFinishingSound() {
  tone(_pin, BUZZER_FREQUENCY_FINISHING, BUZZER_SOUND_DURATION);
}

void Buzzer::playFinishSound() {
  tone(_pin, BUZZER_FREQUENCY_FINISH, BUZZER_SOUND_DURATION);
}

void Buzzer::off() {
  noTone(_pin);
}
