#include <Adafruit_MotorShield.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include "vision_drive.h"
#include "network.h"
#include "stop.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RMotor = AFMS.getMotor(2);
char isBlue = 0, start = 0;
int cubeX = 0, cubeY = 0, robotX = 0, robotY = 0, robotRot = 0, 
String rob;

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

}



void loop(){
  mqttClient.poll();
}


void onMqttMessage(int messageSize) {
  if(mqttClient.messageTopic() == RobotX){
    rob = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      rob = rob + a;
    }
    robotX = rob.toInt();
    Serial.println(cubeX);
  }else if(mqttClient.messageTopic() == RobotY){
    rob = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      rob = rob + a;
    }
    robotY = rob.toInt();
    Serial.println(rob);
  }else if(mqttClient.messageTopic() == RobotRot){
    rob = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      rob = rob + a;
    }
    robotRot = rob.toInt();
    Serial.println(rob);
  }else if(mqttClient.messageTopic() == CubeX){
    rob = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      rob = rob + a;
    }
    cubeX = rob.toInt();
    Serial.println(rob);
  }else if(mqttClient.messageTopic() == CubeY){
    rob = String("");
    while(mqttClient.available()){
      char a = (char)mqttClient.read();
      rob = rob + a;
    }
    cubeY = rob.toInt();
    Serial.println(rob);
  }else if(mqttClient.messageTopic() == IsBlue){
    isBlue = (char)mqttClient.read();
  }else if(mqttClient.messageTopic() == Start){
    start = (char)mqttClient.read();
  }
}
  
