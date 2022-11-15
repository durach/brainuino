#define MAX_TABLES 6
#define START_LAMP_DURATION  1 // seconds

#define PIN_BUTTON_TABLE_0  62 // PCINT16 / A8
#define PIN_BUTTON_TABLE_1  63 // PCINT17 / A9
#define PIN_BUTTON_TABLE_2  64 // PCINT18 / A10
#define PIN_BUTTON_TABLE_3  65 // PCINT19 / A11
#define PIN_BUTTON_TABLE_4  66 // PCINT20 / A12
#define PIN_BUTTON_TABLE_5  67 // PCINT21 / A13

#define PIN_BUTTON_START_60  2 // INT4
#define PIN_BUTTON_START_20 18 // INT3
#define PIN_BUTTON_RESET    19 // INT2
#define PIN_BUTTON_MODE     3 

#define PIN_BUZZER 25

#define PIN_LED_TABLE_0 54 // A0
#define PIN_LED_TABLE_1 55 // A1
#define PIN_LED_TABLE_2 68 // A14
#define PIN_LED_TABLE_3 69 // A15
#define PIN_LED_TABLE_4 32 // D32
#define PIN_LED_TABLE_5 33 // D33

#define PIN_LED_START 15

#define PIN_LED_START_60 5
#define PIN_LED_START_20 7

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

// Bits in register PINK

#define STATE_BUTTONS_TABLE_INIT      B00111111
#define STATE_BUTTONS_TABLE_0_PRESSED B00111110
#define STATE_BUTTONS_TABLE_1_PRESSED B00111101
#define STATE_BUTTONS_TABLE_2_PRESSED B00111011
#define STATE_BUTTONS_TABLE_3_PRESSED B00110111
#define STATE_BUTTONS_TABLE_4_PRESSED B00101111
#define STATE_BUTTONS_TABLE_5_PRESSED B00011111

#define MODE_BR 0
#define MODE_SG 1
