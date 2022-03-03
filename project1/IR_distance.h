const int IRd_f_port = A0, IRd_b_port = A3;
const int IRd_count = 3;

int find_max(int arr[])
{
  // find max of an int array
    int n = sizeof(arr) / sizeof(arr[0]);
    int maximum = arr[0];
    for (int j = 0; j < n; ++j) {
        if (arr[j] > maximum) {
            maximum = arr[j];
        }
    }
    return maximum;
}

int find_min(int arr[])
{
  // find min of an int array
    int n = sizeof(arr) / sizeof(arr[0]);
    int minimum = arr[0];
    for (int j = 0; j < n; ++j) {
        if (arr[j] < minimum) {
            minimum = arr[j];
        }
    }
    return minimum;
}

int result[6];


int IRd_f_avr(){
  // find the average of 6 readings from front IR distance sensor at the front
  int sensor_value;
  //Serial.println("Here are the raw data.");
  for (int j = 0; j<6; j++){
    sensor_value = analogRead(IRd_f_port);
    //Serial.print(sensor_value);Serial.print(" ");
    result[j] = sensor_value;
  }
  //Serial.println(" ");
  //Serial.println("Here are the max min average");
  /*int maximum = find_max(result);
  Serial.print(maximum);Serial.print(" ");
  int minimum = find_min(result);
  Serial.print(minimum);Serial.print(" ");
  int sum = -maximum - minimum;*/
  int sum = 0;
  for (int j = 0; j<6; j++){
    sum += result[j];
  }
  sum /= 6;
  //Serial.println(sum);
  return sum;
}

const int IRd_f_reference = 300;
int IRd_f(){
  // return 0 or 1 (undetected or detected) for front IR distance sensor
  if (IRd_f_avr() > IRd_f_reference) {green_LED(1);return 1;}
  else {green_LED(0);return 0;}
}

const int IRd_b_reference = 300;
int IRd_b_avr(){
  // find the average of 6 readings from back IR distance sensor
  int sensor_value;
  //Serial.println("Here are the raw data.");
  for (int j = 0; j<6; j++){
    sensor_value = analogRead(IRd_b_port);
    //Serial.print(sensor_value);Serial.print(" ");
    result[j] = sensor_value;
  }
  //Serial.println(" ");
  //Serial.println("Here are the max min average");
  /*int maximum = find_max(result);
  Serial.print(maximum);Serial.print(" ");
  int minimum = find_min(result);
  Serial.print(minimum);Serial.print(" ");
  int sum = -maximum - minimum;*/
  int sum = 0;
  for (int j = 0; j<6; j++){
    sum += result[j];
  }
  sum /= 6;
  //Serial.println(sum);
  return sum;
}

int IRd_b(){
  // return 0 or 1 (undetected or detected) for the back IR distance sensor
  if (IRd_b_avr() > IRd_b_reference) {return 1;}
  else { return 0;}
}

int IRd_approach_dummy(){
  run(120, 120);
  int count = 0;
  while(count < IRd_count){if(IRd_b()==1){count += 1;}else{count = 0;}}
  run(0,0);delay(100);
}
