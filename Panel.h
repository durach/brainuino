#include <Arduino.h>
#include "config.h"
#include "TM1637Display.h"

#define PANEL_BRIGHTNESS 4

class Panel {

  private:
    byte _pinCLK;
    byte _pinDIO;
    TM1637Display _tm;
    
  public:
    Panel(byte pinCLK, byte pinDIO);
    void setup();
    void displayTime(float time);
    void displayFalseStart();
    void off();
};
