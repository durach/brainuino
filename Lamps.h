#include <Arduino.h>
#include "config.h"

class Lamps {

  private:
    byte _pinStart;
    byte _piFalseStart;
    byte _pinTable[MAX_TABLES];
    
  public:
    Lamps(byte pinStart);
    void setup();
    void onStart();
    void onFalseStart();
    void onTable(byte table);
    void allOff();
};
