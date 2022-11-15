#include <Arduino.h>
#include "Timer.h"

Timer::Timer() {

}

void Timer::start(uint32_t target) {
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
  
  value = millis() - _startTime;

  if (value >= _target) {
    value = _target;
    isRunning = false;
    isFinished = true;
    isUpdated = true;
  }
  
  uint32_t precision = is_finishing() ? TIMER_PRECISION_FINISHING : TIMER_PRECISION;

  if ((value - _oldValue) >= precision) {
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

bool Timer::is_finishing() {
  uint32_t finishing_value = millis() - _startTime;
  return (_target - finishing_value) <= IS_FINISHING;
}
