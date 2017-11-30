
#include "config.h"
#include "LedPanel.h"
#include "Timer.h"

LedPanel led_panel = LedPanel();
Timer timer = Timer();

volatile byte state = STATE_INIT;
volatile byte display_table = NO_TABLE;

bool update_panel = false;
bool display_falsestart = false;

void setup() {

  Serial.begin(9600);
  Serial.println("Setup Begin");
  
  pinMode(PIN_BUTTON_START_60, INPUT_PULLUP);
  pinMode(PIN_BUTTON_START_20, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RESET, INPUT_PULLUP);
  pinMode(PIN_BUTTON_TABLE_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_TABLE_2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_1), handle_table1_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_2), handle_table2_button, FALLING);

  led_panel.setup();

  state = STATE_WAITING;
  Serial.println("Setup Done");

}

void loop() {
 
  process_buttons();

  timer.tick();
  
  if (timer.is_updated || display_falsestart) {
    update_panel = true;
  }

  if (update_panel) {
    update_panel = false;
    
    led_panel.drawBegin();

    if (state == STATE_STOPPED) {
      if (display_falsestart) {
        led_panel.drawFalseStart();
      } else {
        led_panel.drawTime(timer.value);  
      }
    } else if (state == STATE_STARTED) {
      if (timer.value <= START_SCREEN_TIME) {
        led_panel.drawTime(timer.value, true);
      } else {
        led_panel.drawTime(timer.value);
      }
    }
    
    if (display_table != NO_TABLE) {
       led_panel.drawBrainTable(display_table);
    }
    
    led_panel.drawEnd();
    
    timer.is_updated = false;
  }
}

void process_buttons() {
  if (state == STATE_WAITING) {
      if (digitalRead(PIN_BUTTON_START_60) == BUTTON_PRESSED) {
      handle_start60_button();
    } else if (digitalRead(PIN_BUTTON_START_20) == BUTTON_PRESSED) {
      handle_start20_button();
    }
  } else if ((state == STATE_STARTED) || (state == STATE_STOPPED)) {
    if (digitalRead(PIN_BUTTON_RESET) == BUTTON_PRESSED) {
      handle_reset_button();
    }
  }
}

// NOTE: called from loop
void handle_start60_button() {
  Serial.println("Start 60");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_60);
    state = STATE_STARTED;
  }
}

void handle_start20_button() {
  Serial.println("Start 20");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_20);
    state = STATE_STARTED;
  }
}

void handle_reset_button() {
  Serial.println("Reset");
  state = STATE_WAITING;
  timer.stop();
  timer.reset();
  display_table = NO_TABLE;
  update_panel = true;
  display_falsestart = false;
}

// NOTE: to be called from an interrrupt
void handle_table1_button() {
  Serial.println("Table 1");
  hanble_table(TABLE_1);
}

// NOTE: to be called from an interrrupt
void handle_table2_button() {
  Serial.println("Table 2");
  hanble_table(TABLE_2);
}

// NOTE: to be called from an interrrupt
void hanble_table(byte table) {
 if (state == STATE_STARTED) {
    display_table = table;
    state = STATE_STOPPED;
    timer.stop();
  } else if (state == STATE_WAITING) {
    display_table = table;
    display_falsestart = true;
    state = STATE_STOPPED;
  }
}

