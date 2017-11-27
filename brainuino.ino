
#include "config.h"
#include "LedPanel.h"
#include "Timer.h"

LedPanel led_panel = LedPanel();
Timer timer = Timer();

byte state = STATE_WAITING;

void setup() {
  debug_start();

  pinMode(PIN_BUTTON_START_60, INPUT_PULLUP);
  pinMode(PIN_BUTTON_START_20, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RESET, INPUT_PULLUP);
  pinMode(PIN_BUTTON_TABLE_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_TABLE_2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_1), do_table1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_2), do_table2, FALLING);

  led_panel.setup();
  led_panel.test();

//  timer.start(TIMER_START_20);

}

void loop() {

  if (state == STATE_WAITING) {
    if (digitalRead(PIN_BUTTON_START_60) == BUTTON_PRESSED) {
      do_start60();
    } else if (digitalRead(PIN_BUTTON_START_20) == BUTTON_PRESSED) {
      do_start20();
    } else if (digitalRead(PIN_BUTTON_RESET) == BUTTON_PRESSED) {
      do_reset();
    }
  }

  delay(100);
  
//  timer.tick();
//  if (timer.is_running) {
//    led_panel.drawTime(timer.value);
//    Serial.println(timer.value);
//  }
//  delay(5);
}

void debug_start() {
  #ifdef DEBUG
  Serial.begin(9600);
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only  
  #endif
}

void do_start60() {
  Serial.println("Start 60");
}

void do_start20() {
  Serial.println("Start 20");
}

void do_reset() {
  Serial.println("Reset");
}

void do_table1() {
  Serial.println("Table 1");
}

void do_table2() {
  Serial.println("Table 2");
}

