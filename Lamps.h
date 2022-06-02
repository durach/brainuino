#include <Arduino.h>
#include "config.h"

class Lamps {

  private:
    int8_t _pinStart;
    int8_t _pinStart60;
    int8_t _pinStart20;
    int8_t* _pinTable; // aka byte _pinTable[MAX_TABLES]

  public:
    bool isStartOn = false;
    
  public:
    Lamps(int8_t pinStart, int8_t pinStart60, int8_t pinStart20, int8_t pinTable[]);
    void setup();
    void onStart();
    void offStart();
    void onStart60();
    void offStart60();
    void onStart20();
    void offStart20();
    void onTable(int8_t table);
    void allOff();
};
