#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RMotor = AFMS.getMotor(2);

int setpoint = 100;
long I = 0;
long lastI = 0;
double kp = 0.3, ki = 0.15, kd = 0.1;
int lastP;
int input = 10;
int output;
int P;

void setup(){
    Serial.begin(9600);

    Serial.println("Adafruit Motorshield v2 - DC Motor test!");
    if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
        Serial.println("Could not find Motor Shield. Check wiring.");
        while (1);
    }
  Serial.println("Motor Shield found.");


  // Set the speed to start
  LMotor->setSpeed(LSpeed);
  RMotor->setSpeed(RSpeed);
  LMotor->run(FORWARD);
  LMotor->run(FORWARD);
  // turn on motor
  LMotor->run(RELEASE);
  RMotor->run(RELEASE);
}

void loop(){

    input = Serial.read()
    P = setpoint - input;
    I = lastI + P;
    D = lastP - P
    output = kp*P + ki*I + kd*D;
    constrain(output, 0, 255);
    lastP = P;
    lastI = I
    Left_Speed = output
    Right_Speed = -output
    if(output > 1){
        LMotor->run(FORWARD);
        RMotor->run(BACKWARD);
    }else if(output < -1){
        LMotor->run(BACKWARD);
        RMotor->run(FORWARD);
    }else if(output <= 1 && output >= -1){
        LMotor->setSpeed(0);
        RMotor->setSpeed(0);
        LMotor->run(RELEASE);
        RMotor->run(RELEASE);
        break
    }

    LMotor->setSpeed(output);
    RMotor->setSpeed(output);

    Serial.println(P);
}
