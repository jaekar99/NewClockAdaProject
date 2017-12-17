
//  Includes below:
#include <Wire.h>
#include <stdio.h>
#include <Adafruit_NeoPixel.h>
#include <RTClib.h> 

RTC_DS1307 RTC;             // Create RTC object

//  Define things here and set things up.
#define LED_Loop 60
#define PIN 6    // This is defining which Arduino pin is driving the Pixel ring

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_Loop, PIN, NEO_GRB + NEO_KHZ800);

int song_playing;
int mins;
int LED[LED_Loop];
int Q     =  (LED_Loop / 4);
int THREE =  (Q-1);
int SIX =    ((Q * 2)-1);
int NINE =   ((Q * 3)-1);
int TWELVE = ((Q * 4)-1);
//int TWELVE = (LED_Loop-1);

int ONE = (LED_Loop/12);    //  Should be 5 if loop = 60

int HR_Fade = 7;
int MN_Fade;
//       /////////////////////////////////////Second hand color
long HR_Colour;
long SE_Colour = 0x000010;

long THIS_LED;
int Led_Flag;
int argh;

//  trying this way to get colours working.
int HR_R;
int HR_G;
int HR_B;

//hour pixel colour
int HR1_R = 0x64;
int HR1_G = 0;
int HR1_B = 0;

//hour pixel colour for PM mode (if enabled)
//int HR2_R = 0;
//int HR2_G = 0x64;
//int HR2_B = 0;

//minute pixel colour
int MN_R = 0;
int MN_G = 0x50;
int MN_B = 0;

//int SE_R = 0;
//int SE_G = 0x55;
//int SE_B = 0;

int hour_led;
int minute_led;
int second_led;
int new_minute;

//-----------------------------  Set up here -----------------------------//
void setup() 
{
  // put your setup code here, to run once:
  delay(2000);          //  This is just to give you time to open the window and watch.
  Serial.begin(9600);
  Serial.println("-----------------------------");
  Serial.println("Setting up");
  
  Wire.begin();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

/*  
  Serial.print(THREE);
  Serial.print(" ");
  Serial.print(SIX);
  Serial.print(" ");
  Serial.print(NINE);
  Serial.print(" ");
  Serial.println(TWELVE);
*/

  if (! RTC.isrunning()) 
  {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("Done setting up");
  Serial.println("-----------------------------");
}

//---------------------------  MAIN LOOP here ---------------------------//
void loop() 
{
  //  Get time
   DateTime now = RTC.now();

   //  24 hour time change colour of hour hand.
   int hr = now.hour();
//   if (hr < 13)
//   {
     //HR_Colour = HR_Colour1;
     HR_R = HR1_R;
     HR_G = HR1_G;
     HR_B = HR1_B;
//   }
//   else
//   {
//     //HR_Colour = HR_Colour2;
//     HR_R = HR2_R;
//     HR_G = HR2_G;
//     HR_B = HR2_B;
//   }
   
   mins = now.minute();        //  This line is only incase any maths had to be done.
   second_led = now.second();

  //
  //  calculate leds
   hour_led = (((LED_Loop/12) * hr) + (mins / (LED_Loop/5)))%LED_Loop;


   minute_led = mins;

//  Debug code below
//
//----------------------------------------//
/*    
    Serial.print(now.year());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.day());
    Serial.print(' ');
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.println();
*/
//----------------------------------------//
/*
  Serial.println("=========================");
  Serial.println(hr);
  Serial.println(mins);
  Serial.println("-----------------------");
  Serial.println(hour_led);
  Serial.println(minute_led);
  Serial.println(second_led);
*/
//----------------------------------------//

  //
  //  Show LEDs            ----------------   Main loop here ---------------

   //  Keep this at the top so it doesn't mess up any other settings when LEDs are
   //  turned on.  This is where you set the brightness of the second hand trailing leds.  
   //  Set the last part 0xXX to a number between 55 and 0.  Zero returns the LED to off.
   strip.setPixelColor(second_led-1,0x08);
   strip.setPixelColor(second_led-2,0x06);
   strip.setPixelColor(second_led-3,0x04);
   strip.setPixelColor(second_led-4,0x02);
   strip.setPixelColor(second_led-5,0x0);

  //
  //  show THREE, SIX, NINE and TWELVE
  //
  
  strip.setPixelColor (THREE, 0x050505);
  strip.setPixelColor (SIX, 0x111100);
  strip.setPixelColor (NINE, 0x050505);
  strip.setPixelColor (TWELVE, 0x111100);
  
  //
  //  show the other numbers.   (Optional)
  //
  strip.setPixelColor(4, 0x050505);
  strip.setPixelColor(9, 0x050505);
  strip.setPixelColor(19, 0x050505);
  strip.setPixelColor(24, 0x050505);
  strip.setPixelColor(34, 0x050505);
  strip.setPixelColor(39, 0x050505);
  strip.setPixelColor(49, 0x050505);
  strip.setPixelColor(54, 0x050505);


  //
  //  Now start setting LEDs below here.
  //

   if (second_led == 0)
   {
      strip.setPixelColor(LED_Loop-1, SE_Colour/2);
      strip.setPixelColor(LED_Loop-2,SE_Colour/4);
      strip.setPixelColor(LED_Loop-3,SE_Colour/8);
      strip.setPixelColor(LED_Loop-4,SE_Colour/16);
      strip.setPixelColor(LED_Loop-5,0);
      new_minute = 1;
   }
   if (second_led == 1)
   {
      strip.setPixelColor(second_led-1, SE_Colour/2);
      strip.setPixelColor(LED_Loop-1, SE_Colour/4);
      strip.setPixelColor(LED_Loop-2,SE_Colour/8);
      strip.setPixelColor(LED_Loop-3,SE_Colour/16);
      strip.setPixelColor(LED_Loop-4,0);
   }
   if (second_led == 2)
   {
      strip.setPixelColor(second_led-1, SE_Colour/2);
      strip.setPixelColor(second_led-2, SE_Colour/4);
      strip.setPixelColor(LED_Loop-1, SE_Colour/8);
      strip.setPixelColor(LED_Loop-2,SE_Colour/16);
      strip.setPixelColor(LED_Loop-3,0);
   }
   if (second_led == 3)
   {
      strip.setPixelColor(second_led-1, SE_Colour/2);
      strip.setPixelColor(second_led-2, SE_Colour/4);
      strip.setPixelColor(second_led-3, SE_Colour/8);
      strip.setPixelColor(LED_Loop-1,SE_Colour/16);
      strip.setPixelColor(LED_Loop-2,0);
   }
   if (second_led == 4)
   {
      strip.setPixelColor(second_led-1, SE_Colour/2);
      strip.setPixelColor(second_led-2, SE_Colour/4);
      strip.setPixelColor(second_led-3, SE_Colour/8);
      strip.setPixelColor(second_led-4,SE_Colour/16);
      strip.setPixelColor(LED_Loop-1,0);
   }

/*
  if (Led_Flag == 0)
  {
    //
    Led_Flag = 1;
    THIS_LED = strip.getPixelColor(second_led);
    //  This is where I am at.
    Serial.print(second_led);
    Serial.print(" ");
    Serial.println(THIS_LED);
  }
*/  



  //--------------  Draw SECOND HAND on clock --------------/
   strip.setPixelColor(second_led,SE_Colour);
//   strip.setPixelColor(second_led,SE_Colour+THIS_LED);
//   strip.setPixelColor(second_led-1,THIS_LED);
   if (new_minute == 1)
   {
     //new_minute = 0;
//     strip.setPixelColor(minute_led-1,MN_Colour/50);
   }
  //--------------  Draw MINUTE HAND on clock --------------/
   //strip.setPixelColor(minute_led,MN_Colour);
   //  MN_Fade for fading.
   strip.setPixelColor(minute_led,MN_R,MN_G,MN_B);
   strip.setPixelColor(minute_led+1, MN_R,     (MN_G * (second_led*10/6)/100)      , MN_B);
   strip.setPixelColor(minute_led-1, MN_R,     (MN_G * (100-(second_led*10/6))/100)      , MN_B);
   
  //--------------  Draw HOUR HAND on clock --------------/
   strip.setPixelColor(hour_led,HR_R,HR_G,HR_B);
   //strip.setPixelColor((hour_led-1)%LED_Loop,HR_R/HR_Fade,HR_G,HR_B/HR_Fade);
   //strip.setPixelColor((hour_led+1)%LED_Loop,HR_R/HR_Fade,HR_G,HR_B/HR_Fade);
/*
   strip.setPixelColor(hour_led,HR_Colour);
   strip.setPixelColor((hour_led-1)%LED_Loop,HR_Colour);
   strip.setPixelColor((hour_led+1)%LED_Loop,HR_Colour);
*/   

   if (second_led > minute_led)
   {
     new_minute = 0;
   }

/*
  if (second_led != argh)
  {
     Led_Flag = 0;
     argh = second_led;
  }
*/ 
//   LED's are wired backwards.  They read G-R-B
//   Set the time for the show to happen below.
  if (minute_led == 0){
     if (second_led == 0){
//      while (song_playing == 1){
//                                    G----R----B
          theaterChase(strip.Color(    0,   0, 255), 50); // Blue
          theaterChase(strip.Color(   51,   0, 255), 50); // 
          theaterChase(strip.Color(   75,   0, 255), 50); //
          theaterChase(strip.Color(  102,   0, 255), 50); //
          theaterChase(strip.Color(  153,   0, 255), 50); //
          theaterChase(strip.Color(  204,   0, 255), 50); //
          theaterChase(strip.Color(  255,   0, 255), 50); // Aqua
          theaterChase(strip.Color(  255,   0, 204), 50); //
          theaterChase(strip.Color(  255,   0, 153), 50); //
          theaterChase(strip.Color(  255,   0, 102), 50); //
          theaterChase(strip.Color(  255,   0,  75), 50); //
          theaterChase(strip.Color(  255,   0,  51), 50); // 
          theaterChase(strip.Color(  255,   0,   0), 50); // Green
          theaterChase(strip.Color(  255,  51,   0), 50); // 
          theaterChase(strip.Color(  255,  75,   0), 50); //
          theaterChase(strip.Color(  255, 102,   0), 50); // 
          theaterChase(strip.Color(  255, 153,   0), 50); // 
          theaterChase(strip.Color(  255, 204,   0), 50); // 
          theaterChase(strip.Color(    0,   0, 255), 50); // Blue
          theaterChase(strip.Color(   51,   0, 255), 50); // 
          theaterChase(strip.Color(   75,   0, 255), 50); //
          theaterChase(strip.Color(  102,   0, 255), 50); //
          theaterChase(strip.Color(  153,   0, 255), 50); //
          theaterChase(strip.Color(  204,   0, 255), 50); //
          theaterChase(strip.Color(    0, 255,   0), 50); // Red
          theaterChase(strip.Color(    0, 255,  25), 50); //
          theaterChase(strip.Color(    0, 255,  51), 50); // 
          theaterChase(strip.Color(    0, 255,  75), 50); //
          theaterChase(strip.Color(    0, 255, 102), 50); // 
          theaterChase(strip.Color(    0, 255, 153), 50); // 
          theaterChase(strip.Color(    0, 255, 204), 50); // 
          theaterChase(strip.Color(    0, 255, 255), 50); // Purple
          theaterChase(strip.Color(    0, 204, 255), 50); // 
          theaterChase(strip.Color(    0, 153, 255), 50); // 
          theaterChase(strip.Color(    0, 102, 255), 50); //
          theaterChase(strip.Color(    0,  75, 255), 50); // 
          theaterChase(strip.Color(    0,  51, 255), 50); // 
         
          
          
          
   
//      }  
   }
  }


  
  


  //
  //  show alarms
  //
  
  strip.show();
  delay(500);
  //                    ----------------   End of Main loop here ---------------
}

//==================================================================
//                      -----------------  Light Crawl Code  -------------------
//Theatre-style crawling lights.

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }}}}
// Input a value 0 to 255 to get a color value.
// The colours are a transition g - r - b - back to g.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}







//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}



//==================================================================
//
//    end of code
//
//==================================================================﻿
