#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "Adafruit_Trellis.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Constants for LED strips
const int STRIP1_PIN = 11;
const int STRIP1_NUM = 120;
//const int STRIP2_PIN = 11;
//const int STRIP2_NUM = 118;

//Constants for trellis
const int TRELLIS_PIN = 2;
const int TRELLIS_KEYS = 16;
const int LED_RESET_BUTTON = 15;

//Constants for servo
//Servo *servos[3];
//const int SERVO_PIN[] = {A0,A1,A2};
//const int SERVO_CLOSE[] = {20,90,90};
//const int SERVO_OPEN[] = {100,0,0};
//const int SERVO_DELAY = 2000;

//LED Color (0-255)
const int COLOR_R = 255;
const int COLOR_G = 0;
const int COLOR_B = 0;

//Word and letter constants
const int WORD_START[] = {93, 71, 0, 21, 47, 48};
const int WORD_END[] = {119, 94, 22, 49, 72};
const int WORD_DELAY = 500;
//const int WORD_NUM[] = {21,27,23,22,25};  //Learn, Inspire, Grow, Teach, Create
//const int WORD_ORDER[] = {4,3,0,1,2};     //Create, Teach, Learn, Inspire, Grow
const int LETTER_SPEED = 500;


//Create neopixel strips and trellis
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(STRIP1_NUM, STRIP1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

int delayval = 50; // delay for half a second
static int currentWord = 0;
static int servoIndex = 0;

void setup() {
  Serial.begin(9600); //Set logging baud rate
  Serial.println("RUBY STARTING!");

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
          trellis.writeDisplay();
          Serial.print("v"); Serial.println(i);
          if (i == LED_RESET_BUTTON) {
            clearLEDs();
          }
          else {
            Serial.print("Light word: ");
            Serial.println(currentWord);
            lightNextWord();
            //delay(WORD_DELAY);
          }
        }
      }
    }
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

void clearLEDs() {
   for (int i=0; i<STRIP1_NUM; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
  }
  for (int i=0; i<TRELLIS_KEYS; i++) {
    trellis.clrLED(i);
  }
  trellis.writeDisplay();
  currentWord = 0;
  servoIndex = 0;
}

void lightNextWord() {
  int startLED = WORD_START[currentWord];
  int endLED = WORD_END[currentWord];
  int numLED = endLED-startLED-1; //-1 to account for 0 indexing
  int prevLEDs = 0;
  for (int i=0; i<numLED; i++) {
    pixels.setPixelColor(startLED+i, pixels.Color(255,0,0));
    pixels.show();
  }
  currentWord++;
}

