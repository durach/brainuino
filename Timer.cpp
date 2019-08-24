#include <Arduino.h>
#include "Timer.h"

Timer::Timer() {

}

void Timer::start(double target) {
  _target = target;
  _start_time = millis();
  is_running = true;
  is_updated = true;
}

void Timer::stop() {
  is_running = false;
  is_updated = true;
}

void Timer::tick() {
  if (!is_running) {
    return;
  }
  
  value = (millis() - _start_time) / TIMER_MILLISECONDS_IN_SECOND;

  if (value >= _target) {
    value = _target;
    is_running = false;
    is_updated = true;
  }
  
  if (!is_updated && (abs(value - _old_value) > TIMER_PERCISION)) {
    _old_value = value;
    is_updated = true;
  }
}

void Timer::reset() {
  value = 0;
  is_running = false;
}

