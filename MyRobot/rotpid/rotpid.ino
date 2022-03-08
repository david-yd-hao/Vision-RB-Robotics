int setpoint = 100;
long sumerror;
double kp = 0.3, ki = 0.15, kd = 0.1;
int lasterror;
int input = 10;
int output;
int nowerror;

void setup(){
    Serial.begin(9600)
}

void loop(){
    nowerror = setpoint - input;
    sumerror += nowerror;
    output = kp*nowerror + ki*sumerror + kd*(lasterror-nowerror);
    lasterror = nowerror;
    input += output;
    Serial.println(input);
    delay(50);

}