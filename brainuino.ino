// Requires: https://github.com/avishorp/TM1637/

#include "config.h"
#include "Timer.h"
#include "Buzzer.h"
#include "Lamps.h"
#include "Panel.h"

Timer timer = Timer();
Buzzer buzzer = Buzzer(PIN_BUZZER);

byte lampPins[] = {PIN_LED_TABLE_0, PIN_LED_TABLE_1, PIN_LED_TABLE_2, PIN_LED_TABLE_3};
Lamps lamps = Lamps(PIN_LED_START, lampPins);

byte tablePins[] = {PIN_BUTTON_TABLE_0, PIN_BUTTON_TABLE_1, PIN_BUTTON_TABLE_2, PIN_BUTTON_TABLE_3};

Panel panel = Panel(PIN_PANEL_CLK, PIN_PANEL_DIO);

volatile byte state = STATE_INIT;
volatile byte state_table = NO_TABLE;

unsigned long this_loop_start = 0;
unsigned long profile_1 = 0;
unsigned long profile_2 = 0;
unsigned long profile_3 = 0;
unsigned long profile_4 = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("Setup Begin");
  
  pinMode(PIN_BUTTON_START_60, INPUT_PULLUP);
  pinMode(PIN_BUTTON_START_20, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RESET, INPUT_PULLUP);

  for (byte i = 0; i < MAX_TABLES; i++) {
    pinMode(tablePins[i], INPUT_PULLUP);
  }

  lamps.setup();
  panel.setup();

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_0), handleTableButton0, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_1), handleTableButton1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_2), handleTableButton2, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_TABLE_3), handleTableButton3, FALLING);

  state = STATE_WAITING;
  state_table = NO_TABLE;

  Serial.println("Setup Done");
  
}

void loop() {
  this_loop_start = millis();
 
  processButtons();

  profile_1 = millis();

  timer.tick();
  profile_2 = millis();

  bool is_updated = timer.isUpdated;

  processPanel();
  profile_3 = millis();
  
  processStartLampOff();

  profile_4 = millis();

  if (state == STATE_STARTED) {
    Serial.print("this_loop_start: ");
    Serial.print(this_loop_start);
    Serial.print(", 1+: ");
    Serial.print(profile_1-this_loop_start);
    Serial.print(", 2+: ");
    Serial.print(profile_2-profile_1);
    Serial.print(", 3+: ");
    Serial.print(profile_3-profile_2);
    Serial.print(", 4+: ");
    Serial.print(profile_4-profile_3);
    Serial.print(", now+: ");
    Serial.print(millis()-profile_4);
    Serial.print(", is_updated: ");
    Serial.print(is_updated);
    Serial.print(", timer.value: ");
    Serial.print(timer.value);
    Serial.println("$");
  }
}

void processButtons() {

  // TODO: Add SI Mode

  if (state == STATE_STARTED) {
    processButtonsTables() || processButtonReset();
  } else if (state == STATE_WAITING) {
    processButtonsTables() || processButtonsStart(); // Probably we need to add reset here to switch to SI more
  } else if (state == STATE_STOPPED) {
    processButtonReset();
  } else if (state == STATE_INIT) {
    handleError(ERROR_NOT_INITED, "");    
  } else {
    handleError(ERROR_UNKNOWN_STATE, "");
  }

}

bool processButtonsTables() {
  
  if (state_table == NO_TABLE) {
    return false;
  }

  handleTable(state_table);
  return true;

}

bool processButtonsStart() {
  if (digitalRead(PIN_BUTTON_START_60) == BUTTON_PRESSED) {
    handleButtonStart60();
    return true;
  } else if (digitalRead(PIN_BUTTON_START_20) == BUTTON_PRESSED) {
    handleButtonStart20();
    return true;
  } 
  return false;
}

bool processButtonReset() {
  if (digitalRead(PIN_BUTTON_RESET) == BUTTON_PRESSED) {
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

void handleButtonStart60() {
  Serial.println("Start 60");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_60);
    buzzer.playStartSound();
    lamps.onStart();
    state = STATE_STARTED;
  }
}

void handleButtonStart20() {
  Serial.println("Start 20");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_20);
    buzzer.playStartSound();
    lamps.onStart();
    state = STATE_STARTED;
  }
}

void handleButtonReset() {
  Serial.println("Reset");
  state = STATE_WAITING;
  state_table = NO_TABLE;
  timer.stop();
  timer.reset();
  lamps.allOff();
  buzzer.off();
  // TODO: Should we display 0:00 ?
  panel.off();
}

// NOTE: to be called from an interrrupt
void handleTableButton0() {
  handleTableButton(TABLE_0);
}

// NOTE: to be called from an interrrupt
void handleTableButton1() {
  handleTableButton(TABLE_1);
}

// NOTE: to be called from an interrrupt
void handleTableButton2() {
  handleTableButton(TABLE_2);
}

// NOTE: to be called from an interrrupt
void handleTableButton3() {
  handleTableButton(TABLE_3);
}

// NOTE: to be called from an interrrupt
void handleTableButton(byte table) {
  Serial.print("Detected ");
  Serial.print(table);
  Serial.print(". ");

  if (state_table != NO_TABLE) {
    Serial.print("Table ");
    Serial.print(state_table);
    Serial.println(" already pressed. Ignoring");
    return;
  }

  state_table = table;
  Serial.println();
}

void handleTable(byte table) {
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

void handleError(byte errorNo, String description) {
  state = STATE_STOPPED;
  timer.stop(false);
  panel.error(errorNo);

  Serial.print("Error #");
  Serial.print(errorNo);
  Serial.print(", <");
  Serial.print(description);
  Serial.println("$");
}
