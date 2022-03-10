#include <Adafruit_MotorShield.h>


void stop(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor){
  LMotor->setSpeed(0);
  RMotor->setSpeed(0);
  delay(1000);
}
