#include <Servo.h>

Servo servo_claw;

const int servo_claw_port = 9; // inner one 9
const int pos_open = 180, pos_closed = 90;

// the following functions give combinations of the servo position and movements

void openClaw(){
    servo_claw.write(pos_open);
    delay(1000);
}

void closeClaw(){
    servo_claw.write(pos_closed);
    delay(1000);
}

void attach_servo(){
  // attach all servo
  servo_claw.attach(servo_claw_port);
}