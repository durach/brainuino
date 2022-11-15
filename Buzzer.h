#include <Arduino.h>

#define BUZZER_FREQUENCY_START 800 // Hz
#define BUZZER_FREQUENCY_TABLE 700 // Hz
#define BUZZER_FREQUENCY_FALSE_START 600 // Hz
#define BUZZER_FREQUENCY_FINISH 550 // Hz

#define BUZZER_SOUND_DURATION 1000 // ms

class Buzzer {

  private:
    uint8_t _pin;

  public:
    Buzzer(uint8_t pin);
    
    void playStartSound();
    void playTableSound();
    void playFalseStartSound();
    void playFinishSound();
    void off();
};
