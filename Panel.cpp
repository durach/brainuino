#include "Panel.h"
#include "TM1637.h"

Panel::Panel(byte pinCLK, byte pinDIO) {
   _pinCLK = pinCLK;
   _pinDIO = pinDIO;
   _tm = TM1637(_pinCLK, _pinDIO);
}

void Panel::setup() {
   _tm.init(D4056A);
   _tm.set(PANEL_BRIGHTNESS);
   _tm.display(0);
}

void Panel::displayTime(float time) {
  _tm.display(time);
}

void Panel::displayFalseStart() {
  _tm.display(99.99);
}

void Panel::off() {
  _tm.clearDisplay();
}
