#include <Arduino.h>
#include "config.h"

class Lamps {

  private:
    byte _pinStart;
    byte* _pinTable; // aka byte _pinTable[MAX_TABLES]

  public:
    bool isStartOn = false;
    
  public:
    Lamps(byte pinStart, byte pinTable[]);
    void setup();
    void onStart();
    void offStart();
    void onTable(byte table);
    void allOff();
};
