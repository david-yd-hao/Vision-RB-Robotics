#include <Adafruit_MotorShield.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include "vision_drive.h"
#include "network.h"
#include "UltraColor.h""
#include "servo_control.h"
#include "angle.h"
#include "linesensor.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = AFMS.getMotor(3);
Adafruit_DCMotor *RMotor = AFMS.getMotor(4);
String str_cubeX, str_cubeY, str_robotX, str_robotY, str_robotRot, str_isBlue;
int cubeX, cubeY, robotX, robotY, robotRot;
int isBlue;
int main_output = 0;
bool toStop = false;
int red_or_blue = 0, blue_count = 0, red_count= 0;


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
  ultraSetup();
  colorSetup();
}



void loop(){
  
   delay(2000);
   openClaw();


    // GO GET THE CUBE FIRST (20pts)
    //initial rotation to corner
   while(toStop == false){
     mqttClient.poll();
     robotRot = str_robotRot.toInt();
     if(robotRot == 0){
       continue;
     }
     main_output, toStop = visionRotWithRight(RMotor, 2.5, robotRot, 272);
   }
  

  // goes to corner
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
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
    if(robotX == 0 || robotY == 0){
       continue;
     }
     main_output = visionLineFollowUltra(LMotor, RMotor, 8, spx1, spy1, 810, 732, robotX, robotY);
     toStop = ultraStop(LMotor, RMotor, 8);
   }
  
  // rotation at corner
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   while(toStop == false){
     mqttClient.poll();
     robotRot = str_robotRot.toInt();
     isBlue = str_isBlue.toInt();
     if(robotRot == 0){
       continue;
     }
     main_output, toStop = visionRotation(LMotor, RMotor, 1, 1.5, robotRot, 185);
   }
  
  // goes fowards to near cube position
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
   spx1 = robotX;
   spy1 = robotY;
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
     if(robotX == 0 || robotY == 0){
       continue;
     }
     main_output, toStop = visionLineFollow(LMotor, RMotor, 10, spx1, spy1, 223, 548, robotX, robotY, 160);
   }

   // Actively rotate to face cube
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
  
   while(true){
       mqttClient.poll();
       cubeX = str_cubeX.toInt();
       cubeY = str_cubeY.toInt();
       if(cubeX != 0 && cubeY != 0){
         cubeX = cubeX+2;
         cubeY = cubeY-2;
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
   while(toStop == false){
      mqttClient.poll();
      robotRot = str_robotRot.toInt();
      if(robotRot == 0){
       continue;
      }  
      main_output, toStop = visionRotation(LMotor, RMotor, 0.8, 1, robotRot, cube_angle);
   }

   delay(1000);
   toStop = false;

   while(true){
       mqttClient.poll();
       robotX = str_robotX.toInt();
       robotY = str_robotY.toInt();
       if(robotX != 0 && robotY != 0){
         break;  
       }
   }
   cube_angle = (int)getAngle(robotX, robotY, cubeX, cubeY);
   while(toStop == false){
      mqttClient.poll();
      robotRot = str_robotRot.toInt();
      if(robotRot == 0){
       continue;
      }  
      main_output, toStop = visionRotation(LMotor, RMotor, 1.2, 1, robotRot, cube_angle);
   }

   delay(1000);
    toStop = false;
   while(true){
       mqttClient.poll();
       robotX = str_robotX.toInt();
       robotY = str_robotY.toInt();
       if(robotX != 0 && robotY != 0){
         break;  
       }
   }
   cube_angle = (int)getAngle(robotX, robotY, cubeX, cubeY);
   while(toStop == false){
      mqttClient.poll();
      robotRot = str_robotRot.toInt();
      if(robotRot == 0){
       continue;
      }  
      main_output, toStop = visionRotation(LMotor, RMotor, 1.5, 1, robotRot, cube_angle);
   }

   delay(1000);



  int corner_x = 815, corner_y = 736, ramp_x = 563, ramp_y = 374, dest_x = 662, dest_y = 337, origin_x = 730, origin_y = 66;
   int edge_rot = 270, corner_rot = 360, origin_rot = 88;
   
  if(isBlue == 0){
   corner_x = 815, corner_y = 736, ramp_x = 565, ramp_y = 374, dest_x = 662, dest_y = 337, origin_x = 730, origin_y = 66;
   edge_rot = 270, corner_rot = 360, origin_rot = 88;
   redLED();
  }else if(isBlue == 1){
   corner_x = 94, corner_y = 50, ramp_x = 400, ramp_y = 225, dest_x = 448, dest_y = 139, origin_x = 730, origin_y = 66;
   edge_rot = 180, corner_rot = 88, origin_rot = 0;
   blueLED();
  }



   

   // goes to cube before touching
   toStop = false;
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
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
     if(robotX == 0 || robotY == 0){
       continue;
     }
     robotRot = str_robotRot.toInt();
    main_output, toStop = visionLineFollow(LMotor, RMotor, 6, spx1, spy1, cubeX, cubeY, robotX, robotY, 450);
   }
   
   // wait for the light indicator
   if(red_or_blue == 1){
     red_count = 1;
   }else if(red_or_blue == 2){
     blue_count = 1;
   }
   delay(3000);

  //   reach and pick up cube with S movement
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   LMotor->setSpeed(150);
   RMotor->setSpeed(150);
   delay(1000);
   LMotor->setSpeed(150);
   RMotor->setSpeed(80);
   delay(1000);
   LMotor->setSpeed(80);
   RMotor->setSpeed(150);
   delay(1000);
   LMotor->setSpeed(0);
   RMotor->setSpeed(0);
   closeClaw();

  //  goes back a bit to ensure the cube is safe
   LMotor->run(BACKWARD);
   RMotor->run(BACKWARD);
   LMotor->setSpeed(150);
   RMotor->setSpeed(150);
   delay(3000);
   LMotor->setSpeed(0);
   RMotor->setSpeed(0);







  // NOW PLACE THE CUBE (30 pts)
  // the parameter for the red cube



   // rotate to the bottom (or left for blue cube) edge
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   while(toStop == false){ 
     mqttClient.poll();
     robotRot = str_robotRot.toInt();
     if(robotRot == 0){
       continue;
     }
     main_output, toStop = visionRotation(LMotor, RMotor, 1, 1, robotRot, edge_rot);
   }


  // move to edge with ultrasound
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   while(toStop == false){
    LMotor->setSpeed(150);
    RMotor->setSpeed(150);
    toStop = ultraStop(LMotor, RMotor, 10);
   }

   // rotate to face corner (right bottom for red, and top left for blue)
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   while(toStop == false){ 
     mqttClient.poll();
     robotRot = str_robotRot.toInt();
     if(robotRot == 0){
       continue;
     }
     main_output, toStop = visionRotation(LMotor, RMotor, 1, 1, robotRot, corner_rot);
   }

   // move to corner with ultrasonic
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
   spx1 = robotX;
   spy1 = robotY;
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
    if(robotX == 0 || robotY == 0){
       continue;
     }
     main_output = visionLineFollowUltra(LMotor, RMotor, 8, spx1, spy1, corner_x, corner_y, robotX, robotY);
     toStop = ultraStop(LMotor, RMotor, 6);
   }

   // rotate at corner to face the starting point
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   while(toStop == false){ 
     mqttClient.poll();
     robotRot = str_robotRot.toInt();
     if(robotRot == 0){
       continue;
     }
     main_output, toStop = visionRotation(LMotor, RMotor, 1, 1, robotRot, origin_rot);
   }

   // goes forward a bit to avoid collision with bump
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   LMotor->setSpeed(140);
   RMotor->setSpeed(150);
   delay(6000);
   LMotor->setSpeed(0);
   RMotor->setSpeed(0);
   

   // rotate to face ramp
   toStop = false;
   while(true){
       mqttClient.poll();
       robotX = str_robotX.toInt();
       robotY = str_robotY.toInt();
       if(robotX != 0 && robotY != 0){
         break;  
       }
   }
   int ramp_angle = (int)getAngle(robotX, robotY, ramp_x, ramp_y);
  //  Serial.print(String("robotX:")+robotX+String("        robotY:")+robotY+String("       Angle:")+ramp_angle);
   while(toStop == false){
     mqttClient.poll();
     robotRot = str_robotRot.toInt();
     if(robotRot == 0){
       continue;
     }
     main_output, toStop = visionRotation(LMotor, RMotor, 0.8, 1, robotRot, ramp_angle);
   }
   delay(500);

   // moves to ramp with ultrasonic
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
   spx1 = robotX;
   spy1 = robotY;
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
    if(robotX == 0 || robotY == 0){
       continue;
     }
     main_output = visionLineFollowUltra(LMotor, RMotor, 8, spx1, spy1, ramp_x, ramp_y, robotX, robotY);
     toStop = ultraStop(LMotor, RMotor, 19);
   }
   delay(500);

   // actively rotate to face destination zone
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   toStop = false;
   int dest_angle = (int)getAngle(robotX, robotY, dest_x, dest_y);
   while(toStop == false){
      mqttClient.poll();
      robotRot = str_robotRot.toInt();
      if(robotRot == 0){
       continue;
      }  
      main_output, toStop = visionRotation(LMotor, RMotor, 0.7, 1, robotRot, dest_angle);
   }
   delay(1000);
   toStop = false;
   while(true){
       mqttClient.poll();
       robotX = str_robotX.toInt();
       robotY = str_robotY.toInt();
       if(robotX != 0 && robotY != 0){
         break;  
       }
   }
   dest_angle = (int)getAngle(robotX, robotY, dest_x, dest_y);
    while(toStop == false){
      mqttClient.poll();
      robotRot = str_robotRot.toInt();
      if(robotRot == 0){
       continue;
      }  
      main_output, toStop = visionRotation(LMotor, RMotor, 1, 1, robotRot, dest_angle);
   }
   delay(1000);
   toStop = false;
   while(true){
       mqttClient.poll();
       robotX = str_robotX.toInt();
       robotY = str_robotY.toInt();
       if(robotX != 0 && robotY != 0){
         break;  
       }
   }
   cube_angle = (int)getAngle(robotX, robotY, cubeX, cubeY);
   while(toStop == false){
      mqttClient.poll();
      robotRot = str_robotRot.toInt();
      if(robotRot == 0){
       continue;
      }  
      main_output, toStop = visionRotation(LMotor, RMotor, 1.3, 1, robotRot, dest_angle);
   }

   


    // goes to destination and DROP CUBE and reverse back
    delay(1000);
    LMotor->run(FORWARD);
    RMotor->run(FORWARD);
    toStop = false;
    LMotor->setSpeed(100);
    RMotor->setSpeed(100);
    while(toStop == false){
      toStop = lineSensorStop(LMotor, RMotor);
    }
    openClaw();
    delay(1000);
    LMotor->run(BACKWARD);
    RMotor->run(BACKWARD);
    LMotor->setSpeed(200);
    RMotor->setSpeed(200);
    delay(4000);
    LMotor->setSpeed(0);
    RMotor->setSpeed(0);
  



    // NOW GET BACK TO THE ORIGIN (20pts)
    // rotate to face the edge
    LMotor->run(FORWARD);
    RMotor->run(FORWARD);
    toStop = false;
    while(toStop == false){ 
      mqttClient.poll();
      robotRot = str_robotRot.toInt();
      if(robotRot == 0){
        continue;
      }
      main_output, toStop = visionRotation(LMotor, RMotor, 1, 1, robotRot, corner_rot);
    }

  // moves to edge with ultrasonic
   LMotor->run(FORWARD);
   RMotor->run(FORWARD);
   LMotor->setSpeed(150);
   RMotor->setSpeed(150);
   toStop = false;
   while(toStop == false){
     toStop = ultraStop(LMotor, RMotor, 12);
   }

   // rotate to face the starting zone
  LMotor->run(FORWARD);
  RMotor->run(FORWARD);
  toStop = false;
  while(toStop == false){ 
    mqttClient.poll();
    robotRot = str_robotRot.toInt();
    if(robotRot == 0){
      continue;
    }
    main_output, toStop = visionRotation(LMotor, RMotor, 1, 1, robotRot, origin_rot);
  }

  // moves to starting zone and stop with ultrasonic
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
   spx1 = robotX;
   spy1 = robotY;
   while(toStop == false){
     mqttClient.poll();
     robotX = str_robotX.toInt();
     robotY = str_robotY.toInt();
    if(robotX == 0 || robotY == 0){
       continue;
     }
     main_output = visionLineFollowUltra(LMotor, RMotor, 8, spx1, spy1, origin_x, origin_y, robotX, robotY);
     toStop = ultraStop(LMotor, RMotor, 10);
   }
   
  delay(5000);
  
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
    char a = (char)mqttClient.read();
    str_isBlue = a;
  }else if(mqttClient.messageTopic() == Start){
    char start = (char)mqttClient.read();
  }
}
  
