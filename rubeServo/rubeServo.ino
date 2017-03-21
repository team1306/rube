#include <Servo.h>

const int SERVO_PIN[] = {A0,A1,A2};
const int SERVO_CLOSE[] = {20,90,90};
const int SERVO_OPEN[] = {100,0,0};
const int SERVO_DELAY = 1000;

const int BUTTON_PIN[] = {1,2,3};

void setup() {
  Servo servo1 = new Servo();
  servo1.attach(SERVO_PIN[0]);
  Servo servo2 = new Servo();
  servo2.attach(SERVO_PIN[1]);
  Servo servo3 = new Servo();
  servo3.attach(SERVO_PIN[2]);

  pinMode(BUTTON_PIN[0], INPUT);
  pinMode(BUTTON_PIN[1], INPUT);
  pinMode(BUTTON_PIN[2], INPUT);
}

void loop() {
  if (digitalRead(BUTTON_PIN[0])) {
    servo(servo1, 0);
  }
  else if (digitalRead(BUTTON_PIN[1])) {
    servo(servo2, 1);
  }
  else if (digitalRead(BUTTON_PIN[2])) {
    servo(servo3, 2);
  }
}

void servo(Servo servo, int servoNum) {
  servo.write(SERVO_CLOSE[servoNum]);
  delay(SERVO_DELAY);
  servo.write(SERVO_OPEN[servoNum]);
}
