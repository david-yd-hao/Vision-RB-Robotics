#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

const float left_white = 25, left_black = 550; // should be raw data (0 - 1023)
const float right_white = 25, right_black = 560;


Adafruit_MotorShield AFMS = Adafruit_MotorShield();

const int red_LED_port = 13, button_port = 7, green_LED_port = 8, yellow_LED_port = 12;

const int trigPin = 1; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 2; // Echo Pin of Ultrasonic Sensor

unsigned long US_time_out = 6000;
const int left_motor_port = 3, right_motor_port = 4;

Adafruit_DCMotor *L_Motor = AFMS.getMotor(left_motor_port);
Adafruit_DCMotor *R_Motor = AFMS.getMotor(right_motor_port);


const int left_light_sensor_port = A2, right_light_sensor_port = A1; 


void toggle_yellow_led_r() {
  //change state for yellow LED
  digitalWrite(yellow_LED_port, !digitalRead(yellow_LED_port)); // toggle the LED
}

unsigned long blink_period = millis();
void run(int left_speed, int right_speed)
{
  // vehicle run
  left_speed = (float)left_speed * (-0.97);
  right_speed = (float)right_speed * (-1.0);
  if ((millis() - blink_period) > 500){blink_period = millis(); toggle_yellow_led_r();}
  if (left_speed == -1000 || left_speed == 1000) {}
  else if (left_speed >= 0){
    if (left_speed > 255){left_speed = 255;}
    L_Motor->setSpeed(left_speed);
    L_Motor->run(FORWARD);
  }
  else if (left_speed < 0){
    if (left_speed < -255){left_speed = 255;}
    L_Motor->setSpeed(-left_speed);
    L_Motor->run(BACKWARD);
  }
  
  if (right_speed == -1000 || right_speed == 1000) {}
  else if (right_speed >= 0){
    if (right_speed > 255){right_speed = 255;}
    R_Motor->setSpeed(right_speed);
    R_Motor->run(FORWARD);
  }
  else if (right_speed < 0){
    if (right_speed < -255){right_speed = 255;}
    R_Motor->setSpeed(-right_speed);
    R_Motor->run(BACKWARD);
  }
  
}

float LS_L_r(){
  // raw data for line sensor right
  float voltage = analogRead(left_light_sensor_port);
  return voltage;
}

float LS_R_r(){
  // raw data for line sensor left
  float voltage = analogRead(right_light_sensor_port);
  return voltage;
}

float LS_L_v(){
  // voltage data for line sensor left
  float voltage = analogRead(left_light_sensor_port) * 5.0 / 1023.0;
  return voltage;
}

float LS_R_v(){
  // voltage data for line sensor right
  float voltage = analogRead(right_light_sensor_port) * 5.0 / 1023.0;
  return voltage;
}

float map_n(float x, float in_min, float in_max, float out_min, float out_max) {
  // rewrite arduino map function for float
  float result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return result;
}

int button(){
  return digitalRead(button_port);
}

void red_LED(int value){
  // change red_LED
  if (value == 0) {digitalWrite(red_LED_port, LOW);}
  else if (value == 1){digitalWrite(red_LED_port, HIGH);}
}

void red_blink(int t){
  // blink red LED
  red_LED(1);
  delay(t);
  red_LED(0);
}

void green_LED(int value){
  // change green LED
  if (value == 0) {digitalWrite(green_LED_port, LOW);}
  else if (value == 1){digitalWrite(green_LED_port, HIGH);}
}

void green_blink(int t){
  // blink green LED
  green_LED(1);
  delay(t);
  green_LED(0);
}

void gr_LED(int value){
  // change green and red LED
  if (value == 0) {digitalWrite(red_LED_port, LOW);digitalWrite(green_LED_port, LOW);}
  else if (value == 1){digitalWrite(red_LED_port, HIGH);digitalWrite(red_LED_port, HIGH);}
}

void gr_blink(int t){
  // blink green and red LED
  green_LED(1);
  red_LED(1);
  delay(t);
  green_LED(0);
  red_LED(0);
}

void yellow_blink(int t){
  // blink yellow LED
  digitalWrite(yellow_LED_port, 1);delay(t); digitalWrite(yellow_LED_port, 0);
}



bool toggle_yellow_led(void *) {
  // toggle yellow red
  digitalWrite(yellow_LED_port, !digitalRead(yellow_LED_port)); // toggle the LED
  return true; // repeat? true
}

float LS_L(){
  // processed (map to 0 - 255) data on line sensor left
  float sensor_value = LS_L_r();
  float result = map_n(sensor_value, left_black, left_white, 0, 255);
  return result;
}

float LS_R(){
  // processed (map to 0 - 255) data on line sensor right
  float sensor_value = LS_R_r();
  float result = map_n(sensor_value, right_black, right_white, 0, 255);
  return result;
}


long US() {
  // ultrasonic sensor read in cm, with timeout
  long duration, result;
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH, US_time_out);
   if (duration == 0){result=0;}
   else {result =duration / 29 / 2;}
   return result;
}

void turn_around(int spd, int dir, unsigned long t, bool blink_flag){
  // turn around with fixed time and speed
  unsigned long reference = millis() + t;
  int l = spd*dir; int r = spd*dir*(-1);
  run(l, r); while (millis() < reference){}
  run(-50*dir, 50*dir);delay(10);run(0,0);
  if(blink_flag){gr_blink(500);}
}

void dummy_offset(){
  run(160, 160);delay(300);run(0,0);delay(100);
}
