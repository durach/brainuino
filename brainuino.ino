#include <Arduino.h>

// Requires: https://github.com/avishorp/TM1637/

#include "config.h"
#include "Timer.h"
#include "Buzzer.h"
#include "Lamps.h"
#include "Panel.h"

#define DEBOUNCE_DELAY 50  // debounce delay in milliseconds

Timer timer = Timer();
Buzzer buzzer = Buzzer(PIN_BUZZER);

uint8_t lampPins[] = {PIN_LED_TABLE_1, PIN_LED_TABLE_2, PIN_LED_TABLE_3, PIN_LED_TABLE_4, PIN_LED_TABLE_5, PIN_LED_TABLE_6};
Lamps lamps = Lamps(PIN_LED_START, PIN_LED_START_60, PIN_LED_START_20, lampPins);

uint8_t tablePins[] = {PIN_BUTTON_TABLE_1, PIN_BUTTON_TABLE_2, PIN_BUTTON_TABLE_3, PIN_BUTTON_TABLE_4, PIN_BUTTON_TABLE_5, PIN_BUTTON_TABLE_6};

Panel panel = Panel(PIN_PANEL_CLK, PIN_PANEL_DIO);

uint8_t tableButtonsPressed[] = {STATE_BUTTONS_TABLE_1_PRESSED, STATE_BUTTONS_TABLE_2_PRESSED, STATE_BUTTONS_TABLE_3_PRESSED, STATE_BUTTONS_TABLE_4_PRESSED, STATE_BUTTONS_TABLE_5_PRESSED, STATE_BUTTONS_TABLE_6_PRESSED}; 

volatile uint8_t state_buttons_table = STATE_BUTTONS_TABLE_INIT;
volatile uint8_t state_buttons_table_waiting = true;

// Removed volatile flags for Start60, Start20, and Reset buttons
// They are no longer needed since we poll these buttons in the main loop.

uint8_t state = STATE_INIT;
uint8_t mode = MODE_BR;
bool finishing_sound_played = false;

uint32_t random_end;

void setup() {

  Serial.begin(9600);
  Serial.println("! Setup Begin");
  
  pinMode(PIN_BUTTON_START_60, INPUT_PULLUP);
  pinMode(PIN_BUTTON_START_20, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RESET, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MODE, INPUT_PULLUP);

  for (int8_t i = 0; i < MAX_TABLES; i++) {
    pinMode(tablePins[i], INPUT_PULLUP);
  }

  mode = digitalRead(PIN_BUTTON_MODE);

  lamps.setup();
  panel.setup(mode);

  setupButtons();
  enableButtonsTables();

  state = STATE_WAITING;
  state_buttons_table_waiting = true;
  
  timer.reset();

  randomInit();

  Serial.println("! Setup Done");
  
}

void loop() {
  processButtons();
  timer.tick();
  processTimer();
  processPanel();  
  processStartLampOff();
}

void processButtons() {
  if (state == STATE_STARTED) {
    processButtonsTables() || processButtonReset();
  } else if (state == STATE_WAITING) {
    processButtonsTables() || processButtonsStart() || processButtonReset();
  } else if (state == STATE_DELAYED_START_60) {
    processButtonsTables() || processDelayedStart60() || processButtonReset();
  } else if (state == STATE_DELAYED_START_20) {
    processButtonsTables() || processDelayedStart20() || processButtonReset();
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
  if (mode == MODE_SG) {
    return false;
  }
  
  static uint8_t lastStart60State = HIGH;
  static uint8_t lastStart20State = HIGH;
  
  uint8_t currentStart60State = digitalRead(PIN_BUTTON_START_60);
  uint8_t currentStart20State = digitalRead(PIN_BUTTON_START_20);
  
  // Detect falling edge for Start 60 button
  if (lastStart60State == HIGH && currentStart60State == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(PIN_BUTTON_START_60) == LOW) {
      handleButtonStart60();
      lastStart60State = LOW;
      return true;
    }
  }
  lastStart60State = currentStart60State;
  
  // Detect falling edge for Start 20 button
  if (lastStart20State == HIGH && currentStart20State == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(PIN_BUTTON_START_20) == LOW) {
      handleButtonStart20();
      lastStart20State = LOW;
      return true;
    }
  }
  lastStart20State = currentStart20State;
  
  return false;
}

bool processDelayedStart60() {
  if (isRandomFinished()) {
    handleDelayedStart60();
    return true;
  }
  return false;
}

bool processDelayedStart20() {
  if (isRandomFinished()) {
    handleDelayedStart20();
    return true;
  }
  return false;
}

bool processButtonReset() {
  static uint8_t lastResetState = HIGH;
  
  uint8_t currentResetState = digitalRead(PIN_BUTTON_RESET);
  
  if (lastResetState == HIGH && currentResetState == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(PIN_BUTTON_RESET) == LOW) {
      handleButtonReset();
      lastResetState = LOW;
      return true;
    }
  }
  lastResetState = currentResetState;
  
  return false;
}

void processPanel() {
  if (mode == MODE_BR) {
    if (timer.isUpdated) {
      bool display_cs = !state_buttons_table_waiting || timer.is_finishing() || timer.isFinished;
      panel.displayTime(timer.value, display_cs);
      timer.isUpdated = false;
    }
  }
}

void processStartLampOff() {
  if ((state == STATE_STARTED) && (timer.value > START_LAMP_DURATION) && (lamps.isStartOn)) {
    lamps.offStart();
  }
}

void processTimer() {
  if (state == STATE_STARTED) {
    if (timer.is_finishing() && !finishing_sound_played) {
      buzzer.playFinishingSound();
      finishing_sound_played = true;
    }
    if (timer.isFinished) {
      handleFinish();
    }
  }
}

void handleButtonStart60() {
  Serial.println("\n! Start 60");
  if (state == STATE_WAITING) {
    randomStart();
    state = STATE_DELAYED_START_60;
  }
}

void handleButtonStart20() {
  Serial.println("\n! Start 20");
  if (state == STATE_WAITING) {
    randomStart();
    state = STATE_DELAYED_START_20;
  }
}

void handleDelayedStart60() {
  Serial.println("\n! Delayed Start 60");
  timer.start(TIMER_START_60);
  buzzer.playStartSound();
  lamps.onStart();
  lamps.onStart60();
  finishing_sound_played = false;
  state = STATE_STARTED;
}

void handleDelayedStart20() {
  Serial.println("\n! Delayed Start 20");
  timer.start(TIMER_START_20);
  buzzer.playStartSound();
  lamps.onStart();
  lamps.onStart20();
  finishing_sound_played = false;
  state = STATE_STARTED;
}

void handleButtonReset() {
  Serial.println("\n! Reset");
  state = STATE_WAITING;
  state_buttons_table_waiting = true;
  timer.stop();
  timer.reset();
  lamps.allOff();
  buzzer.off();
  panel.off();
  enableButtonsTables();
  Serial.println("...Waiting...");
}

void handleTable(int8_t table) {
  Serial.print("Table ");
  Serial.print(table);
  Serial.print(" >> ");
  if (state == STATE_STARTED) {
    Serial.println("BR: Table Pressed");
    buzzer.off();
    buzzer.playTableSound();
    lamps.offStart();
    lamps.onTable(table);
    state = STATE_STOPPED;
    timer.stop();
  } else if (state == STATE_WAITING || state == STATE_DELAYED_START_60 || state == STATE_DELAYED_START_20) {
    if (mode == MODE_BR) {
      Serial.println("BR: False Start");
      buzzer.off();
      buzzer.playFalseStartSound();
      panel.displayFalseStart();
    } else {
      Serial.println("SG: Table Pressed");
      buzzer.playTableSound();
    }
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
  // Removed attachInterrupt for PIN_BUTTON_RESET, PIN_BUTTON_START_20, and PIN_BUTTON_START_60.
  // These buttons are now polled in the main loop.
  
  PCMSK2 = B00111111; // enable PCINT16..21 for table buttons
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

void randomInit() {
  randomSeed(analogRead(PIN_RANDOM_SEED));
}

void randomStart() {
  uint32_t random_delay = random(MAX_RANDOM_DELAY);
  uint32_t random_start = millis();
  Serial.print("Delay: ");
  Serial.println(random_delay);
  
  random_end = random_start + random_delay;
}

bool isRandomFinished() {
  return millis() >= random_end;
}