#include "Button.h"

#include <Arduino.h>

void Button::init() {
  pinMode(_pin, INPUT);
  _is_pressed = digitalRead(_pin);
}

void Button::update() {
  bool v = digitalRead(_pin);
  if(_callback && (v != _is_pressed)) {
    (*_callback)(v);
  }
  
  _is_pressed = v;
}
