#include <Arduino.h>
#include "config.h"

class Lamps {

  private:
    uint8_t _pinStart;
    uint8_t _pinStart60;
    uint8_t _pinStart20;
    uint8_t* _pinTable; // aka byte _pinTable[MAX_TABLES]

  public:
    bool isStartOn = false;
    
  public:
    Lamps(uint8_t pinStart, uint8_t pinStart60, uint8_t pinStart20, uint8_t pinTable[]);
    void setup();
    void onStart();
    void offStart();
    void onStart60();
    void offStart60();
    void onStart20();
    void offStart20();
    void onTable(uint8_t table);
    void allOff();
};
