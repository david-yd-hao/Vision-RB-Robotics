#include <Adafruit_MotorShield.h>
#include <math.h>
const float Kp = 1, Ki = 0, Kd = 0.1;
float current_angle =0, setpoint_angle = 0;
float error = 1, P = 0, I = 0, D = 0, PID_value = 0, output = 0;
int LSpeed = 200; //initial speed
int RSpeed = 200;


float visionRotation(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, float lr, float current_angle, float setpoint_angle, float previous_error, float previous_I){

  error = setpoint_angle - current_angle;
  P = error;
  I = error + previous_I;
  D = error - previous_error;
  output = (6 * P) + (0 * I) + (0 * D);
  previous_I = I;
  previous_error = error;
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
    return output, previous_error, previous_I, true;
   }

  LMotor->setSpeed(abs((int)(lr*output)));
  RMotor->setSpeed(abs((int)output));
//  Serial.println(String("left")+output+String("        right")+(-output));
  return output, previous_error, previous_I, false;
}


float visionRotWithLeft(Adafruit_DCMotor *LMotor, float current_angle, float setpoint_angle, float previous_error, float previous_I){

  error = setpoint_angle - current_angle;
  P = error;
  I = error + previous_I;
  D = error - previous_error;
  output = (10 * P) + (0 * I) + (0 * D);
  previous_I = I;
  previous_error = error;
  output = constrain(output, -255, 255);
  
  if(output > 4){
    LMotor->run(BACKWARD);
  }else if(output < -4){
    LMotor->run(FORWARD);
  }else if(output <= 4 && output >= -4){
    LMotor->setSpeed(0);
    LMotor->run(RELEASE);
    return output, error, I, true;
  }

  LMotor->setSpeed(abs(output));
//  Serial.println(String("        left")+output);
//  Serial.println(String("error")+P+String("        PID_value")+output+String("       pi")+previous_I+String("       pe")+previous_error);
  return output, error, I, false;
}


float visionRotWithRight(Adafruit_DCMotor *RMotor, float current_angle, float setpoint_angle, float previous_error, float previous_I){
  error = setpoint_angle - current_angle;
  P = error;
  I = error + previous_I;
  D = error - previous_error;
  output = (10 * P) + (0 * I) + (0 * D);
  output = constrain(output, -255, 255);

  if(error > 4){
    RMotor->run(FORWARD);
  }else if(error < -4){
    RMotor->run(BACKWARD);
  }else if(error <= 4 && error >= -4){
    RMotor->setSpeed(0);
    RMotor->run(RELEASE);
    return output, error, I, true;
  }

  RMotor->setSpeed(abs(output));
//  Serial.println(String("        right")+output);
//  Serial.println(String("error")+P+String("        PID_value")+output+String("       pi")+previous_I+String("       pe")+previous_error);
  return output, error, I, false;
}


float visionLineFollow(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, float x1, float y1, float x2, float y2, float current_x, float current_y, float previous_error, float previous_I){


  error = -((x2-x1)*(y1-current_y)-(x1-current_x)*(y2-y1))/sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  P = error;
  I = error + previous_I;
  D = error - previous_error;
  PID_value = (1.5 * P) + (0 * I);
  PID_value = constrain(PID_value, -55, 55);
  if((current_x - x2)*(current_x - x2) + (current_y - y2)*(current_y - y2) <= 140){
    LMotor->setSpeed(0);
    RMotor->setSpeed(0);
    LMotor->run(RELEASE);
    RMotor->run(RELEASE);
    return PID_value, error, I, true;
  }

  int left_motor_speed = LSpeed - PID_value;
  int right_motor_speed = RSpeed + PID_value;
  
  LMotor->setSpeed(left_motor_speed);
  RMotor->setSpeed(right_motor_speed);
//  Serial.println(String("error")+ error +String("        previous_error")+previous_error+String("       d")+D);
//  Serial.println(String("left")+left_motor_speed+String("        right")+right_motor_speed);
  return PID_value, error, I, false;
}


float visionLineFollowUltra(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, float x1, float y1, float x2, float y2, float current_x, float current_y, float previous_error, float previous_I){


  error = -((x2-x1)*(y1-current_y)-(x1-current_x)*(y2-y1))/sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  P = error;
  I = error + previous_I;
  D = error - previous_error;

  PID_value = (1.5 * P) + (0 * I);
  PID_value = constrain(PID_value, -55, 55);

  int left_motor_speed = LSpeed - PID_value;
  int right_motor_speed = RSpeed + PID_value;
  
  LMotor->setSpeed(left_motor_speed);
  RMotor->setSpeed(right_motor_speed);
//  Serial.println(String("error")+ error);
//  Serial.println(String("left")+left_motor_speed+String("        right")+right_motor_speed);
  return PID_value, error, I;
}


float visionLineFollowColor(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, float x1, float y1, float x2, float y2, float current_x, float current_y, float previous_error, float previous_I){

  error = -((x2-x1)*(y1-current_y)-(x1-current_x)*(y2-y1))/sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  P = error;
  I = error + previous_I;
  D = error - previous_error;

  PID_value = (1.5 * P) + (0 * I);
  PID_value = constrain(PID_value, -55, 55);

  int left_motor_speed = 100 - PID_value;
  int right_motor_speed = 100 + PID_value;
  
  LMotor->setSpeed(left_motor_speed);
  RMotor->setSpeed(right_motor_speed);
//  Serial.println(String("error")+ error +String("        previous_error")+previous_error+String("       d")+D);
//  Serial.println(String("left")+left_motor_speed+String("        right")+right_motor_speed);
  return PID_value, error, I;
}
