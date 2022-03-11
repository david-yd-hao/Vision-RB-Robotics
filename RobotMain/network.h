#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID
char pass[] = SECRET_PASS;    // your network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "10.254.223.22";
int        port     = 1883;
const char RobotX[] = "arduino/RobotX";
const char RobotY[] = "arduino/RobotY";
const char RobotRot[] = "arduino/RobotRot";
const char CubeX[] = "arduino/CubeX";
const char CubeY[] = "arduino/CubeY";
const char IsBlue[]  = "arduino/IsBlue";
const char Start[]  = "arduino/Start";

