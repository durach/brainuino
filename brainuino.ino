
#include "config.h"
#include "LedPanel.h"

LedPanel led_panel = LedPanel();

void setup() {
  led_panel.setup();
  led_panel.test();
}

void loop() {

}

void debug_start() {
  #ifdef DEBUG
  Serial.begin(9600);
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only  
  #endif
}

void debug(String msg) {
  #ifdef DEBUG
  Serial.println(msg); 
  #endif
}

