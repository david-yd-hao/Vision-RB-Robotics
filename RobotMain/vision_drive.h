#include <Adafruit_MotorShield.h>
#include <math.h>
float current_angle =0, setpoint_angle = 0;
float error = 1, P = 0, I = 0, D = 0, PID_value = 0, output = 0;
int LSpeed = 200; //initial speed
int RSpeed = 200;


float visionRotation(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, float kp, float lr, float current_angle, float setpoint_angle){

  if(setpoint_angle == 360 && current_angle < 180){
    error = 0 - current_angle;
    }else if(setpoint_angle == 360 && current_angle > 180){
    error = 360 - current_angle;
  }else if(setpoint_angle != 360){
    error = setpoint_angle - current_angle;
  }
  
  if(error>0){
output = kp * error * error;
  }else if(error<0){
output = -kp * error * error;
}
  output = constrain(output, -180/lr, 180/lr);
  Serial.println(String("erorr    ") + error);
  if(error > 4){
    LMotor->run(BACKWARD);
    RMotor->run(FORWARD);
  }else if(error < -4){
    LMotor->run(FORWARD);
    RMotor->run(BACKWARD);
  }else if(error <= 4 && error >= -4){
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


float visionRotWithLeft(Adafruit_DCMotor *LMotor, float kp, float current_angle, float setpoint_angle){

  error = setpoint_angle - current_angle;
  if(error>0){
    output = kp * error * error;
  }else if(error<0){
    output = -kp * error * error;
  }
  output = constrain(output, -180, 180);
  
  if(output > 4){
    LMotor->run(BACKWARD);
  }else if(output < -4){
    LMotor->run(FORWARD);
  }else if(output <= 4 && output >= -4){
    LMotor->setSpeed(0);
    LMotor->run(RELEASE);
    return output, true;
  }

  LMotor->setSpeed(abs(output));
//  Serial.println(String("        left")+output);
//  Serial.println(String("error")+P+String("        PID_value")+output+String("       pi")+previous_I+String("       pe")+previous_error);
  return output, false;
}


float visionRotWithRight(Adafruit_DCMotor *RMotor, float kp, float current_angle, float setpoint_angle){
  error = setpoint_angle - current_angle;
  if(error>0){
    output = kp * error * error;
  }else if(error<0){
    output = -kp * error * error;
  }
  output = constrain(output, -180, 180);

  if(error > 4){
    RMotor->run(FORWARD);
  }else if(error < -4){
    RMotor->run(BACKWARD);
  }else if(error <= 4 && error >= -4){
    RMotor->setSpeed(0);
    RMotor->run(RELEASE);
    return output, true;
  }

  RMotor->setSpeed(abs(output));
//  Serial.println(String("        right")+output);
//  Serial.println(String("error")+P+String("        PID_value")+output+String("       pi")+previous_I+String("       pe")+previous_error);
  return output, false;
}


float visionLineFollow(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, int kp, float x1, float y1, float x2, float y2, float current_x, float current_y, int tolerance){

  error = -((x2-x1)*(y1-current_y)-(x1-current_x)*(y2-y1))/sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  P = error;
  if(P<0){
    PID_value = - kp * sqrt(-P);
  }else if(P>0){
    PID_value = kp * sqrt(P);
  }
  PID_value = constrain(PID_value, -55, 55);
  if((current_x - x2)*(current_x - x2) + (current_y - y2)*(current_y - y2) <= tolerance){
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
