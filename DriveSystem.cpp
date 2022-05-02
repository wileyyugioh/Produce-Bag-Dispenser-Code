#include "DriveSystem.h"

void DriveSystem::moveDistance(long a_dist, long b_dist, void(*callback)(DriveSystem&)) {
  _state = MOVE_DISTANCE;
  if(a_dist != 0) {
    a_done = false;
    motor_a.moveDistance(a_dist, this, [](DriveSystem& ds) {
      ds.a_done = true;
    });
  } else {
    a_done = true;
  }

  if(b_dist != 0) {
    b_done = false;
    motor_b.moveDistance(b_dist, this, [](DriveSystem& ds) {
      ds.b_done = true;
    });
  } else {
    b_done = true;
  }

  _callback = callback;
}

void DriveSystem::wait(unsigned long ms, void(*callback)(DriveSystem&)) {
  _state = WAIT;
  unsigned long now = millis();
  _finish_time = now + ms;

  _callback = callback;
}

void DriveSystem::init() {
      motor_a.init();
      motor_a.setSpeed(1.00);

      motor_b.init();
      motor_b.setSpeed(1.00);
      motor_b.reverse(true);
}

void DriveSystem::update() {
      motor_a.update();
      motor_b.update();

      switch(_state) {
        case MOVE_DISTANCE: {
          if(a_done && b_done) {
            a_done = false;
            b_done = false;
            auto myCallback = _callback;
            _callback = nullptr;
            _state = NONE;
            (*myCallback)(*this);
          }
          break;
        }
        case WAIT: {
          unsigned long now = millis();
          if(now >= _finish_time) {
            auto myCallback = _callback;
            _callback = nullptr;
            _state = NONE;
            (*myCallback)(*this);
          }
          break;
        }
        default: {
          break;
        }
      }
}

void DriveSystem::startSequence() {
  if(_state != NONE) return;
  // One revolution is approximately 500 units
  moveDistance(500 / 4, 500 / 4, [](DriveSystem& ds) {
    ds.wait(1000, [](DriveSystem& ds) {
      ds.moveDistance(0, 500.0 * 0.50, [](DriveSystem& ds) {
        ds.wait(1000, [](DriveSystem& ds) {
          ds.moveDistance(-500 * 4.7, -500 * 4.7, nullptr);    
        });
      }); 
    });
  });
  Serial.println("Starting sequence");
}
