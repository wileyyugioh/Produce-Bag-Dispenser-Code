#ifndef MyMotor_h_
#define MyMotor_h_

#include <stdint.h>

class MyMotor {
  private:
    uint8_t _pin_1, _pin_2, _pin_pwm;
    bool _reversed = false;
    float _speed = 0.0;

    void turn();
  public:
    MyMotor (uint8_t pin_1, uint8_t pin_2, uint8_t pin_pwm) : _pin_1(pin_1), _pin_2(pin_2), _pin_pwm(pin_pwm) {};

    void init();
    void reverse(bool v) { _reversed = v; _speed *= -1.0;};
    void setSpeed(float s);
    void update();
};

#endif
