#include <Arduino.h>

#define MAX_TABLES 6
#define START_LAMP_DURATION 1 // seconds

#define PIN_BUTTON_TABLE_0 53
#define PIN_BUTTON_TABLE_1 52
#define PIN_BUTTON_TABLE_2 51
#define PIN_BUTTON_TABLE_3 50
#define PIN_BUTTON_TABLE_4 10
#define PIN_BUTTON_TABLE_5 11

#define PIN_BUTTON_START_60 42
#define PIN_BUTTON_START_20 40
#define PIN_BUTTON_RESET 38
#define PIB_BUTTON_MODE 36 // ??

#define PIN_BUZZER 25

#define PIN_LED_TABLE_0 3
#define PIN_LED_TABLE_1 4
#define PIN_LED_TABLE_2 5
#define PIN_LED_TABLE_3 6
#define PIN_LED_TABLE_4 7
#define PIN_LED_TABLE_5 8

#define PIN_LED_START 15

#define PIN_PANEL_CLK 44
#define PIN_PANEL_DIO 46

#define BUTTON_PRESSED 0

#define STATE_INIT 0
#define STATE_WAITING 1
#define STATE_STARTED 2
#define STATE_STOPPED 3

#define NO_TABLE int8_t(-1)
#define TABLE_0 0
#define TABLE_1 1
#define TABLE_2 2
#define TABLE_3 3
#define TABLE_4 4
#define TABLE_5 5

//#define START_SCREEN_TIME 1.00

#define ERROR_NOT_INITED 0
#define ERROR_UNKNOWN_STATE 1
#define ERROR_BUTTONS 2
