#include <Arduino.h>
#include "Timer.h"

Timer::Timer() {

}

void Timer::start(double target) {
  _target = target;
  _startTime = millis();
  isRunning = true;
  isUpdated = true;
}

void Timer::stop() {
  isRunning = false;
  isUpdated = true;
}

void Timer::tick() {
  if (!isRunning) {
    return;
  }
  
  value = (millis() - _startTime) / TIMER_MILLISECONDS_IN_SECOND;

  if (value >= _target) {
    value = _target;
    isRunning = false;
    isUpdated = true;
  }
  
  if (!isUpdated && (abs(value - _oldValue) > TIMER_PERCISION)) {
    _oldValue = value;
    isUpdated = true;
  }
}

void Timer::reset() {
  value = 0;
  isRunning = false;
}
