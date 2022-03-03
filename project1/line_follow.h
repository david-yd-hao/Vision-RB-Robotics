const float LW = 180, LB = 100;
const float RW = 180, RB = 100; 
const float LM = 140, RM = 140;
const int LS_count = 2;
const int US_count = 4;


void fl(float speed_f, float speed_b){
  // basic follow line
  float L = LS_L(), R = LS_R();
  if (L > LW){
    run(speed_b, speed_f);
  }
  else if (R > RW){
    run(speed_f, speed_b);
  }
  else if (R < RW && R < RW){
    run(speed_f, speed_f);
  }
}

void fl_pid(float max_speed, float min_speed, float k){
  // basic follow line with pid
  float L = LS_L(), R = LS_R();
  float delta = L - R; 
  float l_speed = 0, r_speed = 0;
  l_speed = -k * delta + max_speed;
  if (l_speed < min_speed){l_speed = min_speed;}
  r_speed = +k * delta + max_speed;
  if (r_speed < min_speed){r_speed = min_speed;}
  run(l_speed, r_speed);
}

void fl_pid_stop(int max_speed, int min_speed, float k){
  // follow line pid and stop at cross
  float L = LS_L(), R = LS_R();
  while (L<LW || R<RW) {
    L = LS_L();
    R = LS_R();
    fl_pid(max_speed, min_speed, k);
  }
  run(0,0);
}

void fl_stop(int max_speed, int min_speed ){
  // follow line and stop at cross
  float L = LS_L(), R = LS_R();
  while (L<LW || R<RW) {
    L = LS_L();
    R = LS_R();
    fl(max_speed, min_speed);
  }
  run(0,0);
}


void fl_cross(int max_speed, int min_speed, int lines, int traverse_time, bool blink_trigger){
  // follow line with given speed and cross given number of lines, each with given traverse time. 
  float L = LS_L(), R = LS_R();
  int lines_passed = 0;
  int counted = 0;
  while (lines_passed < lines) {
    if (L>LM && R>RM){counted += 1;} else{counted = 0;}
    if (counted > LS_count){
      counted = 0;
      if (blink_trigger) {run(0,0);red_blink(1000);}
      run(max_speed, max_speed); while(LS_L()>LW && LS_R()>RW){}
      delay(traverse_time);
      run(0,0);
      if (blink_trigger) {red_blink(1000);}
      lines_passed += 1;
    }
    else{fl(max_speed, min_speed);}
    L = LS_L();
    R = LS_R();
  }
  run(0,0);
}


void fl_pid_cross(int max_speed, int min_speed, float k, int lines, int traverse_time, bool blink_trigger){
  // follow line pid with given speed and cross given number of lines, each with given traverse time. 
  float L = LS_L(), R = LS_R();
  int lines_passed = 0;
  while (lines_passed < lines) {
    if (L>LW && R>RW){
      if (blink_trigger) {run(0,0);red_blink(1000);}
      run(max_speed, max_speed); 
      delay(traverse_time);
      run(0,0);
      if (blink_trigger) {red_blink(1000);}
      lines_passed += 1;
    }
    else{fl_pid(max_speed, min_speed, k);}
    L = LS_L();
    R = LS_R();
  }
}

void cross_white(int spd, int t, int line_num, bool blink_trigger){
  // cross given number of white lines with speed and traverse time
  float L = LS_L(), R = LS_R();
  int count = 0;
  for (int line_crossed = 0; line_crossed<line_num; line_crossed++){
    count = 0;
    while (count < LS_count){
      run(spd, spd);
      L = LS_L();
      R = LS_R();
      if (L > LW || R>RW){count += 1;}else{count = 0;}
    }
    if(blink_trigger){run (0,0); red_blink(500);}
    count = 0;
    while (count < LS_count){
      run(spd, spd);
      L = LS_L();
      R = LS_R();
      if (L < LB || R<RB){count += 1;}else{count = 0;}
    }
    if(blink_trigger){run (0,0); red_blink(500);}
    run(spd, spd);
    delay(t);
    run (0,0);
    if(blink_trigger){ red_blink(500);}
  }
}

void fl_time(int max_speed, int min_speed, unsigned long t, bool blink_trigger){
  // follow line for a given time
  //elapsed_time in ms
  float L = LS_L(), R = LS_R();
  unsigned long t_end = millis() + t;
  while (millis() < t_end) {
    if (L>LW && R>RW){
      run(max_speed, max_speed); 
    }
    else{fl(max_speed, min_speed);}
    L = LS_L();
    R = LS_R();
  }
  run(0,0);
  if(blink_trigger){red_blink(500);}
}

void fl_US(int max_speed, int min_speed, long cm, bool blink_trigger){
  // follow line until the ultrasonic sensor senses within a given distance
  float L = LS_L(), R = LS_R();
  long sensor_value = US();
  int count = 0;
  while (count<US_count) {
    if (L>LW && R>RW){
      run(max_speed, max_speed); 
    }
    else{fl(max_speed, min_speed);}
    L = LS_L();
    R = LS_R();
    sensor_value = US();
    if (sensor_value != 0 && sensor_value < cm){count+=1;}
  }
  run(0,0);
  if(blink_trigger){red_blink(500);}
}

void fl_IRd(int max_speed, int min_speed,bool blink_trigger){
  // follow line until the IR distance sensor senses annything (a dummy)
  float L = LS_L(), R = LS_R();
  int sensor_value = IRd_b();
  int count = 0;
  while (count< IRd_count) {
    if (L>LW && R>RW){
      run(max_speed, max_speed); 
    }
    else{fl(max_speed, min_speed);}
    L = LS_L();
    R = LS_R();
    sensor_value = IRd_b();
    if (sensor_value == 1){count+=1;}
  }
  run(0,0);
  if(blink_trigger){red_blink(500);}
}

void turn_to_line_r(int Max_speed_time, bool blink_flag){
  // turn right with a max_speed for a certain amount of time and then to white line
  run(255, -255);delay(Max_speed_time);
  if (blink_flag == true){run(0,0);gr_blink(1000);}
  float R = LS_R();
  while (R < RB){
    R = LS_R();
    run(200, -200);
  }
  delay(50);
  while (R > RW){
    R = LS_R();
    run(150, -150);
  }delay(30);
  run(0,0);delay(300);
}

void turn_to_line_l(int Max_speed_time, bool blink_flag){
  // turn left with a max_speed for a certain amount of time and then to white line
  run(-255, 255); delay(Max_speed_time);
  if (blink_flag == true){run(0,0);gr_blink(1000);}
  float L = LS_L();
  while (L < LB){
    L = LS_L();
    run(-200, 200);
  }
  delay(50);
  while (L > LW){
    L = LS_L();
    run(-150, 150);
  }delay(30);
  run(0,0);delay(300);
}
