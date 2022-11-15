#include <Arduino.h>
#include "config.h"
#include "TM1637Display.h"

#define PANEL_BRIGHTNESS 4
#define PANEL_SPLASH_DURATION 2000

class Panel {

  private:
    int8_t _pinCLK;
    int8_t _pinDIO;
    TM1637Display _tm;
    
  public:
    Panel(int8_t pinCLK, int8_t pinDIO);
    void setup(int8_t mode);
    void displayTime(float time);
    void displayFalseStart();
    void off();
    void error(int8_t error);
};
