#include <Servo.h> 

Servo gripservo, iiftservo;

int servo_port_grip = 9; // inner one, brown out
int servo_port_lift = 10; // inner one, brown out

int open_grip_angle = 160;
int close_grip_angle = 0;
int open_lift_angle = 140;
int close_lift_angle = 0;
int flag = 0; //set flag to differentiate if claw is opened (0) or closed (1)
// flag should be set in project1 file so that it doesn't reset every time it is called or maybe not?

void activateclaw() 
{
 if(flag == 0){
 Serial.print("Claw is closed ");
 gripservo.write(close_grip_angle);
 delay(1000);
 liftservo.write(close_lift_angle);
 flag = 1;
 }else if(flag == 1){
 Serial.print("Claw is opened ");
 liftservo.write(open_lift_angle);
 gripservo.write(open_grip_angle);
 delay(1000);
 gripservo.write(open_grip_angle);
 flag = 0;
 }
}
