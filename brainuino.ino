
#include "config.h"
#include "LedPanel.h"
#include "Timer.h"

LedPanel led_panel = LedPanel();
Timer timer = Timer();

volatile byte state = STATE_INIT;

void setup() {
  pinMode(PIN_BUTTON_START_60, INPUT_PULLUP);
  pinMode(PIN_BUTTON_START_20, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RESET, INPUT_PULLUP);
  pinMode(PIN_BUTTON_TABLE_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_TABLE_2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_1), handle_table1_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_2), handle_table2_button, FALLING);

  // debug_start();

  led_panel.setup();
  led_panel.test();

  state = STATE_WAITING;
  
}

void loop() {

//  if (state == STATE_WAITING) {
//    if (digitalRead(PIN_BUTTON_START_60) == BUTTON_PRESSED) {
//      handle_start60_button();
//    } else if (digitalRead(PIN_BUTTON_START_20) == BUTTON_PRESSED) {
//      handle_start20_button();
//    } else if (digitalRead(PIN_BUTTON_RESET) == BUTTON_PRESSED) {
//      handle_reset_button();
//    }
//  }
//
//  delay(100);
//  
//  timer.tick();
  
//  if (timer.is_running) {
//    led_panel.drawTime(timer.value);
//    Serial.println(timer.value);
//  }
  delay(100);
  Serial.println("Do nothing");
}

void debug_start() {
  #ifdef DEBUG
  Serial.begin(9600);
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only  
  #endif
}

// NOTE: called from loop
void handle_start60_button() {
  Serial.println("Start 60");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_60);
  }
}

void handle_start20_button() {
  Serial.println("Start 20");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_20);
  }
}

void handle_reset_button() {
  Serial.println("Reset");
  state = STATE_WAITING;
}

// NOTE: called from interrrupt
void handle_table1_button() {
  if ((state == STATE_STARTED) || (state == STATE_WAITING)) {
    Serial.println("Table 1");
  }
}

void handle_table2_button() {
  if ((state == STATE_STARTED) || (state == STATE_WAITING)) {
    Serial.println("Table 2");
  }
}

void panel_display_start() {
  
}

void panel_display_table1_pressed() {
  
}

void panel_display_table2_pressed() {
  
}

void panel_update_timer() {
  
}

