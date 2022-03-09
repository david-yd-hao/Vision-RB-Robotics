/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2
---->  http://www.adafruit.com/products/1438
*/

#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RMotor = AFMS.getMotor(4);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
}

void loop() {
  delay(10000);
  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  LMotor->setSpeed(150);
  RMotor->setSpeed(150);
  delay(10000);
  LMotor->setSpeed(50);
  RMotor->setSpeed(50);
  delay(5000);
  LMotor->setSpeed(0);
  RMotor->setSpeed(0);
  LMotor->run(RELEASE);
  RMotor->run(RELEASE);
  delay(5000);

}
