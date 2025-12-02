
#include <Arduino.h>
#define RIGHT_MOTOR1 5
#define RIGHT_MOTOR2 17
#define LEFT_MOTOR1  16
#define LEFT_MOTOR2  4
#define LED_PIN 2
#define INT 39

int capteurs[8] = {14, 27, 26, 25, 33, 32, 35, 34};
int thresholdVals[8] = {0,0,0,0,0,0,0,0}; 
int avgWhite[8] = {0,0,0,0,0,0,0,0}; 
int avgBlack[8] = {0,0,0,0,0,0,0,0}; 
float corr = 0.0;
float lastErr = 0.0;
int baseSpeed = 200;
int maxSpeed  = 255;
int minSpeed  = 100;
float integral = 0.0;
void setMotorLeft(int speed) {
  speed = constrain(speed, -maxSpeed, maxSpeed);
  if (speed >= 0) {
    analogWrite(LEFT_MOTOR1, speed);
    analogWrite(LEFT_MOTOR2, 0);
  } else {
    analogWrite(LEFT_MOTOR1, 0);
    analogWrite(LEFT_MOTOR2, -speed);
  }
}
void setMotorRight(int speed) {
  speed = constrain(speed, -maxSpeed, maxSpeed);
  if (speed >= 0) {
    analogWrite(RIGHT_MOTOR1, speed);
    analogWrite(RIGHT_MOTOR2, 0);
  } else {
    analogWrite(RIGHT_MOTOR1, 0);
    analogWrite(RIGHT_MOTOR2, -speed);
  }
}

void pidControl(float Kp, float Ki, float Kd) {
  int weights[8] = {-3, -2, -1, 0, 0, 1, 2, 3};
  float err = 0;
  int valSum = 0;
  for (int i = 0; i < 8; i++) {
    int g = getSensor(i);
    err += g * weights[i];
    valSum += g;
  }
  int x=235;

  if (valSum == 0) {
    if (lastErr > 0) {
      setMotorLeft(x);
      setMotorRight(-x);
    } else {
      setMotorLeft(-x);
      setMotorRight(x);
    }
    return;
  }
  err /= valSum;
  integral += err;
  float dErr = err - lastErr;
  lastErr = err;
  corr = Kp * err + Ki * integral + Kd * dErr;
  int leftSpeed  = x + corr*2;
  int rightSpeed = x - corr*2;
  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);
  setMotorLeft(leftSpeed);
  setMotorRight(rightSpeed);
}
void pidControl1(float Kp, float Ki, float Kd) {
  int weights[8] = {0,-1, -2, -3, 3, 2, 1, 0};
  float err = 0;
  int valSum = 0;
  for (int i = 0; i < 8; i++) {
    int g = getSensor(i);
    err += g * weights[i];
    valSum += g;
  }
  if (valSum == 0) {
    if (lastErr > 0) {
      setMotorLeft(baseSpeed);
      setMotorRight(-baseSpeed);
    } else {
      setMotorLeft(-baseSpeed);
      setMotorRight(baseSpeed);
    }
    return;
  }
  err /= valSum;
  integral += err;
  float dErr = err - lastErr;
  lastErr = err;
  corr = Kp * err + Ki * integral + Kd * dErr;
  int leftSpeed  = baseSpeed + corr;
  int rightSpeed = baseSpeed - corr;
  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);
  setMotorLeft(leftSpeed);
  setMotorRight(rightSpeed);
}
String checkSensorPattern() {
  String sensorString = "";
  for (int i = 0; i < 8; i++) {
    int val = getSensor(i);
    sensorString += String(val);
   
  }
  return(sensorString);}
  /*if (sensorString == "00011000" || sensorString == "00111000" || sensorString == "00011100") {
        pidControl(25,0.5,18);  
  } else if (sensorString.startsWith("1111") || sensorString.startsWith("1110") || sensorString.startsWith("1100")) {
    setMotorLeft(100);   // 
    setMotorRight(200);
  } else if (sensorString.endsWith("1111") || sensorString.endsWith("0111") || sensorString.endsWith("0011")) {
    setMotorLeft(200);   
    setMotorRight(100);
  } else if (sensorString == "00000000") {
    setMotorLeft(baseSpeed);
    setMotorRight(baseSpeed);
  } else {
    pidControl(25,0.5,18); 
  }
}*/
void pidControl2(float Kp, float Ki, float Kd) {
  int weights[8] = {-3, -2, -1, 0, 0, 1, 2, 3};

  float err = 0;
  int valSum = 0;
  for (int i = 0; i < 8; i++) {
    int g = !(getSensor(i));
    err += g * weights[i];
    valSum += g;
  }
  int x=baseSpeed-100;
  if (valSum == 0) {
    if (lastErr > 0) {
      setMotorLeft(x);
      setMotorRight(-x);
    } else {
      setMotorLeft(-x);
      setMotorRight(x);
    }
    return;
  }
  err /= valSum;
  integral += err;
  float dErr = err - lastErr;
  lastErr = err;
  corr = Kp * err + Ki * integral + Kd * dErr;
  int leftSpeed  = x + corr*3;
  int rightSpeed = x - corr*3;
  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);
  setMotorLeft(leftSpeed);
  setMotorRight(rightSpeed);
}
/*void pidControl2(float Kp, float Ki, float Kd) {
  int weights[8] = {-3, -2, -1, 0, 0, 1, 2, 3};
  float err = 0;
  int valSum = 0;

  // Invert sensors: white = 1, black = 0
  for (int i = 0; i < 8; i++) {
    int g = !(getSensor(i));
    err += g * weights[i];
    valSum += g;
  }

  if (valSum == 0) {
    // Line lost: gentle turn based on last error
    if (lastErr > 0) {
      setMotorLeft(minSpeed);
      setMotorRight(maxSpeed);
    } else {
      setMotorLeft(maxSpeed);
      setMotorRight(minSpeed);
    }
    return;
  }

  err /= valSum;
  integral += err;
  float dErr = err - lastErr;
  lastErr = err;

  corr = Kp * err + Ki * integral + Kd * dErr;

  int leftSpeed  = baseSpeed + corr;
  int rightSpeed = baseSpeed - corr;

  leftSpeed  = constrain(leftSpeed, minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);

  setMotorLeft(leftSpeed);
  setMotorRight(rightSpeed);
}*/
void pidControl3(float Kp, float Ki, float Kd) {
  int weights[8] = {-3, -2, -1, 0, 0, 1, 2, 3};
  float err = 0;
  int valSum = 0;
  for (int i = 0; i < 8; i++) {
    int g = getSensor(i);
    err += g * weights[i];
    valSum += g;
  }
  int x=255;

  if (valSum == 0) {
    if (lastErr > 0) {
      setMotorLeft(x);
      setMotorRight(-x);
    } else {
      setMotorLeft(-x);
      setMotorRight(x);
    }
    return;
  }
  err /= valSum;
  integral += err;
  float dErr = err - lastErr;
  lastErr = err;
  corr = Kp * err + Ki * integral + Kd * dErr;
  int leftSpeed  = x + corr;
  int rightSpeed = x - corr;
  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);
  setMotorLeft(leftSpeed);
  setMotorRight(rightSpeed);
}
void pidControl4(float Kp, float Ki, float Kd) {
    int weights[8] = {-6, -4, -2, 0, 0, 0, 0, 0};
  float err = 0;
  int valSum = 0;
  for (int i = 0; i < 8; i++) {
    int g = getSensor(i);
    err += g * weights[i];
    valSum += g;
  }
  if (valSum == 0) {
    if (lastErr > 0) {
      setMotorLeft(baseSpeed);
      setMotorRight(-baseSpeed);
    } else {
      setMotorLeft(-baseSpeed);
      setMotorRight(baseSpeed);
    }
    return;
  }
  err /= valSum;
  integral += err;
  float dErr = err - lastErr;
  lastErr = err;
  corr = Kp * err + Ki * integral + Kd * dErr;
  int leftSpeed  = baseSpeed + corr*2;
  int rightSpeed = baseSpeed - corr;
  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);
  setMotorLeft(leftSpeed);
  setMotorRight(rightSpeed);
}
void pidControl5(float Kp, float Ki, float Kd) {
  int weights[8] = {-3, -2, -1, 0, 0, 1, 2, 3};
  float err = 0;
  int x=baseSpeed-100; 
  int valSum = 0;
  for (int i = 0; i < 8; i++) {
    int g = getSensor(i);
    err += g * weights[i];
    valSum += g;
  }
  if (valSum == 0) {
    if (lastErr > 0) {
      setMotorLeft(x);
      setMotorRight(-x);
    } else {
      setMotorLeft(-x);
      setMotorRight(x);
    }
    return;
  }
  err /= valSum;
  integral += err;
  float dErr = err - lastErr;
  lastErr = err;
  corr = Kp * err + Ki * integral + Kd * dErr;
  int leftSpeed  = x + corr*3;
  int rightSpeed = x - corr*3;
  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);
  setMotorLeft(leftSpeed);
  setMotorRight(rightSpeed);
}
void pidControl8(float Kp, float Ki, float Kd) {
  int weights[8] = {0 ,0, -1, -3, 3, 1, 2, 3};
  float err = 0;
  int x=baseSpeed-20; 
  int valSum = 0;
  for (int i = 0; i < 8; i++) {
    int g = getSensor(i);
    err += g * weights[i];
    valSum += g;
  }
  if (valSum == 0) {
    if (lastErr > 0) {
      setMotorLeft(x);
      setMotorRight(-x);
    } else {
      setMotorLeft(-x);
      setMotorRight(x);
    }
    return;
  }
  err /= valSum;
  integral += err;
  float dErr = err - lastErr;
  lastErr = err;
  corr = Kp * err + Ki * integral + Kd * dErr;
  int leftSpeed  = x + corr*3;
  int rightSpeed = x - corr*3;
  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);
  setMotorLeft(leftSpeed);
  setMotorRight(rightSpeed);
}
void calibration() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println(F("Calibration: put robot on WHITE surface"));
  delay(2000);

  for (int i = 0; i < 8; i++) {
    avgWhite[i] = 0;
    avgBlack[i] = 0;
    for (int j = 0; j < 100; j++) {
      avgWhite[i] += analogRead(capteurs[i]);
      delay(5);
    }
  }
  for (int i = 0; i < 8; i++) avgWhite[i] /= 100;
  Serial.println(F("Now move sensors over BLACK line"));
  for (int t = 0; t < 20; t++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  digitalWrite(LED_PIN, HIGH); 
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 100; j++) {
      avgBlack[i] += analogRead(capteurs[i]);
      delay(5);
    }
  }
  for (int i = 0; i < 8; i++) avgBlack[i] /= 100;
  for (int i = 0; i < 8; i++) {
    thresholdVals[i] = (avgWhite[i] + avgBlack[i]) / 2;
  
  
   /* Serial.print(F("Sensor "));
    Serial.print(i);
    Serial.print(F(" Thr="));
    Serial.print(thresholdVals[i]);
    Serial.print(F(" W="));
    Serial.print(avgWhite[i]);
    Serial.print(F(" B="));
    Serial.println(avgBlack[i]);*/
  }
  Serial.println(F("Calibration DONE! Place robot at START position."));
  digitalWrite(LED_PIN, LOW); 
  delay(1000);
}
int getSensor(int i) {
  int v = analogRead(capteurs[i]);
  int isBlack = (v > thresholdVals[i]) ? 1 : 0; 
  return isBlack;
}
void forward(){
  analogWrite(LEFT_MOTOR1,255);
  analogWrite(RIGHT_MOTOR1,255);
  analogWrite(LEFT_MOTOR2,0);
  analogWrite(RIGHT_MOTOR2,0);
}
bool calibrated = false;
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 8; i++) pinMode(capteurs[i], INPUT);
  pinMode(LEFT_MOTOR1, OUTPUT);
  pinMode(LEFT_MOTOR2, OUTPUT);
  pinMode(RIGHT_MOTOR1, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR2, OUTPUT);
  pinMode(INT, INPUT);
  setMotorLeft(0);
  setMotorRight(0);
  delay(500);
  if (!calibrated) {
    setMotorLeft(0);
    setMotorRight(0);
    calibration();   
    calibrated = true; 
    Serial.println("Calibration done, robot ready!");
    delay(1000);     
  }
}


int test=0;
int flag1=0;


void loop() {
  
  while (digitalRead(INT)){}
  if(test==0){
    forward();
    delay(1075);
    test++;
  }
  digitalWrite(LED_PIN, HIGH); 

  unsigned long start = millis();
  while (millis() - start < 3500) {
    pidControl(70,0,50);
  }
  digitalWrite(LED_PIN, LOW); 

  unsigned long start1=millis();
  while (millis() - start1 < 1200) {
    forward();
  }
  digitalWrite(LED_PIN, HIGH); 

    
    unsigned long start2=millis();
    while (millis() - start2 < 1025) 
    {
        pidControl5(100,0,50);
    }
    digitalWrite(LED_PIN, LOW); 

    unsigned long start3 = millis();
while (millis() - start3 < 600) {
    int m = 0;
    for (int i = 0; i < 8; i++) {
        m += getSensor(i);
    }

    if (m >= 2) {
        pidControl(50, 0, 0);
    } 
    else if (m <= 2) {
        forward();
    }

}
  digitalWrite(LED_PIN, HIGH); 

    unsigned long start4=millis();
    while (millis() - start4 <1000) 
    {
        pidControl(70,0,20);
    }
  digitalWrite(LED_PIN, LOW); 


    unsigned long start5=millis();
    while (millis() - start5 <2200 )
    {
      pidControl2(70,0,10);
    }
      digitalWrite(LED_PIN, HIGH); 



    unsigned long start15=millis();
    while (millis() - start15 <500 )
    {
      pidControl(70,0,10);
    }
      digitalWrite(LED_PIN, HIGH); 

    /*unsigned long start6=millis();
     while (millis() - start6 <2500 ){
        pidControl3(50,0,20);
    }*/

    unsigned long start6=millis();
     while (millis() - start6 <1000){
      forward();    
    }
    unsigned long start11=millis();
     while (millis() - start11 <1000 ){
        pidControl3(50,0,20);
    }

    digitalWrite(LED_PIN, LOW); 

    unsigned long start9=millis();
     while (millis() - start9 <700 ){
        pidControl(70,0,20);

    }
    digitalWrite(LED_PIN, HIGH);


    unsigned long start7=millis();//
     while ((millis() - start7 <500 )) 
    {
        pidControl4(70,0,20);

    }
      digitalWrite(LED_PIN, LOW); 


    unsigned long start8=millis();
     while ((millis() - start8 <3000 )) 
    {
        pidControl8(70,0,20);

    }
      digitalWrite(LED_PIN, LOW); 
    //pidControl(100,0.0,0); 
    unsigned long start17=millis();//
     while ((millis() - start17 <5000000000000000000 )) 
    {
        int s=0;
        for(int i=0;i<8;i++){
          s+=getSensor(i);
        }
        if(s>=3){
          setMotorRight(0);
          setMotorLeft(0);

        }

    }
    

  delay(20);
}
