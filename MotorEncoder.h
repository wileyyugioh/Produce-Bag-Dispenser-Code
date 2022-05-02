#ifndef MotorEncoder_h_
#define MotorEncoder_h_

#include "MyMotor.h"
#include "Encoder.h"

struct MyEncoderData {
  long pos;
  long dPos;
  unsigned long dT;
};

class MyEncoder {
  private:  
    Encoder _enc;
    long _startPos;
    bool _reversed = false;
    unsigned long _startTime;

  public:
    MyEncoder(uint8_t pin1, uint8_t pin2) : _enc(pin1, pin2) {}

    void init() {
      _startPos = _enc.read();
      _startTime = millis();
    };

    void reverse(bool v) {_reversed = v;};

    MyEncoderData data() {
      long newPos = _enc.read();
      unsigned long now = millis();

      MyEncoderData data{newPos, newPos - _startPos, now - _startTime};
      data.pos *= _reversed ? -1 : 1;
      data.dPos *= _reversed ? -1 : 1;
      //Serial.print("Avg speed is: ");
      //Serial.print(pulse);
      //Serial.println(" pulses per second");
      _startTime = now;
      _startPos = newPos;
      return data;
    };
};

template <typename T>
class MotorEncoder {
  private:
    enum STATES {
      STARTED,
      STOPPED
    };
  
    MyMotor _myMotor;
    MyEncoder _myEncoder;
    bool _reversed = false;
    float _speed = 1.0;
    long _desiredPos;
    STATES _state = STOPPED;
    T* _callback_data = nullptr;
    void (*_callback)(T&) = nullptr;
    bool _direction_fwd = true;
    

  public:
    MotorEncoder(uint8_t motor_pin_1, uint8_t motor_pin_2, uint8_t motor_pin_pwm, uint8_t encoder_pin_1, uint8_t encoder_pin_2) :
      _myMotor(motor_pin_1, motor_pin_2, motor_pin_pwm),
      _myEncoder(encoder_pin_1, encoder_pin_2) {};
 
    void init() {
      _myMotor.init();
      _myEncoder.init();
    };

    void reverse(bool r) {
      _myEncoder.reverse(r);
      _myMotor.reverse(r);
    };

    void setSpeed(float s) {
      _speed = s;
      if(_speed > 1.0) _speed = 1.0;
      if(_speed < 0.0) _speed = 0.0;
    };

    void update() {
      if (_state == STARTED) {
        MyEncoderData data = _myEncoder.data();
        bool do_stop = _direction_fwd ? (data.pos > _desiredPos) : (data.pos < _desiredPos);
        if (do_stop) {
          _state = STOPPED;
          _myMotor.setSpeed(0);
          if (_callback) {
            auto myCallback = _callback;
            auto data = _callback_data;
            _callback = nullptr;
            _callback_data = nullptr;
            (*myCallback)(*data);
          }
        } else {
          float new_speed = _speed * (_direction_fwd ? 1.0 : -1.0);
          _myMotor.setSpeed(new_speed);
        }
     } else {
        _myMotor.setSpeed(0);
     }
     _myMotor.update();
    };

    void moveDistance(long d, T* t, void(*callback)(T&)) {
        if(_state == STARTED && _callback) {
          _callback(*_callback_data);
        }
        MyEncoderData data = _myEncoder.data();
        _desiredPos = data.pos + d;
        _state = STARTED;
        _callback_data = t;
        _callback = callback;
        _direction_fwd = (d > 0.0);
    };

    bool isMoving() const {return _state == STARTED;};
};

#endif
