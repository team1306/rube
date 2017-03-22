#include <Servo.h>

const int SERVO_PIN[] = {A0,A1,A2};
const int SERVO_CLOSE[] = {20,50,90};
const int SERVO_OPEN[] = {80,0,0};
const int SERVO_DELAY = 1000;

const int BUTTON_PIN[] = {2,3,4};

Servo servo1;
Servo servo2;
Servo servo3;

const boolean debug = false;
  
void setup() {
  //Serial.begin(9600);
  //Serial.println("Ruby Servo Starting!");
  servo1.attach(SERVO_PIN[0]);
  servo2.attach(SERVO_PIN[1]);
  servo3.attach(SERVO_PIN[2]);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  pinMode(BUTTON_PIN[0], INPUT);
  pinMode(BUTTON_PIN[1], INPUT);
  pinMode(BUTTON_PIN[2], INPUT);
}

void loop() {
  if (digitalRead(BUTTON_PIN[0])) {
      servo1.write(SERVO_CLOSE[0]);
  }
  if (digitalRead(BUTTON_PIN[1])) {
      servo2.write(SERVO_CLOSE[1]);
  }
  if (digitalRead(BUTTON_PIN[2])) {
      servo3.write(SERVO_CLOSE[2]);
  }

  if (!digitalRead(BUTTON_PIN[0])) {
      servo1.write(SERVO_OPEN[0]);
  }
  if (!digitalRead(BUTTON_PIN[1])) {
      servo2.write(SERVO_OPEN[1]);
  }
  if (!digitalRead(BUTTON_PIN[2])) {
      servo3.write(SERVO_OPEN[2]);
  }
}

void servo(Servo servo, int servoNum) {
  servo.write(SERVO_CLOSE[servoNum]);
  delay(SERVO_DELAY);
  servo.write(SERVO_OPEN[servoNum]);
}
