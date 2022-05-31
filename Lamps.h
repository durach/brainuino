#include <Arduino.h>
#include "config.h"

class Lamps {

  private:
    int8_t _pinStart;
    int8_t* _pinTable; // aka byte _pinTable[MAX_TABLES]

  public:
    bool isStartOn = false;
    
  public:
    Lamps(int8_t pinStart, int8_t pinTable[]);
    void setup();
    void onStart();
    void offStart();
    void onTable(int8_t table);
    void allOff();
};
