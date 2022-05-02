#ifndef DriveSystem_h_
#define DriveSystem_h_

#include "MotorEncoder.h"

class DriveSystem {
  private:
    // Embedded system state machine
    // Update code is going to be a big switch statement
    enum STATES {
      NONE,
      MOVE_DISTANCE,
      WAIT
    };

    STATES _state = NONE;
    MotorEncoder<DriveSystem> motor_a{8, 9, 10, 12, 11}; // Top (WIRE ORANGE 11 and RED 12)
    MotorEncoder<DriveSystem> motor_b{6, 7, 5, 3, 4}; // Bottom (WIRE ORANGE 3 AND BROWN 4)

    // Callback variable shared with different events
    void(*_callback)(DriveSystem&) = nullptr;

    // Move Distance Code
    void moveDistance(long a_dist, long b_dist, void(*callback)(DriveSystem&));
    bool a_done = false;
    bool b_done = false;

    // Wait Code
    void wait(unsigned long ms, void(*callback)(DriveSystem&));
    unsigned long _finish_time;
  public:
    void init();
    void update();
    void startSequence();
    bool inSequence() const {return _state != NONE;};
};

#endif
