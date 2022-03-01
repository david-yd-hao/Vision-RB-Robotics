const int IN_A1 = 2;
const int PWM_A = 3;
const int IN_A2 = 4;
const int PWM_B = 5;
const int IN_B1 = 7;
const int IN_B2 = 8;
// motor control


const int sensor1 = 9;
const int sensor2 = 10;
const int sensor3 = 11;
const int sensor4 = 12;
const int sensor5 = 13;
// line following sensor

int sensor[5] = {0, 0, 0, 0, 0};
float Kp = 30, Ki = 10, Kd = 1;
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;
int DIN_A1 = 100;
int DIN_B1 = 100;
void setup() {
  Serial.begin(9600);
  pinMode(IN_A1, OUTPUT);
  pinMode(IN_A2, OUTPUT);
  pinMode(IN_B1, OUTPUT);
  pinMode(IN_B2, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
}
void loop() {
  sensor[0] = digitalRead(sensor1);
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);
  if (sensor[0] == 0 && sensor[1] != 0 && sensor[2] != 0 && sensor[3] != 0 && sensor[4] != 0) {
    error = -2;
  } else if (sensor[0] == 0 && sensor[1] == 0 && sensor[2] != 0 && sensor[3] != 0 && sensor[4] != 0) {
    error = -2;
  } else if (sensor[1] == 0 && sensor[0] != 0 && sensor[2] != 0 && sensor[3] != 0 && sensor[4] != 0) {
    error = -1;
  } else if (sensor[1] == 0 && sensor[0] != 0 && sensor[2] == 0 && sensor[3] != 0 && sensor[4] != 0) {
    error = -1;
  } else if (sensor[2] == 0 && sensor[1] != 0 && sensor[0] != 0 && sensor[3] != 0 && sensor[4] != 0) {
    error = 0;
  } else if (sensor[2] == 0 && sensor[1] == 0 && sensor[0] != 0 && sensor[3] == 0 && sensor[4] != 0) {
    error = 0;
  } else if (sensor[3] == 0 && sensor[1] != 0 && sensor[2] != 0 && sensor[0] != 0 && sensor[4] != 0) {
    error = 1;
  } else if (sensor[3] == 0 && sensor[1] != 0 && sensor[2] == 0 && sensor[0] != 0 && sensor[4] != 0) {
    error = 1;
  } else if (sensor[4] == 0 && sensor[1] != 0 && sensor[2] != 0 && sensor[3] == 0 && sensor[0] != 0) {
    error = 2;
  } else if (sensor[4] == 0 && sensor[1] != 0 && sensor[2] != 0 && sensor[3] != 0 && sensor[0] != 0) {
    error = 2;
  } else {
    error = 0;
  }
  P = error;
  I = I + previous_I;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_I = I;
  previous_error = error;
  int left_motor_speed = DIN_A1 - PID_value;
  int right_motor_speed = DIN_B1 + PID_value;
  constrain(left_motor_speed, 0, 255);
  constrain(right_motor_speed, 0, 255);
  digitalWrite(IN_A2, HIGH);
  digitalWrite(IN_B2, HIGH);
  digitalWrite(IN_A1, LOW);
  digitalWrite(IN_B1, LOW);
  analogWrite(PWM_A, DIN_A1 - PID_value);
  analogWrite(PWM_B, DIN_B1 + PID_value);
}