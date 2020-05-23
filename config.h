#include <Arduino.h>

#define MAX_TABLES 4
#define START_LAMP_DURATION 1 // seconds

#define PIN_BUTTON_TABLE_0 19
#define PIN_BUTTON_TABLE_1 21
#define PIN_BUTTON_TABLE_2 20
#define PIN_BUTTON_TABLE_3 18

#define PIN_BUTTON_START_60 42
#define PIN_BUTTON_START_20 40
#define PIN_BUTTON_RESET 38
#define PIB_BUTTON_MODE 36 // ??

#define PIN_BUZZER 25

#define PIN_LED_TABLE_0 3
#define PIN_LED_TABLE_1 5
#define PIN_LED_TABLE_2 7
#define PIN_LED_TABLE_3 9
#define PIN_LED_START 15

#define PIN_PANEL_CLK 44
#define PIN_PANEL_DIO 46

#define BUTTON_PRESSED 0

#define STATE_INIT 0
#define STATE_WAITING 1
#define STATE_STARTED 2
#define STATE_STOPPED 3

#define TABLE_COLLISION byte(-2)
#define NO_TABLE byte(-1)
#define TABLE_0 0
#define TABLE_1 1
#define TABLE_2 2
#define TABLE_3 3

//#define START_SCREEN_TIME 1.00

#define ERROR_NOT_INITED 0
#define ERROR_UNKNOWN_STATE 1
#define ERROR_BUTTONS_COLLISION 2
