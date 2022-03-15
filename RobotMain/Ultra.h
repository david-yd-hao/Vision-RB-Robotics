// Define Trig and Echo pin:
#define trigPin 13
#define echoPin 12
#define red_Sensor_Port 3
#define blue_Sensor_Port 2

// Define variables:
long duration;
float distance;


void ultraSetup() {
  // Define inputs and outputs:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float ultraDistance() {
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance:
  distance = duration * 0.034 / 2;

  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
//  Serial.print(String("ULTRASONIC Distance =   ") + distance + String("    cm"));
  delay(50);
  return distance;
}

bool ultraStop(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor, float target){
    float dist = ultraDistance();
    if(dist < target){
        LMotor->setSpeed(0);
        RMotor->setSpeed(0);
        LMotor->run(RELEASE);
        RMotor->run(RELEASE);
        return true;
    }else{
        return false;
    }
}

void colorSetup(){
    pinMode(red_Sensor_Port, INPUT);
    pinMode(blue_Sensor_Port, INPUT);
}


bool colorStop(Adafruit_DCMotor *LMotor, Adafruit_DCMotor *RMotor){
    int red = digitalRead(red_Sensor_Port);
    int blue = digitalRead(blue_Sensor_Port);
    Serial.println(String("red: ")+red+String("         blue: ")+blue);
    if(red == 1 || blue == 1){
        LMotor->setSpeed(0);
        RMotor->setSpeed(0);
        LMotor->run(RELEASE);
        RMotor->run(RELEASE);
        delay(1000);
        return true;
    }else{
        return false;
    }
}
