#include <Adafruit_MotorShield.h>
#include <math.h>
float current_angle =0, setpoint_angle = 0;
float error = 1, P = 0, I = 0, D = 0, PID_value = 0, output = 0;
int LSpeed = 200; //initial speed
int RSpeed = 200;


float visionRotation(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, int kp, float lr, float current_angle, float setpoint_angle){

  error = setpoint_angle - current_angle;
  if(error>0){
    output = kp * error * error;
  }else if(error<0){
    output = -kp * error * error;
  }
  output = constrain(output, -255/lr, 255/lr);
  
  if(error > 3){
    LMotor->run(BACKWARD);
    RMotor->run(FORWARD);
  }else if(error < -3){
    LMotor->run(FORWARD);
    RMotor->run(BACKWARD);
  }else if(error <= 3 && error >= -3){
    LMotor->setSpeed(0);
    RMotor->setSpeed(0);
    LMotor->run(RELEASE);
    RMotor->run(RELEASE);
    return output, true;
   }

  LMotor->setSpeed(abs((int)(lr*output)));
  RMotor->setSpeed(abs((int)output));
//  Serial.println(String("left")+output+String("        right")+(-output));
  return output, false;
}


float visionRotWithLeft(Adafruit_DCMotor *LMotor, int kp, float current_angle, float setpoint_angle){

  error = setpoint_angle - current_angle;
  if(error>0){
    output = kp * error * error;
  }else if(error<0){
    output = -kp * error * error;
  }
  output = constrain(output, -255, 255);
  
  if(output > 3){
    LMotor->run(BACKWARD);
  }else if(output < -3){
    LMotor->run(FORWARD);
  }else if(output <= 3 && output >= -3){
    LMotor->setSpeed(0);
    LMotor->run(RELEASE);
    return output, true;
  }

  LMotor->setSpeed(abs(output));
//  Serial.println(String("        left")+output);
//  Serial.println(String("error")+P+String("        PID_value")+output+String("       pi")+previous_I+String("       pe")+previous_error);
  return output, false;
}


float visionRotWithRight(Adafruit_DCMotor *RMotor, int kp, float current_angle, float setpoint_angle){
  error = setpoint_angle - current_angle;
  if(error>0){
    output = kp * error * error;
  }else if(error<0){
    output = -kp * error * error;
  }
  output = constrain(output, -255, 255);

  if(error > 3){
    RMotor->run(FORWARD);
  }else if(error < -3){
    RMotor->run(BACKWARD);
  }else if(error <= 3 && error >= -3){
    RMotor->setSpeed(0);
    RMotor->run(RELEASE);
    return output, true;
  }

  RMotor->setSpeed(abs(output));
//  Serial.println(String("        right")+output);
//  Serial.println(String("error")+P+String("        PID_value")+output+String("       pi")+previous_I+String("       pe")+previous_error);
  return output, false;
}


float visionLineFollow(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, int kp, float x1, float y1, float x2, float y2, float current_x, float current_y){

  error = -((x2-x1)*(y1-current_y)-(x1-current_x)*(y2-y1))/sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  P = error;
  if(P<0){
    PID_value = - kp * sqrt(-P);
  }else if(P>0){
    PID_value = kp * sqrt(P);
  }
  PID_value = constrain(PID_value, -55, 55);
  if((current_x - x2)*(current_x - x2) + (current_y - y2)*(current_y - y2) <= 120){
    LMotor->setSpeed(0);
    RMotor->setSpeed(0);
    LMotor->run(RELEASE);
    RMotor->run(RELEASE);
    return PID_value, true;
  }

  int left_motor_speed = LSpeed - PID_value;
  int right_motor_speed = RSpeed + PID_value;
  
  LMotor->setSpeed(left_motor_speed);
  RMotor->setSpeed(right_motor_speed);
//  Serial.println(String("error")+ error +String("        previous_error")+previous_error+String("       d")+D);
//  Serial.println(String("left")+left_motor_speed+String("        right")+right_motor_speed);
  return PID_value, false;
}


float visionLineFollowUltra(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, int kp, float x1, float y1, float x2, float y2, float current_x, float current_y){


  error = -((x2-x1)*(y1-current_y)-(x1-current_x)*(y2-y1))/sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  P = error;
  if(P<0){
    PID_value = - kp * sqrt(-P);
  }else if(P>0){
    PID_value = kp * sqrt(P);
  }
  PID_value = constrain(PID_value, -55, 55);

  int left_motor_speed = LSpeed - PID_value;
  int right_motor_speed = RSpeed + PID_value;
  
  LMotor->setSpeed(left_motor_speed);
  RMotor->setSpeed(right_motor_speed);
//  Serial.println(String("error")+ error);
//  Serial.println(String("left")+left_motor_speed+String("        right")+right_motor_speed);
  return PID_value;
}


float visionLineFollowColor(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, int kp, float x1, float y1, float x2, float y2, float current_x, float current_y){

  error = -((x2-x1)*(y1-current_y)-(x1-current_x)*(y2-y1))/sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  P = error;
  if(P<0){
    PID_value = - kp * sqrt(-P);
  }else if(P>0){
    PID_value = kp * sqrt(P);
  }
  PID_value = constrain(PID_value, -55, 55);

  int left_motor_speed = 100 - PID_value;
  int right_motor_speed = 100 + PID_value;
  
  LMotor->setSpeed(left_motor_speed);
  RMotor->setSpeed(right_motor_speed);
//  Serial.println(String("error")+ error +String("        previous_error")+previous_error+String("       d")+D);
//  Serial.println(String("left")+left_motor_speed+String("        right")+right_motor_speed);
  return PID_value;
}
