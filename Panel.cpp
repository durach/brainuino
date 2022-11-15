#include "Panel.h"
#include "TM1637Display.h"

const uint8_t SEG_ERR[] = {
  SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,    // E 
  SEG_G | SEG_E,                            // r
  SEG_G | SEG_E,                            // r
  0,
};

const uint8_t SEG_SG[] = {
  0,
  SEG_A | SEG_F | SEG_E | SEG_D,           // С 
  SEG_A | SEG_F | SEG_E,                   // Г
  0,
};

const uint8_t SEG_BR[] = {
  0,
  SEG_A | SEG_F | SEG_E | SEG_D | SEG_C | SEG_G,  // Б
  SEG_G | SEG_B | SEG_A | SEG_F | SEG_E,          // Р
  0,
};


Panel::Panel(int8_t pinCLK, int8_t pinDIO)  : _tm(pinCLK, pinDIO, 100) {
  _pinCLK = pinCLK;
  _pinDIO = pinDIO;
}

void Panel::setup(int8_t mode) {
  _tm.setBrightness(PANEL_BRIGHTNESS);
  _tm.clear();
  if (mode == MODE_BR) {
    _tm.setSegments(SEG_BR);
    delay(PANEL_SPLASH_DURATION);
    _tm.showNumberDecEx(0, 0b01000000, true); // 00:00
  } else {
    _tm.setSegments(SEG_SG);
    delay(PANEL_SPLASH_DURATION);
    _tm.clear();
  }
  
}

void Panel::displayTime(float time) {
  _tm.showNumberDecEx(time * 100, 0b01000000, true);
}

void Panel::displayFalseStart() {
  _tm.clear();
  _tm.showNumberHexEx(0xFA, 0, false, 2, 1);
}

void Panel::off() {
  _tm.clear();
}

void Panel::error(int8_t number) {
  _tm.clear();
  _tm.setSegments(SEG_ERR);
  _tm.showNumberDec(number, false, 1, 3);
}
