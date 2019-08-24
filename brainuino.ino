#include "config.h"
#include "Timer.h"
#include "Buzzer.h"
#include "Lamps.h"

Timer timer = Timer();
Buzzer buzzer = Buzzer(PIN_BUZZER);
Lamps lamps = Lamps(PIN_BUTTON_START);

volatile byte state = STATE_INIT;

//bool update_panel = false;
//bool display_falsestart = false;

void setup() {

  Serial.begin(9600);
  Serial.println("Setup Begin");
  
  pinMode(PIN_BUTTON_START_60, INPUT_PULLUP);
  pinMode(PIN_BUTTON_START_20, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RESET, INPUT_PULLUP);
  pinMode(PIN_BUTTON_TABLE_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_TABLE_2, INPUT_PULLUP);

  lamps.setup();

  state = STATE_WAITING;
  Serial.println("Setup Done");
}

void loop() {
 
  processButtons();

  timer.tick();
  
//  process_panel();

}

void processButtons() {

  if (state == STATE_STARTED) {
    processButtonsTables() || processButtonReset();
  } else if (state == STATE_WAITING) {
    processButtonsTables() || processButtonsStart();
  } else if (state == STATE_STOPPED) {
    processButtonReset();
  } else if (state == STATE_INIT) {
    handleError(ERROR_NOT_INITED);    
  } else {
    handleError(ERROR_UNKNOWN_STATE);
  }

}

bool processButtonsTables() {
  // TODO: Add protection against simultation tracking
  if (digitalRead(PIN_BUTTON_TABLE_1) == BUTTON_PRESSED) {
    handleTable(TABLE_1);
    return true;
  } else if (digitalRead(PIN_BUTTON_TABLE_2) == BUTTON_PRESSED) {
    handleTable(TABLE_2);
    return true;
  }
  return false;
}

bool processButtonsStart() {
  // TODO: Add protection against simultation tracking
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

//void process_panel() {
//  if (timer.is_updated || display_falsestart) {
//    update_panel = true;
//  }
//
//  if (update_panel) {
//    update_panel = false;
//    
//    led_panel.drawBegin();
//
//    if (state == STATE_STOPPED) {
//      if (display_falsestart) {
//        led_panel.drawFalseStart();
//      } else {
//        led_panel.drawTime(timer.value);  
//      }
//    } else if (state == STATE_STARTED) {
//      if (timer.value <= START_SCREEN_TIME) {
//        led_panel.drawTime(timer.value, true);
//      } else {
//        led_panel.drawTime(timer.value);
//      }
//    }
//    
//    if (display_table != NO_TABLE) {
//       led_panel.drawBrainTable(display_table);
//    }
//    
//    led_panel.drawEnd();
//    
//    timer.is_updated = false;
//  }
//}

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
//  display_table = NO_TABLE;
//  update_panel = true;
//  display_falsestart = false;
}

void handleTable(byte table) {
  Serial.print(table);
  Serial.print(" >> ");
 if (state == STATE_STARTED) {
    Serial.println("Table Pressed");
//    display_table = table;
    buzzer.off();
    buzzer.playTableSound();
    lamps.onTable(table);
    state = STATE_STOPPED;
    timer.stop();
  } else if (state == STATE_WAITING) {
    Serial.println("False Start");
//    display_table = table;
    buzzer.off();
    buzzer.playFalseStartSound();
//    display_falsestart = true;
    state = STATE_STOPPED;
  } else if (state == STATE_INIT) {
    Serial.println("State Init ***");
  } else if (state == STATE_STOPPED) {
    Serial.println("State Stopped ***");
  } else {
    Serial.println("Unknown state");   
  }
}

void handleError(byte errorNo) {
  Serial.print("Error ");
  Serial.print(errorNo);
}
