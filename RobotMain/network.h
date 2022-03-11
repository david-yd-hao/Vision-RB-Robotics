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
const char cube_error[]  = "arduino/cube_error";
const char cross_error[]  = "arduino/cross_error";
const char dest1_error[]  = "arduino/dest1_error";
const char dest2_error[]  = "arduino/dest2_error";
const char red1_error[]  = "arduino/red1_error";
const char red2_error[]  = "arduino/red2_error";
const char blue1_error[]  = "arduino/blue1_error";
const char blue2_error[]  = "arduino/blue2_error";
const char isBlue[]  = "arduino/isBlue";
const char start[]  = "arduino/start";
const char RobotRot[] = "arduino/RobotRot";

//mqttClient.poll();
