#include <Adafruit_MotorShield.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include "vision_drive.h"
#include "network.h"
#include "stop.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RMotor = AFMS.getMotor(2);
int isBluevar = 0, startvar = 0;
float RobotRotvar = 0;

void setup() {
  Serial.begin(38400);         //Start serial and set the correct Baud Rate
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

  Serial.print("Subscribing to topic: ");
  Serial.println(cube_error);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(cube_error);
  mqttClient.subscribe(cross_error);
  mqttClient.subscribe(dest1_error);
  mqttClient.subscribe(dest2_error);
  mqttClient.subscribe(red1_error);
  mqttClient.subscribe(red2_error);
  mqttClient.subscribe(blue1_error);
  mqttClient.subscribe(blue2_error);
  mqttClient.subscribe(isBlue);
  mqttClient.subscribe(start);
  mqttClient.subscribe(RobotRot);
  
  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

}

void onMqttMessage(int messageSize) {

  if(mqttClient.messageTopic() == isBlue){
    isBluevar = (int)mqttClient.read();
  }
  if(mqttClient.messageTopic() == start){
    startvar = (int)mqttClient.read();
  }
  if(mqttClient.messageTopic() == RobotRot){
    RobotRotvar = (float)mqttClient.read();
  }

}

void loop(){
  mqttClient.poll();
  Serial.println((char)RobotRotvar);
}




  
