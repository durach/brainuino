#include <Arduino.h>

// Requires: https://github.com/avishorp/TM1637/

#include "config.h"
#include "Timer.h"
#include "Buzzer.h"
#include "Lamps.h"
#include "Panel.h"

Timer timer = Timer();
Buzzer buzzer = Buzzer(PIN_BUZZER);

int8_t lampPins[] = {PIN_LED_TABLE_0, PIN_LED_TABLE_1, PIN_LED_TABLE_2, PIN_LED_TABLE_3, PIN_LED_TABLE_4, PIN_LED_TABLE_5};
Lamps lamps = Lamps(PIN_LED_START, PIN_LED_START_60, PIN_LED_START_20, lampPins);

int8_t tablePins[] = {PIN_BUTTON_TABLE_0, PIN_BUTTON_TABLE_1, PIN_BUTTON_TABLE_2, PIN_BUTTON_TABLE_3, PIN_BUTTON_TABLE_4, PIN_BUTTON_TABLE_5};

Panel panel = Panel(PIN_PANEL_CLK, PIN_PANEL_DIO);

int8_t tableButtonsPressed[] = {STATE_BUTTONS_TABLE_0_PRESSED, STATE_BUTTONS_TABLE_1_PRESSED, STATE_BUTTONS_TABLE_2_PRESSED, STATE_BUTTONS_TABLE_3_PRESSED, STATE_BUTTONS_TABLE_4_PRESSED, STATE_BUTTONS_TABLE_5_PRESSED}; 

volatile int8_t state_buttons_table = STATE_BUTTONS_TABLE_INIT;
volatile int8_t state_buttons_table_waiting = true;

volatile int8_t state_buttons_reset_pressed = false;
volatile int8_t state_buttons_start20_pressed = false;
volatile int8_t state_buttons_start60_pressed = false;

int8_t state = STATE_INIT;

void setup() {

  Serial.begin(9600);
  Serial.println("! Setup Begin");
  
  pinMode(PIN_BUTTON_START_60, INPUT_PULLUP);
  pinMode(PIN_BUTTON_START_20, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RESET, INPUT_PULLUP);

  for (int8_t i = 0; i < MAX_TABLES; i++) {
    pinMode(tablePins[i], INPUT_PULLUP);
  }

  lamps.setup();
  panel.setup();

  setupButtons();
  enableButtonsTables();

  state = STATE_WAITING;
  state_buttons_table_waiting = true;

  Serial.println("! Setup Done");
  
}

void loop() {
  processButtons();
  timer.tick();
  processTimer();

  if ((state == STATE_STARTED) && timer.isUpdated) {
    Serial.print("T=");
    Serial.println(timer.value);
  }

  processPanel();  
  processStartLampOff();
}

void processButtons() {

  // TODO: Add SI Mode

  if (state == STATE_STARTED) {
    processButtonsTables() || processButtonReset();
  } else if (state == STATE_WAITING) {
    processButtonsTables() || processButtonsStart() || processButtonReset();
  } else if (state == STATE_STOPPED) {
    processButtonReset();
  } else if (state == STATE_INIT) {
    handleError(ERROR_NOT_INITED);    
  } else {
    handleError(ERROR_UNKNOWN_STATE);
  }

}

bool processButtonsTables() {
  
  if (state_buttons_table_waiting) {
    return false;
  }

  Serial.print("Processing: <<");
  Serial.print(state_buttons_table, BIN);
  Serial.println(">>");
  
  for (int i = 0; i < MAX_TABLES; i++) {
    if (state_buttons_table == tableButtonsPressed[i]) {
      handleTable(i);
      return true;
    }    
  }

  handleError(ERROR_BUTTONS);

  return true;
}

bool processButtonsStart() {
  if (state_buttons_start60_pressed) {
    resetFlagsButtonsControl();
    handleButtonStart60();
    return true;
  } else if (state_buttons_start20_pressed) {
    resetFlagsButtonsControl();
    handleButtonStart20();
    return true;
  } 
  return false;
}

bool processButtonReset() {
  if (state_buttons_reset_pressed) {
    resetFlagsButtonsControl();
    handleButtonReset();
    return true;
  }
}

void processPanel() {
  
  if (timer.isUpdated) {
     panel.displayTime(timer.value);
  }
        
  timer.isUpdated = false;
}

void processStartLampOff() {
  if ((state == STATE_STARTED) && (timer.value > START_LAMP_DURATION) && (lamps.isStartOn)) {
    lamps.offStart();
  }
}

void processTimer() {
  if ((state == STATE_STARTED) && timer.isFinished) {
    handleFinish();
  }
}

void handleButtonStart60() {
  Serial.println("\n! Start 60");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_60);
    buzzer.playStartSound();
    lamps.onStart();
    lamps.onStart60();
    state = STATE_STARTED;
  }
}

void handleButtonStart20() {
  Serial.println("\n! Start 20");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_20);
    buzzer.playStartSound();
    lamps.onStart();
    lamps.onStart20();
    state = STATE_STARTED;
  }
}

void handleButtonReset() {
  Serial.println("\n! Reset");
  state = STATE_WAITING;
  state_buttons_table_waiting = true;
  timer.stop();
  timer.reset();
  lamps.allOff();
  buzzer.off();
  // TODO: Should we display 0:00 ?
  panel.off();
  enableButtonsTables();
  Serial.println("...Waiting...");
}

void handleTable(int8_t table) {
  Serial.print("Table ");
  Serial.print(table);
  Serial.print(" >> ");
  if (state == STATE_STARTED) {
    Serial.println("Table Pressed");
    buzzer.off();
    buzzer.playTableSound();
    lamps.onTable(table);
    state = STATE_STOPPED;
    timer.stop();
  } else if (state == STATE_WAITING) {
    Serial.println("False Start");
    buzzer.off();
    buzzer.playFalseStartSound();
    panel.displayFalseStart();
    lamps.onTable(table);
    state = STATE_STOPPED;
  } else if (state == STATE_INIT) {
    Serial.println("State Init ***");
  } else if (state == STATE_STOPPED) {
    Serial.println("State Stopped ***");
  } else {
    Serial.println("Unknown state");   
  }
}

void handleError(int8_t errorNo) {
  state = STATE_STOPPED;
  timer.stop(false);
  panel.error(errorNo);

  Serial.print("Error #");
  Serial.print(errorNo);

  if (errorNo == ERROR_BUTTONS) {
    for (int i = 0; i < MAX_TABLES; i++) {
      if (bitRead(state_buttons_table, 0)) {
        lamps.onTable(i);
      }    
    }
  }
}

void handleFinish() {
  Serial.println("! Finish");
  state = STATE_STOPPED;
  buzzer.playFinishSound();
}

void setupButtons() {
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_RESET), isrButtonReset, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_START_20), isrButtonStart20, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_START_60), isrButtonStart60, FALLING);

  PCMSK2 = B00111111; // enable PCINT16..21

  resetFlagsButtonsControl();
}

void resetFlagsButtonsControl() {
  state_buttons_reset_pressed = false;
  state_buttons_start20_pressed = false;
  state_buttons_start60_pressed = false;  
}

void enableButtonsTables() {
  PCICR |= B00000100;  // enable PCIE2 group
}

void disableButtonsTables() {
  PCICR &= ~B00000100;  // disable PCIE2 group
}

ISR(PCINT2_vect) {
  uint8_t buttons;
  buttons = PINK;
  buttons &= B00111111; // sometimes not-used bits are HIGH

  if (buttons == STATE_BUTTONS_TABLE_INIT) {
    Serial.print(". ");      
  } else if (state_buttons_table_waiting) {
    disableButtonsTables();
    state_buttons_table_waiting = false;
    state_buttons_table = buttons;
    Serial.print("+ ");  
  } else {
    Serial.print("- ");  
  }
  Serial.println(buttons, BIN);
}

void isrButtonReset() {
  state_buttons_reset_pressed = true;
  Serial.println("B=Reset");  
}

void isrButtonStart20() {
  state_buttons_start20_pressed = true;
  Serial.println("B=Start20");  
}

void isrButtonStart60() {
  state_buttons_start60_pressed = true;
  Serial.println("B=Start60");  
}
