#include <Adafruit_MotorShield.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include "vision_drive.h"
#include "network.h"
#include "stop.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = AFMS.getMotor(3);
Adafruit_DCMotor *RMotor = AFMS.getMotor(4);
char isBlue, start;
String str_cubeX, str_cubeY, str_robotX, str_robotY, str_robotRot;
int cubeX, cubeY, robotX, robotY, robotRot;
String rob, str;
float main_output=0, main_previous_error=0, main_previous_I=0;
bool toStop = false;


void setup() {
  Serial.begin(9600);         //Start serial and set the correct Baud Rate
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
 
  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  // subscribe to a topic
  mqttClient.subscribe(RobotX);
  mqttClient.subscribe(RobotY);
  mqttClient.subscribe(RobotRot);
  mqttClient.subscribe(CubeX);
  mqttClient.subscribe(CubeY);
  mqttClient.subscribe(IsBlue);
  mqttClient.subscribe(Start);


  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  attach_servo();
}



void loop(){
  
  delay(1000)

  openClaw();
  while(toStop == false){
      mqttClient.poll();
      robotX, robotY, robotRot, cubeX, cubeY = manyInts(str_robotX, str_robotY, str_robotRot, str_cubeX, str_cubeY);
      main_output, main_previous_error, main_previous_I, toStop = visionRotWithRight(RMotor, robotRot, 275, main_previous_error, main_previous_I);
  }
  
  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  toStop = false;
  
  while(toStop == false){
    
      mqttClient.poll();
      robotX, robotY, robotRot, cubeX, cubeY = manyInts(str_robotX, str_robotY, str_robotRot, str_cubeX, str_cubeY);
      main_output, main_previous_error, main_previous_I, toStop = visionLineFollow(LMotor, RMotor, 776, 86, 816, 732, robotX, robotY, main_previous_error, main_previous_I);
  }
  
  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  toStop = false;
  
  while(toStop == false){
      mqttClient.poll();
      robotX, robotY, robotRot, cubeX, cubeY = manyInts(str_robotX, str_robotY, str_robotRot, str_cubeX, str_cubeY);
      main_output, main_previous_error, main_previous_I, toStop = visionRotation(LMotor, RMotor, 1.5, robotRot, 180, main_previous_error, main_previous_I);
  }
  
  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  toStop = false;
  
  while(toStop == false){
      LMotor->setSpeed(255);
      RMotor->setSpeed(255);
  }
  
  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  toStop = false;

  
}

void onMqttMessage(int messageSize) {
  if(mqttClient.messageTopic() == RobotX){
    str_robotX = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      str_robotX = str_robotX + a;
    }
  }else if(mqttClient.messageTopic() == RobotY){
    str_robotY = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      str_robotY = str_robotY + a;
    }
  }else if(mqttClient.messageTopic() == RobotRot){
    str_robotRot = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      str_robotRot = str_robotRot + a;
    }
  }else if(mqttClient.messageTopic() == CubeX){
    str_cubeX = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      str_cubeX = str_cubeX + a;
    }
  }else if(mqttClient.messageTopic() == CubeY){
    str_cubeY = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      str_cubeY = str_cubeY + a;
    }
  }else if(mqttClient.messageTopic() == IsBlue){
    isBlue = (char)mqttClient.read();
  }else if(mqttClient.messageTopic() == Start){
    start = (char)mqttClient.read();
  }
}
  
