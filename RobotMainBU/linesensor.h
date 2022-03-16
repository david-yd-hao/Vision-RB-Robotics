int sensorPin0 = A0;  // select the input pin for the potentiometer
int sensorPin1 = A1;
int sensorValue0 = 0;  // variable to store the value coming from the sensor
int sensorValue1 = 0;


bool lineSensorStop(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor){
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  Serial.print("Left = ");
  Serial.print(sensorValue0);
  Serial.print("        Right = ");
  Serial.println(sensorValue1);
  if(sensorValue0 < 400 && sensorValue1 < 400){
      LMotor->setSpeed(0);
      RMotor->setSpeed(0);
      LMotor->run(RELEASE);
      RMotor->run(RELEASE);
      return true;
  }else{return false;}
}
