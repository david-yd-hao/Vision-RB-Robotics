int sensorPin0 = A0;	// select the input pin for the potentiometer
int sensorPin1 = A1;
int sensorValue0 = 0;  // variable to store the value coming from the sensor
int sensorValue1 = 0;
void setup() {
  Serial.begin(9600);       	//Start serial and set the correct Baud Rate
}
void loop() {
  sensorValue0 = analogRead(sensorPin0);
  sensorValue1 = analogRead(sensorPin1);
  Serial.print("Left = ");
  Serial.print(sensorValue0);
  Serial.print("      	Right = ");
  Serial.println(sensorValue1);
}
