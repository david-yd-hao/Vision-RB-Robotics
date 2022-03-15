# include <math.h>

float getAngle(float x1, float y1, float x2, float y2){
    float dx = x2 - x1;
    float dy = y1 - y2;
    Serial.println(String("dx:  ")+dx+String("     dy:   ")+dy);
    float angle = atan2f(dy, dx) * 180 / M_PI;
    if(angle<0){
        angle = 360+angle;
    }
    return angle;
}
