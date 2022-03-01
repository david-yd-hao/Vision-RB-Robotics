const int IN_A1 = 2;
const int PWM_A = 3;
const int IN_A2 = 4;
const int PWM_B = 5;
const int IN_B1 = 7;
const int IN_B2 = 8;

void setup() {
  pinMode(IN_A1, OUTPUT);
  pinMode(IN_A2, OUTPUT);
  pinMode(IN_B1, OUTPUT);
  pinMode(IN_B2, OUTPUT);
}
void moveLeft(void)
{
  digitalWrite(IN_A1, LOW);
  digitalWrite(IN_B1, HIGH);
  digitalWrite(IN_A2, LOW);
  digitalWrite(IN_B2, LOW);
  analogWrite(PWM_A, 255);
  analogWrite(PWM_B, 255);
}
void moveRight(void)
{
  digitalWrite(IN_A1, HIGH);
  digitalWrite(IN_B1, LOW);
  digitalWrite(IN_A2, LOW);
  digitalWrite(IN_B2, LOW);
  analogWrite(PWM_A, 255);
  analogWrite(PWM_B, 255);
}
void moveForward(void)
{
  digitalWrite(IN_A1, HIGH);
  digitalWrite(IN_B1, HIGH);
  digitalWrite(IN_A2, LOW);
  digitalWrite(IN_B2, LOW);
  analogWrite(PWM_A, 255);
  analogWrite(PWM_B, 255);
}
void moveBackward(void)
{
  digitalWrite(IN_A1, LOW);
  digitalWrite(IN_B1, LOW);
  digitalWrite(IN_A2, HIGH);
  digitalWrite(IN_B2, HIGH);
  analogWrite(PWM_A, 255);
  analogWrite(PWM_B, 255);
}
void stopCar(void)
{
  digitalWrite(IN_A1, LOW);
  digitalWrite(IN_B1, LOW);
  digitalWrite(IN_A2, LOW);
  digitalWrite(IN_B2, LOW);
  analogWrite(PWM_A, 255);
  analogWrite(PWM_B, 255);
}
void loop() {
  moveForward();
  delay(3000);
  moveLeft();
  delay(3000);
  moveRight();
  delay(3000);
  moveBackward();
  delay(3000);
  stopCar();
  delay(3000);
}