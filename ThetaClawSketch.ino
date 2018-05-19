#include <Servo.h>

//define servos
Servo right;
Servo left;
Servo middle;
Servo claw;

//define joystick pins
const int R_SW_pin = 7; // Left joystick switch output connected to digital pin
const int R_X_pin = 1; // Left joystick X output connected to analog pin
const int R_Y_pin = 0; // Right joystick y output connected to analog pin

const int L_SW_pin = 6; // Left joystick switch output connected to digital pin
const int L_X_pin = 3; // Left joystick X output connected to analog pin
const int L_Y_pin = 2; // Right joystick y output connected to analog pin

//variables to store and read values from analog pins
int currRX = 0;
int currRY = 0;
int initJoyValRX = 0;
int initJoyValRY = 0;
int currLX = 0;
int currLY = 0;
int initJoyValLX = 0;
int initJoyValLY = 0;

void setup() {
  right.attach(10);
  middle.attach(11);
  left.attach(12);
  claw.attach(9);
  pinMode(L_SW_pin, INPUT);
  digitalWrite(L_SW_pin,HIGH);
  pinMode(R_SW_pin, INPUT);
  digitalWrite(R_SW_pin,HIGH);
  Serial.begin(115200);
  initJoyValLX = analogRead(L_X_pin);
  initJoyValLY = analogRead(L_Y_pin);
  initJoyValRX = analogRead(R_X_pin);
  initJoyValRY = analogRead(R_Y_pin);
  
  // get joystick values with no motion to remap later
  currRX = map(initJoyValRX, 0, 1023, 0, 180);
  currRY = map(initJoyValRY, 0, 1023, 0, 180);
  currLX = map(initJoyValLX, 0, 1023, 0, 180);
  currLY = map(initJoyValLY, 0, 1023, 0, 180);
}


int joyValRX_analog = 0;
int joyValRY_analog = 0;
int joyValRX = 0;
int joyValRY = 0;
int joyValLX_analog = 0;
int joyValLY_analog = 0;
int joyValLX = 0;
int joyValLY = 0;

void loop() {
  while(true){
    joyValRX_analog = analogRead(R_X_pin);
    delay(8);
    joyValRY_analog = analogRead(R_Y_pin);
    delay(8);
    joyValLX_analog = analogRead(L_X_pin);
    delay(8);
    joyValLY_analog = analogRead(L_Y_pin);
    delay(8);
    // remapping values so the initial joystick analog read is the default
    joyValRX = remap(joyValRX_analog, initJoyValRX);
    joyValRY = remap(joyValRY_analog, initJoyValRY);
    joyValLX = remap(joyValLX_analog, initJoyValLX);
    joyValLY = remap(joyValLY_analog, initJoyValLY);
    
    currRX += joyValRX;
    currRY += joyValRY;
    currRX = servoWrite(currRX, 90, 180); //right
    currRY = servoWrite(currRY, 0, 180); //middle
    
    currLX += joyValLX;
    currLY += joyValLY;
    currLX = servoWrite(currLX, 35, 70); //left
    currLY = servoWrite(currLY, 10, 145); //claw
    
    middle.write(currRX);
    right.write(currRY);
    left.write(currLX);
    claw.write(currLY);
    
    delay(20);
  }
}

int servoWrite(int curr, int minVal, int maxVal){
    if (curr > maxVal) {
      curr = maxVal;
    }
    else if (curr < minVal) {
      curr = minVal;
    }
    return curr;
}

int remap(int analog, int initJoyVal){
  int joyVal = 0;
  if (analog < 212) {
      joyVal = -1;
  } 
  else if (analog > 812) {
      joyVal = 1;
  } else {
      joyVal = 0;
  }
  Serial.print(joyVal);
  return joyVal;
}

void resetServoPosition(int curr, Servo servo) {
  int new_val = 0;
  if (servo.read() < curr) {
    while (servo.read() < curr) {
      new_val = servo.read() + 3;
      servo.write(new_val);
      Serial.println(new_val);
      delay(50);
    }
  } else {
    while (servo.read() >= curr) {
      new_val = servo.read() - 3;
      servo.write(new_val);
      Serial.println(new_val);
      delay(50);
    }
  }
}
