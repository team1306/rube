#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Servo.h>
#include "Adafruit_Trellis.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Constants for LED strips
const int STRIP1_PIN = 11;
const int STRIP1_NUM = 120;
const int STRIP2_PIN = 11;
const int STRIP2_NUM = 120;

//Constants for trellis
const int TRELLIS_PIN = 2;
const int TRELLIS_KEYS = 16;
const int LED_RESET_BUTTON = 15;

//Constants for servo
const int SERVO_PIN[] = {9};
const int SERVO_OPEN = 180;
const int SERVO_CLOSE = 0;
const int SERVO_DELAY = 500;
Servo servos[5];

//LED Color (0-255)
const int COLOR_R = 150;
const int COLOR_G = 0;
const int COLOR_B = 0;

//Word and letter constants
const int WORD_DELAY = 500;
const int WORD_NUM[] = {25,24,24,24,24};
const int LETTER_SPEED = 50;


//Create neopixel strips and trellis
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(STRIP1_NUM, STRIP1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

int delayval = 50; // delay for half a second

void setup() {
  Serial.begin(9600); //Set logging baud rate

  Servo servo1;
  setupServo(servo1, 0);
  //servo1.attach(SERVO_PIN[0]);
  //servo1.write(0);
  //servos[0] = {servo1};
  
  pinMode(TRELLIS_PIN, INPUT);
  digitalWrite(TRELLIS_PIN, HIGH);
  trellis.begin(0x70);
  trellisBootLights();
  
  pixels.begin();
  clearLEDs();
}

void loop() {
  delay(30);
    if (trellis.readSwitches()) {
      for (uint8_t i=0; i<TRELLIS_KEYS; i++) {
        if (trellis.justPressed(i)) {
          trellis.setLED(i);
          Serial.print("v"); Serial.println(i);
          if (i<sizeof(servos)) {
            servo(i);
            delay(WORD_DELAY);
          }
          if (i == LED_RESET_BUTTON) {
            clearLEDs();
          }
          else {
            lightWord(i);
          }
        } 
        // if it was released, turn it off
        if (trellis.justReleased(i)) {
          Serial.print("^"); Serial.println(i);
          //pixels.setPixelColor(i, pixels.Color(0,0,0));
          //pixels.show();
          trellis.clrLED(i);
          //offWord(i);
        }
      }
      trellis.writeDisplay();
    }
  
  /*for(int i=0;i<STRIP1_NUM;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(COLOR_R,COLOR_G,COLOR_B)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }*/
}

void trellisBootLights() {
  //Trellis lights on
  for (uint8_t i=0; i<TRELLIS_KEYS; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
  
  //Trellis lights off
  for (uint8_t i=0; i<TRELLIS_KEYS; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
}

void setupServo(Servo servo, int servoNum) {
  servo.attach(SERVO_PIN[servoNum]);
  servo.write(0);
  servos[servoNum] = servo;
}

void servo(int s) {
  Serial.print("SERVO ");
  Serial.println(s);
  servos[s].write(SERVO_OPEN);
  delay(SERVO_DELAY);
  servos[s].write(SERVO_CLOSE);
}

void clearLEDs() {
   for (int i=0; i<STRIP1_NUM; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
  }
   /*for (int i=0; i<STRIP2_NUM; i++) {
    pixels.setPixelColor(i+prevLEDs, pixels.Color(0,0,0));
    pixels.show();
  }*/
}

void lightWord(int lightWord) {
  int LEDs = WORD_NUM[lightWord];
  int prevLEDs = 0;
  for (int i=0; i<lightWord; i++) {
    prevLEDs+=WORD_NUM[i];
  }
  for (int i=0; i<LEDs; i++) {
    pixels.setPixelColor(i+prevLEDs, pixels.Color(COLOR_R,COLOR_G,COLOR_B));
    pixels.show();
    delay(LETTER_SPEED);
  }
}
void offWord(int lightWord) {
  int LEDs = WORD_NUM[lightWord];
  int prevLEDs = 0;
  int c = 0;
  for (int i=0; i<lightWord; i++) {
    prevLEDs+=WORD_NUM[i];
  }
    Serial.print("Found words");
    Serial.println(prevLEDs);
  for (int i=0; i<LEDs; i++) {
    pixels.setPixelColor(i+prevLEDs, pixels.Color(0,0,0));
    pixels.show();
    delay(LETTER_SPEED);
  }
}

