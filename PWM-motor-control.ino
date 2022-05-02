#include "DriveSystem.h"
#include "Button.h"

Button ir_sensor(13);
DriveSystem ds;

void onIRChange(bool is_pressed) {
  onButtonChange(!is_pressed);
}

void onButtonChange(bool is_pressed) {
  if (!is_pressed) return;
  if (ds.inSequence()) return;
  ds.startSequence();
}

void setup()
{
  Serial.begin (115200);
  Serial.println("Initialized");

  ir_sensor.init();
  ir_sensor.onChange(&onIRChange);

  ds.init();
}

void loop()
{
  ir_sensor.update();
  ds.update();
}
