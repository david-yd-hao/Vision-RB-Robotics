#include <Adafruit_MotorShield.h>



long Ir = 0;
long lastIr = 0;
const double kp = 0.3, ki = 0.15, kd = 0.1;
int lastPr;
int output;
int Pr;

void rotpid(float error, Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor){
    Pr = error;
    Ir = lastIr + Pr;
    D = lastPr - Pr;
    output = kp*Pr + ki*Ir + kd*D;
    constrain(output, 0, 255);
    lastPr = Pr;
    lastIr = Ir;
    if(output > 0){
        LMotor->run(FORWARD);
        RMotor->run(BACKWARD);
    }else if(output < 0){
        LMotor->run(BACKWARD);
        RMotor->run(FORWARD);
    }
//    }else if(output <= 1 && output >= -1){
//        LMotor->setSpeed(0);
//        RMotor->setSpeed(0);
//        LMotor->run(RELEASE);
//        RMotor->run(RELEASE);
//        stop();
//        return
//    }

    LMotor->setSpeed(abs(output));
    RMotor->setSpeed(abs(output));

    Serial.println(P);
}
