#ifndef Button_h_
#define Button_h_

#include <stdint.h>

class Button {
  private:
    bool _is_pressed;
    uint8_t _pin;
    void (*_callback)(bool) = nullptr;

  public:
    Button(uint8_t pin) : _pin(pin) {};
    void init();
    void update();
    void onChange(void (*callback)(bool)) {
      _callback = callback;
    };
};

#endif
