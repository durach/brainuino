#include "Panel.h"
#include "TM1637.h"

Panel::Panel(byte pinCLK, byte pinDIO) {
   _pinCLK = pinCLK;
   _pinDIO = pinDIO;
   _tm = TM1637(_pinCLK, _pinDIO);
}

void Panel::setup() {
   //_tm.init(D4056A);
   _tm.init();
   _tm.set(PANEL_BRIGHTNESS);
//   _tm.display(0);
   _tm.display(0, 0x1A);
}

void Panel::displayTime(float time) {
  _tm.display(time);
}

void Panel::displayFalseStart() {
    int8_t msg[4] = {0x7F, 0xF, 0xA, 0x7F};
    _tm.display(msg);
}

void Panel::off() {
  _tm.clearDisplay();
}
