#include <Arduino.h>
#include "Timer.h"

Timer::Timer() {

}

void Timer::start(double target) {
  _target = target;
  _startTime = millis();
  isRunning = true;
  isUpdated = true;
  isFinished = false;
}

void Timer::stop(bool updated) {
  isRunning = false;
  isUpdated = updated;
}

void Timer::tick() {
  if (!isRunning) {
    return;
  }
  
  value = (millis() - _startTime) / TIMER_MILLISECONDS_IN_SECOND;

  if (value >= _target) {
    value = _target;
    isRunning = false;
    isFinished = true;
    isUpdated = true;
  }
  
  if ((value - _oldValue) > TIMER_PRECISION) {
    _oldValue = value;
    isUpdated = true;
  }
}

void Timer::reset() {
  value = 0;
  _oldValue = 0;
  isRunning = false;
  isFinished = false;
  isUpdated = true;
}
