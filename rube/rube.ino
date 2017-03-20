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
Servo servos[3];
const int SERVO_PIN[] = {A0,A1,A2};
const int SERVO_CLOSE[] = {0,0,0};
const int SERVO_OPEN[] = {60,60,60};
const int SERVO_DELAY = 10000;

//LED Color (0-255)
const int COLOR_R = 255;
const int COLOR_G = 0;
const int COLOR_B = 0;

//Word and letter constants
const int WORD_START[] = {93, 71, 0, 21, 47, 48};
const int WORD_END[] = {117, 92, 21, 47, 70};
const int WORD_DELAY = 500;
//const int WORD_NUM[] = {21,27,23,22,25};  //Learn, Inspire, Grow, Teach, Create
//const int WORD_ORDER[] = {4,3,0,1,2};     //Create, Teach, Learn, Inspire, Grow
const int LETTER_SPEED = 50;


//Create neopixel strips and trellis
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(STRIP1_NUM, STRIP1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

int delayval = 50; // delay for half a second
static int currentWord = 0;

void setup() {
  Serial.begin(9600); //Set logging baud rate
  Serial.println("RUBY STARTING!");

  Servo servo1, servo2, servo3;
  setupServo(servo1, 0);
  setupServo(servo2, 1);
  setupServo(servo3, 2);
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

int servoIndex = 0;
void loop() {
  delay(30);
    if (trellis.readSwitches()) {
      for (uint8_t i=0; i<TRELLIS_KEYS; i++) {
        if (trellis.justPressed(i)) {
          trellis.setLED(i);
          trellis.writeDisplay();
          //Serial.print("v"); Serial.println(i);
          if (i == LED_RESET_BUTTON) {
            clearLEDs();
          }
          else if (i<sizeof(servos) && (i%2==1)) {
            servo(servoIndex);
            servoIndex++;
          }
          else {
            lightNextWord();
            //delay(WORD_DELAY);
          }
        }
      }
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
  Serial.print("Atached Servo: ");
  Serial.println(SERVO_PIN[servoNum]);
  servo.write(SERVO_CLOSE[servoNum]);
  servos[servoNum] = servo;
}

void servo(int s) {
  Serial.print("SERVO ");
  Serial.println(s);
  servos[s].write(SERVO_OPEN[s]);
//  delay(SERVO_DELAY);
//  servos[s].write(SERVO_CLOSE[s]);
}

void clearLEDs() {
   for (int i=0; i<STRIP1_NUM; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
  }
  for (int i=0; i<TRELLIS_KEYS; i++) {
    trellis.clrLED(i);
  }
  trellis.writeDisplay();
}

void lightNextWord() {
  int startLED = WORD_START[currentWord];
  int endLED = WORD_END[currentWord];
  int numLED = endLED-startLED-1; //-1 to account for 0 indexing
  int prevLEDs = 0;
  for (int i=0; i<numLED; i++) {
    pixels.setPixelColor(startLED+i, pixels.Color(COLOR_R,COLOR_G,COLOR_B));
  }
}

//void lightWord() {
//  int wordI = WORD_ORDER[lightWord];
//  int LEDs = WORD_NUM[lightWord];
//  int prevLEDs = 0;
//  for (int i=0; i<lightWord; i++) {
//    prevLEDs+=WORD_NUM[i];
//  }
//  for (int i=0; i<LEDs; i++) {
//    pixels.setPixelColor(i+prevLEDs, pixels.Color(COLOR_R,COLOR_G,COLOR_B));
//    pixels.show();
//    delay(LETTER_SPEED);
//  }
//}
//void offWord(int lightWord) {
//  int LEDs = WORD_NUM[lightWord];
//  int prevLEDs = 0;
//  int c = 0;
//  for (int i=0; i<lightWord; i++) {
//    prevLEDs+=WORD_NUM[i];
//  }
//    Serial.print("Found words");
//    Serial.println(prevLEDs);
//  for (int i=0; i<LEDs; i++) {
//    pixels.setPixelColor(i+prevLEDs, pixels.Color(0,0,0));
//    pixels.show();
//    delay(LETTER_SPEED);
//  }
//}

