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

  state = STATE_WAITING;
  Serial.println("Setup Done");

  pinMode(13, OUTPUT);

}

void loop() {
  processButtons();
  timer.tick();
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
  
  // Hack? Use PIND instead
  byte buttons = 0;

  for (byte i = 0; i < MAX_TABLES; i++) {
    if (digitalRead(tablePins[i]) == BUTTON_PRESSED) {
      bitSet(buttons, i);
    }
  }

  short table = buttonsGetTableNumber(buttons);

  if (table == NO_TABLE) {
    return false;
  }

  if (table == TABLE_COLLISION) {
    String description = String(buttons, BIN);
    handleError(ERROR_BUTTONS_COLLISION, description);
    return true;
  }

  handleTable(table);
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
  timer.stop();
  timer.reset();
  lamps.allOff();
  buzzer.off();
  // TODO: Should we display 0:00 ?
  panel.off();
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
  // TODO: show error state on the screen
  Serial.print("Error ");
  Serial.print(errorNo);
  Serial.print(" ");
  Serial.print(description);
  Serial.println("$");
  state = STATE_STOPPED;
}

short buttonsGetTableNumber(byte buttons) {

  if (buttons == 0) {
    return NO_TABLE;
  }

  byte table = 0;
  byte bit_number = 1;
  
  while(buttons > 0) {
    if (buttons & 1) {
      if (table == 0) {
        table = bit_number;
      } else {
        // Two or more buttons simultaniously pressed
        return TABLE_COLLISION;
      }
    }

    buttons = buttons >> 1;
    bit_number++;
  }

  return table;
}
