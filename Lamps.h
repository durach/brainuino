#include <Arduino.h>
#include "config.h"

class Lamps {

  private:
    byte _pinStart;
    byte _pinTable[MAX_TABLES];
    
  public:
    Lamps(byte pinStart, byte pinTable[]);
    void setup();
    void onStart();
    void onTable(byte table);
    void allOff();
};
