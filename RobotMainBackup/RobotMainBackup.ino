#include <Adafruit_MotorShield.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include "vision_drive.h"
#include "network.h"
#include "Ultra.h"
#include "servo_control.h"
#include "angle.h"
#include "linesensor.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = AFMS.getMotor(3);
Adafruit_DCMotor *RMotor = AFMS.getMotor(4);
char isBlue, start;
String str_cubeX, str_cubeY, str_robotX, str_robotY, str_robotRot;
int cubeX, cubeY, robotX, robotY, robotRot;
String rob, str;
int main_output=0, main_previous_I=0;
bool toStop = false;
int blue_count = 0; int red_count= 0;


void setup() {
  Serial.begin(9600);         //Start serial and set the correct Baud Rate
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // attempt to connect to Wifi network:
//  Serial.print("Attempting to connect to SSID: ");
//  Serial.println(ssid);
//  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
//    // failed, retry
//    Serial.print(".");
//    delay(5000);
//  }
// 
//  Serial.println("You're connected to the network");
//  Serial.println();
//
//  Serial.print("Attempting to connect to the MQTT broker: ");
//  Serial.println(broker);
//
//  if (!mqttClient.connect(broker, port)) {
//    Serial.print("MQTT connection failed! Error code = ");
//    Serial.println(mqttClient.connectError());
//
//    while (1);
//  }
//
//  Serial.println("You're connected to the MQTT broker!");
//  Serial.println();
//
//  // set the message receive callback
//  mqttClient.onMessage(onMqttMessage);
//
//  // subscribe to a topic
//  mqttClient.subscribe(RobotX);
//  mqttClient.subscribe(RobotY);
//  mqttClient.subscribe(RobotRot);
//  mqttClient.subscribe(CubeX);
//  mqttClient.subscribe(CubeY);
//  mqttClient.subscribe(IsBlue);
//  mqttClient.subscribe(Start);


  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  attach_servo();
  ultraSetup();
  colorSetup();
}



void loop(){
  
   delay(1000);

   openClaw();
   // initial rotation
   float main_previous_error = 0;
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
     robotRot = str_robotRot.toInt();
     main_output, main_previous_error, main_previous_I, toStop = visionRotWithRight(RMotor, robotRot, 270, main_previous_error, main_previous_I);
     Serial.println(String("main previous error    ")+main_previous_error);
   }
  
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   while(true){
       mqttClient.poll();
       robotX = str_robotX.toInt();
       robotY = str_robotY.toInt();
       if(robotX != 0 && robotY != 0){
         break;  
       }
   }
   int spx1 = robotX;
   int spy1 = robotY;
   main_previous_error = 0;
   // goes to side
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
     robotRot = str_robotRot.toInt();
     main_output, main_previous_error, main_previous_I = visionLineFollowUltra(LMotor, RMotor, spx1, spy1, 816, 732, robotX, robotY, main_previous_error, main_previous_I);
     toStop = ultraStop(LMotor, RMotor, 10);
   }
  
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   main_previous_error = 0;
   // rotation at side
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
     robotRot = str_robotRot.toInt();
     main_output, main_previous_error, main_previous_I, toStop = visionRotation(LMotor, RMotor, 1.5, robotRot, 180, main_previous_error, main_previous_I);
   }
  
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;

   main_previous_error = 0;
   // goes fowards a bit
     while(true){
       mqttClient.poll();
       robotX = str_robotX.toInt();
       robotY = str_robotY.toInt();
       if(robotX != 0 && robotY != 0){
         break;  
       }
   }
   spx1 = robotX;
   spy1 = robotY;
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
     robotRot = str_robotRot.toInt();
     main_output, main_previous_error, main_previous_I, toStop = visionLineFollow(LMotor, RMotor, spx1, spy1, 270, 700, robotX, robotY, main_previous_error, main_previous_I);
   }


   delay(1000);
  
   // rotate to cube
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   main_previous_error = 0;
  
   while(true){
       mqttClient.poll();
       cubeX = str_cubeX.toInt();
       cubeY = str_cubeY.toInt();
       if(cubeX != 0 && cubeY != 0){
         cubeX = cubeX +2;
         cubeY = cubeY -2;
         break;  
       }
   }
   while(true){
       mqttClient.poll();
       robotX = str_robotX.toInt();
       robotY = str_robotY.toInt();
       if(robotX != 0 && robotY != 0){
         break;  
       }
   }
   int cube_angle = (int)getAngle(robotX, robotY, cubeX, cubeY);
   Serial.print(String("cubeX:")+cubeX+String("        cubeY:")+cubeY+String("       Angle:")+cube_angle);
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
     robotRot = str_robotRot.toInt();
     main_output, main_previous_error, main_previous_I, toStop = visionRotation(LMotor, RMotor, 1, robotRot, cube_angle, main_previous_error, main_previous_I);
   }

   delay(1000);
   Serial.println(String("delayed"));

   // goes to cube before touching
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   while(true){
       mqttClient.poll();
       robotX = str_robotX.toInt();
       robotY = str_robotY.toInt();
       if(robotX != 0 && robotY != 0){
         break;  
       }
   }
   spx1 = robotX;
   spy1 = robotY;
   toStop = false;
   main_previous_error = 0;
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
     robotRot = str_robotRot.toInt();
     main_output, main_previous_error, main_previous_I, toStop = visionLineFollow(LMotor, RMotor, spx1, spy1, cubeX, cubeY, robotX, robotY, main_previous_error, main_previous_I);
 //    toStop = colorStop(LMotor, RMotor);    
   }

   delay(1000);

//   goes to cube
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   main_previous_error = 0;
   LMotor->setSpeed(140);
   RMotor->setSpeed(70);
   delay(1000);
   closeClaw();
   LMotor->run(BACKWARD);
   RMotor->run(BACKWARD);
   LMotor->setSpeed(150);
   RMotor->setSpeed(150);
   delay(1000);
   LMotor->setSpeed(0);
   RMotor->setSpeed(0);
   LMotor->run(RELEASE);
   RMotor->run(RELEASE);
   delay(10000);
  openClaw();
  closeClaw();

  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  LMotor->setSpeed(100);
  RMotor->setSpeed(100);
  toStop = false;
  while(toStop == false){
    toStop = lineSensorStop(LMotor, RMotor);
  }

  openClaw();
  LMotor->run(BACKWARD);
  RMotor->run(BACKWARD);
  LMotor->setSpeed(200);
  RMotor->setSpeed(200);
  delay(1000);
  LMotor->setSpeed(0);
  RMotor->setSpeed(0);
  LMotor->run(RELEASE);
  RMotor->run(RELEASE);
  
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
  
