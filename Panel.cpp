#include "Panel.h"
#include "TM1637Display.h"

Panel::Panel(byte pinCLK, byte pinDIO)  : _tm(pinCLK, pinDIO) {
  _pinCLK = pinCLK;
  _pinDIO = pinDIO;
}

void Panel::setup() {
  _tm.setBrightness(PANEL_BRIGHTNESS);
  _tm.showNumberDecEx(0, 0b01000000, true);
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
