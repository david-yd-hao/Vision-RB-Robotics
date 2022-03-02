int sensorPin0 = A0;	// select the input pin for the potentiometer
int sensorPin1 = A1;
int LineSrLeft = 0;  // variable to store the value coming from the sensor
int LineSrRigt = 0;


void setup() {
  Serial.begin(9600);       	//Start serial and set the correct Baud Rate
}


void loop() {
  LineSrLeft = analogRead(sensorPin0);
  LineSrRigt = analogRead(sensorPin1);
  if (LineSrLeft < 200 & LineSrRigt > 200){
	Serial.println("Turn right");
	// insert motor commands
  }
  if (LineSrRigt < 200 & LineSrLeft > 200){
	Serial.println("Turn left");
	// insert motor commands
  }
  if (LineSrRigt < 200 & LineSrLeft < 200){
	Serial.println("Stop");
	// insert motor commands
  }
  if (LineSrRigt > 200 & LineSrLeft > 200){
	Serial.println("Continue");
	// insert motor commands
  }
}
