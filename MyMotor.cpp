#include "MyMotor.h"

#include <Arduino.h>

#include <math.h>

void MyMotor::init() {
  pinMode(_pin_1, OUTPUT);
  pinMode(_pin_2, OUTPUT);
  pinMode(_pin_pwm, OUTPUT);
}

void MyMotor::setSpeed(float s) {
  _speed = s * (_reversed ? -1.0 : 1.0);
  if(_speed > 1.0) _speed = 1.0;
  if(_speed < -1.0) _speed = -1.0;
}

void MyMotor::turn() {
  if (_speed == 0.0) {
    digitalWrite(_pin_1, LOW);
    digitalWrite(_pin_2, LOW);
  } else {
     digitalWrite(_pin_1, _speed < 0.0 ? LOW : HIGH);
     digitalWrite(_pin_2, _speed < 0.0 ? HIGH : LOW);
  }
  analogWrite(_pin_pwm, abs(_speed) * 255);
}

void MyMotor::update() {
  turn();
}
