  const int White=1; // block color Light: Green and Red
const int Red=2; // Red
const int Blue=3; // Green

const int zigzag=1;
const int sqr=2;
const int mixed=3;

const float high_reference = 0.0731;
const float low_reference=0.0364;
const float offset=0.8;

int diff_blink_t = 5000;

const int IR_port_TSOP_peak = 5, IR_port_diff_peak = 3;
const int IR_port_TSOP = 6, QSD_port = 5;

float IR_counter(unsigned long sample_time){
  // return the ratio between 0 and total number of data from TSOP sensor
  float total = 0;
  float zeros = 0;
  bool result = true;
  unsigned long t = millis()+sample_time;
  while (millis() < t)
  {
    result = digitalRead(IR_port_TSOP);
    total += 1;
    if (result == false){zeros += 1;}
  }
  float return_value = zeros/total;
  return return_value;
}

float QSD_counter(unsigned long sample_time){
  // return the ratio between 0 and total number of data from QSD sensor
  float total = 0;
  float zeros = 0;
  bool result = true;
  unsigned long t = millis()+sample_time;
  while (millis() < t)
  {
    result = digitalRead(QSD_port);
    total += 1;
    if (result == false){zeros += 1;}
  }
  float return_value = zeros/total;
  return return_value;
}


int IR_differentiate(unsigned long sample_time){
  // return the dummy type judging from TSOP sensor (comparison with reference ratios
  float ratio = IR_counter(sample_time);
  if (ratio > (high_reference * offset)){return zigzag;}
  else if (ratio > (low_reference * offset)){ return mixed;}
  else {return sqr;}
}


int IR_differentiate_blink()
{
    // return the dummy type judging from TSOP sensor (comparison with reference ratios) and do the correct blinking
  int dummy = IR_differentiate(2000);
  digitalWrite(yellow_LED_port, 0);digitalWrite(red_LED_port, 0);digitalWrite(green_LED_port, 0);
  if (dummy == White){gr_blink(diff_blink_t);}
  else if (dummy == Red) {red_blink(diff_blink_t);}
  else if (dummy == Blue) {green_blink(diff_blink_t);} 
  return dummy;
}

int IR_search()
{
  // return 0 or 1 for finding a dummy or not sing TSOP
  float ratio = IR_counter(50);
  if (ratio > (low_reference * offset)){green_LED(1); return 1;}
  else {green_LED(0);return 0;}
}


float QSD_reference = 0.001;
int QSD_search()
{  
  // return 0 or 1 for finding a dummy or not sing QSD
  float ratio = QSD_counter(50);
  if (ratio > QSD_reference){green_LED(1); return 1;}
  else {green_LED(0);return 0;}
}

void QSD_search_r(unsigned long QSD_search_r_offset)
{
  // the sweep algorithm using QSD as a radar
  int value = QSD_search();
  if(value == 1){
    while (value == 1){
      run(-100, 100);
      value = QSD_search();
    }
    delay(50);
    run(0,0); 
    value = QSD_search();
    delay(120);
  }
  run(100, -100);
  while(value == 0){value = QSD_search();}
  unsigned long start_time = millis();
  delay(50);
  while(value == 1){value = QSD_search();}
  unsigned long end_time = millis();
  delay(150);run(0,0);delay(200);
  unsigned long rotate_time = (end_time - start_time)/2 + QSD_search_r_offset;
  value = QSD_search();
  run(-100, 100);while(value == 0){value = QSD_search();}
  unsigned long reference_time = millis() + rotate_time;
  while (millis() < reference_time){}
  run(0,0);delay(200); 
}



void IR_search_r(unsigned long IR_search_r_offset)
// the sweep algorithm using TSOP as a radar
{

  int value = IR_search();
  if(value == 1){
    while (value == 1){
      run(-100, 100);
      value = IR_search();
    }
    delay(50);
    run(0,0); 
    value = IR_search();
    delay(120);
  }
  run(100, -100);
  while(value == 0){value = IR_search();}
  unsigned long start_time = millis();
  delay(50);
  while(value == 1){value = IR_search();}
  unsigned long end_time = millis();
  delay(150);run(0,0);delay(200);
  unsigned long rotate_time = (end_time - start_time)/2 + IR_search_r_offset;
  value = IR_search();
  run(-100, 100);while(value == 0){value = IR_search();}
  unsigned long reference_time = millis() + rotate_time;
  while (millis() < reference_time){}
  run(0,0);delay(200); 
}


unsigned long IR_search_r_offset_std = 120;
unsigned long QSD_search_r_offset_std = 50;
unsigned long each_step_time = 1500;

int US_approach = 25;
int US_catch = 8;

void IR_approach(){
  // repeat TSOP sweep and approach 1.5 seconds until the front IR distance sensor sees the dummy
  ini();capture();
  int count = 0;
  while (count < IRd_count){
    IR_search_r(IR_search_r_offset_std);
    unsigned long t = millis() + each_step_time;
    run(200,200);
    while(millis() < t && count < IRd_count){
      // if (US() < US_approach && US() != 0){count += 1;}else{count = 0;}
      if (IRd_f() == 1){count += 1;}else{count = 0;}
    }
  }
  run(0,0);
  ini_h();delay(1000);
  IRd_approach_dummy();
  ini();
}


void QSD_approach(){
  
  // repeat QSD sweep and approach 1.5 seconds until the front IR distance sensor sees the dummy
  ini();capture();
  int count = 0;
  while (count < IRd_count){
    QSD_search_r(QSD_search_r_offset_std);
    unsigned long t = millis() + each_step_time;
    run(200,200);
    while(millis() < t && count < IRd_count){
      // if (US() < US_approach && US() != 0){count += 1;}else{count = 0;}
      if (IRd_f() == 1){count += 1;}else{count = 0;}
    }
  }
  run(0,0);
  ini_h();delay(1000);
  IRd_approach_dummy();
  ini();
}
