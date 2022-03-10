#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RMotor = AFMS.getMotor(2);


int sensorXL = A0;	// select the input pin for the potentiometer
int sensorL = A1;
int sensorR = A2;
int sensorXR = A3;

int sensorVal[4] = {0,0,0,0};  //set up sensor values values in calculation
int sensor[4] = {0,0,0,0}; 
int threshold = 200;
float Kp = 30, Ki = 10, Kd = 1;
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;
int LSpeed = 150; //set up initial speed
int RSpeed = 150;

void setup() {
  Serial.begin(9600);       	//Start serial and set the correct Baud Rate


  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
}


void loop() {
  sensorVal[0] = analogRead(sensorXL);
  sensorVal[1] = analogRead(sensorL);
  sensorVal[2] = analogRead(sensorR);
  sensorVal[3] = analogRead(sensorXR);


  for (int i = 0; i <= 3; i++) {
    if(sensorVal[i] < threshold){
      sensor[i] = 1;
    }
    else{
        sensor[i] = 0;
    }
  }
  

  if (sensor[0] == 0 && sensor[1] != 0 && sensor[2] != 0 && sensor[3] != 0) {
    error = 3;
    Serial.println("Turn Hard Left!!!!!");
  } else if (sensor[1] == 0 && sensor[0] != 0 && sensor[2] != 0 && sensor[3] != 0) {
    error = 1;
    Serial.println("Turn Left!!");
  } else if (sensor[2] == 0 && sensor[1] != 0 && sensor[2] != 0 && sensor[0] != 0) {
    error = -1;
    Serial.println("Turn Right!!");
  } else if (sensor[3] == 0 && sensor[1] != 0 && sensor[2] == 0 && sensor[0] != 0) {
    error = -3;
    Serial.println("Turn Hard Right!!!!!");
  } else if (sensor[1] == 0 && sensor[2] == 0) {
    error = 0;
    Serial.println("Stop Now!");
    LMotor->run(RELEASE);
    RMotor->run(RELEASE);
    delay(1000);
  } else {
    error = 0;
    Serial.println("continue");
  }


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
}
