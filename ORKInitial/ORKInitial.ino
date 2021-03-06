/*

Project: ORK2.0 Kit Control Code
Version: .1
Date: November 2014

Authors:
Nick McComb
Ryan Skeele
Soo-Hyun Yoo


Description:
	Initial code to control ORK Kit.

	This version of the code is _very_ barebones and is intended for people who
	want to figure out what to do on their own.

	There will be future versions that are much more documented and easier to follow.

	PROCEDE AT YOUR OWN RISK.
	
	The most up-to-date version of this code can be found at https://github.com/OSURoboticsClub/ork

*/

#include <Servo.h>

//Constants
const int switchPin = 2;    // switch input
const int motor1Pin = 5;    // H-bridge leg 1 (pin 2, 1A)
const int motor2Pin = 4;    // H-bridge leg 2 (pin 7, 2A)
const int enablePin = 3;    // H-bridge enable pin

const int motor3Pin = 7;    // H-bridge leg 3
const int motor4Pin = 8;    // H-bridge leg 4
const int enable2Pin = 6;

const int shiftRegData = 2;
const int shiftRegClock = 12;

#define trigPin A2
#define echoPin A3

#define MOTOR_MIN 110
#define RIGHT_SCALE (9./11.)   // HACK


//DS - D12
//SH_CP - D13
//ST_CP - D10

Servo sonicServo;  //Create the servo object

void setup() {
    // set the switch as an input:
    pinMode(switchPin, INPUT); 
 
    // set all the other pins you're using as outputs:
    pinMode(motor1Pin, OUTPUT);
    pinMode(motor2Pin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    
    pinMode(motor3Pin, OUTPUT);
    pinMode(motor4Pin, OUTPUT);
    pinMode(enable2Pin, OUTPUT);
    
    //Ultrasonic
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

/*  Only for Shift Register (Not included in Kit)
    pinMode(shiftRegData, OUTPUT); // make the clock pin an output
    pinMode(shiftRegClock , OUTPUT); // make the data pin an output
*/

	//For testing
    pinMode(13, OUTPUT);
    pinMode(13, HIGH);
    
    sonicServo.attach(9);
 
    // set enablePin high so that motor can turn on:
    analogWrite(enablePin, 255);
    
    Serial.begin(9600);  //For debugging
}
    
void loop() {

  int x = 1;
  int var;
  //var = readDistance();
  //Initial Motor start PWM is around 30

  float rot = -1;
  float inc = 0.1;
  while (true) {
    driveRobot(1, rot);
    rot += inc;
    if (rot > 0.9 || rot < -0.9) {
      inc *= -1;
    }

/*  Only for Shift Register (Not included in Kit)
    if(inc < 0) writeToFace(B10101010);
    else writeToFace(B01010101);   
*/
    delay(100);
  }
}

void driveRobot(float lin, float rot){
  float left = max(-1, min(1, lin - rot));
  float right = max(-1, min(1, lin + rot));
  driveMotor(1, left);
  driveMotor(2, right);
}

void driveMotor(uint8_t motorSel, float motorSpeed){
  int dir = 1;
  float speed = motorSpeed;
  if (motorSpeed < 0) {
    dir = 0;
    speed *= -1;
  }
  if(motorSel == 1){  //Right Motor
    /* motorDir: forward 1, reverse 0 */
    digitalWrite(motor1Pin, dir);  // set leg 1 of the H-bridge high
    digitalWrite(motor2Pin, 1 - dir);   // set leg 2 of the H-bridge low
    analogWrite(enablePin, MOTOR_MIN + (255-MOTOR_MIN)*speed*RIGHT_SCALE);
  }
  else if (motorSel == 2){  //Left motor
    digitalWrite(motor3Pin, dir);  // set leg 1 of the H-bridge high
    digitalWrite(motor4Pin, 1 - dir);   // set leg 2 of the H-bridge low
    analogWrite(enable2Pin, MOTOR_MIN + (255-MOTOR_MIN)*speed);
  }
}

//Returns distance
long readDistance(void){
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

//Only for Shift Register (Not included in Kit)
//Don't try to call this function.
void writeToFace(int dataToWrite){
  shiftOut(shiftRegData, shiftRegClock, LSBFIRST, dataToWrite);
}


