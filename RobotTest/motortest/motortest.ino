/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2
---->  http://www.adafruit.com/products/1438
*/

#include <Adafruit_MotorShield.h>
#define Blue_color_in 2
#define Red_color_in 3
#define Red_LED_out 6
#define Blue_LED_out 5

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RMotor = AFMS.getMotor(2);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  pinMode(Blue_color_in, INPUT);
  pinMode(Red_color_in, INPUT);
  pinMode(Red_LED_out, OUTPUT);
  pinMode(Blue_LED_out, OUTPUT);

}

void loop() {

  delay(10000);
  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  LMotor->setSpeed(0);
  RMotor->setSpeed(150);
  delay(10000);
//  LMotor->setSpeed(50);
//  RMotor->setSpeed(50);
//  while(digitalRead(Blue_color_in) == 0 && digitalRead(Red_color_in) == 0){
//    continue;
//  }
//  LMotor->setSpeed(0);
//  RMotor->setSpeed(0);
//  LMotor->run(RELEASE);
//  RMotor->run(RELEASE);
//  delay(10000);
//  exit(0);

  exit(0);
}
