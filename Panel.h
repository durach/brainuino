#include <Arduino.h>
#include "config.h"
#include "TM1637Display.h"

#define PANEL_BRIGHTNESS 4 // 0..7
#define PANEL_SPLASH_DURATION 2000 // ms

class Panel {

  private:
    uint8_t _pinCLK;
    uint8_t _pinDIO;
    TM1637Display _tm;
    
  public:
    Panel(uint8_t pinCLK, uint8_t pinDIO);
    void setup(uint8_t mode);
    void displayTime(uint32_t time);
    void displayFalseStart();
    void off();
    void error(uint8_t error);
};
