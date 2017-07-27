// Original code by Tobias Floery <tobias@floery.net>
// Converted to FastLED by Tablelands Computers <info@tablelandscomputers.com>

// FastLED Library from https://github.com/FastLED/FastLED
#include <FastLED.h>

// Adafruits RTClib from https://github.com/adafruit/RTClib
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

#define NUM_LEDS 60
#define LED_TYPE WS2812
#define DATA_PIN 11
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

// Filter averaging factor - higher value means more sluggish response to changes in ambient brightness
#define FILTER_LEN 8
// Brightness level initial value
int level=15;
// Brightness correction for Leds
unsigned char bright[] = {0, 2, 4, 7, 11, 18, 30, 40, 50, 65, 80,96,125,160, 200, 255};

void setup()
{
  
  Serial.begin(57600);

  // init RTClib  
  Wire.begin();
  RTC.begin();
  
  // Following line sets the RTC to the date & time this sketch was compiled
  /*
  RTC.adjust(DateTime(__DATE__, __TIME__));
  */
  
  // Setup FastLED Lib
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);

  // setup measurement for LDR
  digitalWrite(A0, HIGH);  // set pullup on analog pin 0 
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);    
  
}

unsigned int ldr=0;

void doLDR() {
  unsigned int ldr_value = analogRead(0);          //reads the LDR values
  
  //Adjust for LDR variations.
  //dark = 1023
  //light = 0
  //If your LDR for example reaches a max analogRead value of 350 instead of 1023 for 100% dark.
  //unsigned int ldr_adjusted=map(ldr_value,0,350,0,1023);
  unsigned int ldr_adjusted=map(ldr_value,0,700,0,1023);

  //set max limit (to avoid potential 'divide-by-zero's).
  if (ldr_adjusted >= 1023) ldr_adjusted = 1023;

  //ldr follows ldr_adjusted, but is averaged so that values only change gradually
  ldr = ((unsigned long)(FILTER_LEN-1)*ldr+ldr_adjusted)/FILTER_LEN;

  unsigned int light_level=0;

  //translate [0-1023] to [15-0]
  //light_level = (1024-ldr)>>6;
  light_level=map(ldr,0,1023,15,0);  
  if (light_level >= 15) light_level = 15;
  if (light_level <= 1) light_level = 1;

  //DEBUG SPI
  // /*
  Serial.println("ldr_value [0-???]:");
  Serial.println(ldr_value);
  Serial.println("ldr_adjusted [0-1023]:");
  Serial.println(ldr_adjusted);
  Serial.println("ldr:");
  Serial.println(ldr);
  Serial.println("light_level:");
  Serial.println(light_level);
  // */

  //level=2;  
  level = light_level;

}

DateTime old;

void loop() { 
  // get time
  DateTime now = RTC.now();
  
  // DEBUG RTC
  // /*
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  // */

  if (now.second()!=old.second()) {   
    old = now;
    doLDR();
  }

  // clear LED array
  memset(leds, 0, NUM_LEDS * 3);
    
  if (level < 4) {  
    // this is the night mode
    // set LED background
    for (int i=0; i<NUM_LEDS; i++) {
      leds[i].r = bright[level];
      leds[i].g = (bright[level]+1)/2;
    }  
    // set pixels
    //leds[(now.second())%60].r = bright[level];
    leds[(now.minute())%60].g = 0; 
    leds[(now.minute())%60].r = bright[level+2];
    
    unsigned char hourpos = (now.hour()%12)*5;
    leds[((59+hourpos)%60)%60].g = 0;
    leds[(hourpos)%60].g = 0;
    leds[((hourpos+1))%60].g = 0;
    
    leds[(hourpos)%60].r = bright[level+2];
    
  } else {

  
    // set pixels
    leds[(now.second())%60].r = bright[level];
    leds[(now.minute())%60].b = bright[level]; 
    
    unsigned char hourpos = (now.hour()%12)*5;
    leds[((59+hourpos)%60)%60].g = bright[level/2];
    leds[(hourpos)%60].g = bright[level];
    leds[((hourpos+1))%60].g = bright[level/2];  
  }
  // update LEDs
  //FastLED.setBrightness(100);
  FastLED.show();
  FastLED.delay(500);
  
  // wait some time
  //delay(500);
}
