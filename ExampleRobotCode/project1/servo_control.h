#include <Servo.h> 
Servo servo_lift, servo_claw;

const int servo_lift_port = 10, servo_claw_port = 9; // inner one 9, outer one 10

const int pos_lift = 110, pos_lay = 32, pos_catch = 163, pos_release = 35, pos_put = 35, pos_half_lift = 100;

// the following functions give combinations of the servo position and movements


void half_lift(){
  servo_lift.write(pos_half_lift);
}


void lift()
{
  servo_lift.write(pos_lift);
}

void lay(){
  servo_lift.write(pos_lay);
}

void capture(){
  servo_claw.write(pos_catch);
}

void releasse(){
  servo_claw.write(pos_release);
}

void ini(){
  lay();
  releasse();
}

void ini_h(){
  lift();delay(200);
  releasse();
}

void rise(){
  lift();
  releasse();
}

void half_rise(){
  half_lift();
  releasse();
}

void grab(){
  lay();
  delay(700);
  capture();
  delay(700);
  lift();
  delay(700);
}

void deploy(){
  // deploy a dummy with the slow deploy method
  servo_lift.write(pos_put);
  delay(600);
  int total_time = 5000;
  int position_extra = 70;
  int every_step = total_time / position_extra;
  int claw_value = pos_catch;
  unsigned long t = millis();
  while (claw_value > pos_catch - 30){
    claw_value -= 1;
    servo_claw.write(claw_value);
    delay(every_step);
  }
  releasse();
  delay(500);
  run(-255, -255); delay(200);run(0,0);
  lift();
  delay(500);
}

void attach_all_servo(){
  // attach all servo
  servo_lift.attach(servo_lift_port);
  servo_claw.attach(servo_claw_port);
}

void detach_all_servo(){
  // detach all servo
  servo_lift.detach();
  servo_claw.detach();
}
