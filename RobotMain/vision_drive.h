#include <Adafruit_MotorShield.h>

const float Kp = 20, Ki = 5, Kd = 1;
float current_angle =0, setpoint_angle = 0;
float error = 0, P = 0, I = 0, D = 0, PID_value = 0, output = 0;
float previous_error = 0, previous_I = 0;
int LSpeed = 150; //initial speed
int RSpeed = 150;


float visionLineDrive(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, float current_angle, float setpoint_angle, float previous_error, float previous_I){
  
  error = setpoint_angle - current_angle;
  P = error;
  I = error + previous_I;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_I = I;
  previous_error = error;
  int left_motor_speed = LSpeed - PID_value;
  int right_motor_speed = RSpeed + PID_value;
  constrain(left_motor_speed, 0, 255);
  constrain(right_motor_speed, 0, 255);

  LMotor->setSpeed(left_motor_speed);
  RMotor->setSpeed(right_motor_speed);
  Serial.println((char)P+(char)output+(char)previous_error+(char)previous_I);
  return output, previous_error, previous_I;
}


float visionRotation(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, float current_angle, float setpoint_angle, float previous_error, float previous_I){

  error = setpoint_angle - current_angle;
  P = error;
  I = error + previous_I;
  D = error - previous_error;
  output = (Kp * P) + (Ki * I) + (Kd * D);
  previous_I = I;
  previous_error = error;
  constrain(output, 0, 255);

  if(output > 1){
    LMotor->run(FORWARD);
    RMotor->run(BACKWARD);
  }else if(output < 1){
    LMotor->run(BACKWARD);
    RMotor->run(FORWARD);
  }else if(output <= 1 && output >= -1){
    LMotor->setSpeed(0);
    RMotor->setSpeed(0);
    LMotor->run(RELEASE);
    RMotor->run(RELEASE);
    Serial.println((char)P+(char)output+(char)previous_error+(char)previous_I);
    return output, previous_error, previous_I;
   }

  LMotor->setSpeed(abs(output));
  RMotor->setSpeed(abs(output));
  Serial.println((char)P+(char)output+(char)previous_error+(char)previous_I);
  return output, previous_error, previous_I;
}


float visionRotWithLeft(Adafruit_DCMotor *LMotor, float current_angle, float setpoint_angle, float previous_error, float previous_I){

  error = setpoint_angle - current_angle;
  P = error;
  I = error + previous_I;
  D = error - previous_error;
  output = (Kp * P) + (Ki * I) + (Kd * D);
  previous_I = I;
  previous_error = error;
  constrain(output, 0, 255);
  
  if(output > 1){
    LMotor->run(BACKWARD);
  }else if(output < 1){
    LMotor->run(FORWARD);
  }else if(output <= 1 && output >= -1){
    LMotor->setSpeed(0);
    LMotor->run(RELEASE);
    Serial.println((char)P+(char)output+(char)previous_error+(char)previous_I);
    return output, previous_error, previous_I;
  }

  LMotor->setSpeed(abs(output));
  Serial.println((char)P+(char)output+(char)previous_error+(char)previous_I);
  return output, previous_error, previous_I;
}


float visionRotWithRight(Adafruit_DCMotor *RMotor, float current_angle, float setpoint_angle, float previous_error, float previous_I){

  error = setpoint_angle - current_angle;
  P = error;
  I = error + previous_I;
  D = error - previous_error;
  output = (Kp * P) + (Ki * I) + (Kd * D);
  previous_I = I;
  previous_error = error;
  constrain(output, 0, 255);

  if(output > 1){
    RMotor->run(BACKWARD);
  }else if(output < 1){
    RMotor->run(FORWARD);
  }else if(output <= 1 && output >= -1){
    RMotor->setSpeed(0);
    RMotor->run(RELEASE);
    Serial.println((char)P+(char)output+(char)previous_error+(char)previous_I);
    return output, previous_error, previous_I;
  }

  RMotor->setSpeed(abs(output));
  Serial.println((char)P+(char)output+(char)previous_error+(char)previous_I);
  return output, previous_error, previous_I;
}
