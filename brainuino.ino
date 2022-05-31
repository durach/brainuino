// Requires: https://github.com/avishorp/TM1637/

#include "config.h"
#include "Timer.h"
#include "Buzzer.h"
#include "Lamps.h"
#include "Panel.h"

Timer timer = Timer();
Buzzer buzzer = Buzzer(PIN_BUZZER);

byte lampPins[] = {PIN_LED_TABLE_0, PIN_LED_TABLE_1, PIN_LED_TABLE_2, PIN_LED_TABLE_3, PIN_LED_TABLE_4, PIN_LED_TABLE_5};
Lamps lamps = Lamps(PIN_LED_START, lampPins);

byte tablePins[] = {PIN_BUTTON_TABLE_0, PIN_BUTTON_TABLE_1, PIN_BUTTON_TABLE_2, PIN_BUTTON_TABLE_3, PIN_BUTTON_TABLE_4, PIN_BUTTON_TABLE_5};

Panel panel = Panel(PIN_PANEL_CLK, PIN_PANEL_DIO);

#define STATE_BUTTONS_INIT B00111111
#define STATE_BUTTONS_TABLE_0_PRESSED B00111110
#define STATE_BUTTONS_TABLE_1_PRESSED B00111101
#define STATE_BUTTONS_TABLE_2_PRESSED B00111011
#define STATE_BUTTONS_TABLE_3_PRESSED B00110111
#define STATE_BUTTONS_TABLE_4_PRESSED B00101111
#define STATE_BUTTONS_TABLE_5_PRESSED B00011111

// TODO Generate this array?
byte buttonsPressed[] = {STATE_BUTTONS_TABLE_0_PRESSED, STATE_BUTTONS_TABLE_1_PRESSED, STATE_BUTTONS_TABLE_2_PRESSED, STATE_BUTTONS_TABLE_3_PRESSED, STATE_BUTTONS_TABLE_4_PRESSED, STATE_BUTTONS_TABLE_5_PRESSED}; 

volatile byte state_buttons = STATE_BUTTONS_INIT;
volatile byte state_buttons_waiting = true;

byte state = STATE_INIT;

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

  PCMSK0 = B00111111; // enable PCINT0..7
  enableTableButtons();

  state = STATE_WAITING;
  state_buttons_waiting = true;

  Serial.println("Setup Done");
  
}

void loop() {
  processButtons();
  timer.tick();
  processTimer();

  if ((state == STATE_STARTED) && timer.isUpdated) {
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
  
  if (state_buttons_waiting) {
    return false;
  }

  Serial.print("Processing: ");
  Serial.println(state_buttons, BIN);
  
  for (int i = 0; i < MAX_TABLES; i++) {
    if (state_buttons == buttonsPressed[i]) {
      handleTable(i);
      return true;
    }    
  }

  handleError(ERROR_BUTTONS, ""); // Add buttons here as param 

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

void processTimer() {
  if ((state == STATE_STARTED) && timer.isFinished) {
    handleFinish();
  }
}

void handleButtonStart60() {
  Serial.println("\nStart 60");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_60);
    buzzer.playStartSound();
    lamps.onStart();
    state = STATE_STARTED;
  }
}

void handleButtonStart20() {
  Serial.println("\nStart 20");
  if (state == STATE_WAITING) {
    timer.start(TIMER_START_20);
    buzzer.playStartSound();
    lamps.onStart();
    state = STATE_STARTED;
  }
}

void handleButtonReset() {
  Serial.println("\nReset");
  state = STATE_WAITING;
  state_buttons_waiting = true;
  timer.stop();
  timer.reset();
  lamps.allOff();
  buzzer.off();
  // TODO: Should we display 0:00 ?
  panel.off();
  enableTableButtons();
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

void handleFinish() {
  Serial.println("Finish");
  state = STATE_STOPPED;
  buzzer.playFinishSound();
}

void enableTableButtons() {
  PCICR |= B00000001;  // enable PCIE0 group
}

void disableTableButtons() {
  PCICR &= ~B00000001;  // disable PCIE0 group
}

ISR(PCINT0_vect) {
  uint8_t buttons;
  buttons = PINB;

  if (buttons == STATE_BUTTONS_INIT) {
    Serial.print(". ");      
  } else if (state_buttons_waiting) {
    disableTableButtons();
    state_buttons_waiting = false;
    state_buttons = buttons;
    Serial.print("+ ");  
  } else {
    Serial.print("- ");  
  }
  Serial.println(buttons, BIN);
}
