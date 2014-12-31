#include <MicroView.h>
#include <avr/pgmspace.h>

#define X 32
#define Y 24
#define STAR 80

static const unsigned char bmp [] PROGMEM = {
  0x1f, 0x02, 0x1e, 0x03, 0x20, 0x03, 0x1e, 0x04, 0x21, 0x04, 0x1d, 0x05, 0x21, 0x05, 0x1d, 0x06, 0x22, 0x06, 0x1c, 0x07, 0x22, 0x07, 0x1c, 0x08, 0x23, 0x08, 0x1c, 0x09, 0x23, 0x09, 
  0x1b, 0x0a, 0x24, 0x0a, 0x19, 0x0b, 0x1a, 0x0b, 0x1b, 0x0b, 0x24, 0x0b, 0x25, 0x0b, 0x26, 0x0b, 0x27, 0x0b, 0x17, 0x0c, 0x18, 0x0c, 0x1b, 0x0c, 0x24, 0x0c, 0x28, 0x0c, 0x29, 0x0c, 
  0x16, 0x0d, 0x1a, 0x0d, 0x25, 0x0d, 0x2a, 0x0d, 0x15, 0x0e, 0x1a, 0x0e, 0x25, 0x0e, 0x2b, 0x0e, 0x14, 0x0f, 0x1a, 0x0f, 0x25, 0x0f, 0x2c, 0x0f, 0x14, 0x10, 0x19, 0x10, 0x26, 0x10, 
  0x2c, 0x10, 0x14, 0x11, 0x19, 0x11, 0x26, 0x11, 0x2c, 0x11, 0x14, 0x12, 0x19, 0x12, 0x26, 0x12, 0x27, 0x12, 0x2c, 0x12, 0x15, 0x13, 0x18, 0x13, 0x27, 0x13, 0x2b, 0x13, 0x16, 0x14, 
  0x18, 0x14, 0x20, 0x14, 0x21, 0x14, 0x22, 0x14, 0x27, 0x14, 0x2a, 0x14, 0x17, 0x15, 0x18, 0x15, 0x1f, 0x15, 0x23, 0x15, 0x27, 0x15, 0x28, 0x15, 0x29, 0x15, 0x18, 0x16, 0x1e, 0x16, 
  0x24, 0x16, 0x28, 0x16, 0x18, 0x17, 0x1d, 0x17, 0x1e, 0x17, 0x1f, 0x17, 0x20, 0x17, 0x21, 0x17, 0x22, 0x17, 0x23, 0x17, 0x24, 0x17, 0x28, 0x17, 0x17, 0x18, 0x1c, 0x18, 0x25, 0x18, 
  0x28, 0x18, 0x17, 0x19, 0x1b, 0x19, 0x26, 0x19, 0x28, 0x19, 0x17, 0x1a, 0x1a, 0x1a, 0x27, 0x1a, 0x17, 0x1b, 0x19, 0x1b, 0x18, 0x1c };

int sensorPin = A0;      // select the input pin for the potentiometer
int starCount = 10;
int starValue = 10;      // variable to store the value coming from the sensor
int starValueOld = 10;

float star_x[STAR], star_y[STAR], star_z[STAR]; 
byte star_screen_x[STAR], star_screen_y[STAR], star_t[STAR];

void setup() {
  randomSeed(analogRead(sensorPin));    // init random seed
  digitalWrite(sensorPin, HIGH);        // internal pull up
  pinMode(sensorPin, INPUT);            // make pin as input
  uView.begin();                        // start microview
  uView.clear(PAGE);                    // clear page
  init_star();                          // init all stars
  show_title();                         // show title
  delay(1000);                          // wait 1 seconde
  uView.clear(PAGE);                    // and clear page again
}

void loop() {
  starValue = analogRead(sensorPin);    // read sensorPin
  starValue = map(starValue, 0, 1023, 10, 85);
  if(abs(starValue - starValueOld) > 2) {
    starValueOld = starValue;
    starCount = starValue;
    if(starCount <= 80) {
      uView.setCursor(26, 21);
      uView.circle(31, 24, 10);
      uView.circleFill(32, 25, 8, BLACK, NORM);
      uView.print(starCount);
      uView.display();
      delay(100);
      uView.clear(PAGE);
    }
  }
  
  if(starCount > 80) {
    uView.clear(PAGE);
    show_title();
  }
  else
    warp();
}

//
// init stars
//

void init_star() {
  for (int i = 0; i < STAR; i++) {    
    star_x[i] = random(-100, 100);
    star_y[i] = random(-100, 100);
    star_z[i] = random(100, 500);
    star_t[i] = random(0, 2);
  }
}

//
// draw warp
//

void warp() {
  for (int i = 0; i < starCount; i++) {

    uView.circleFill(int(star_screen_x[i]), int(star_screen_y[i]), star_t[i], BLACK, NORM);
    
    star_z[i] = star_z[i] - 5;

    star_screen_x[i] = star_x[i] / star_z[i] * 100 + X;
    star_screen_y[i] = star_y[i] / star_z[i] * 100 + Y;

    if (
        (star_screen_x[i] < 0)  ||
        (star_screen_x[i] > 63) || 
        (star_screen_y[i] < 0)  ||
        (star_screen_y[i] > 63) ||
        (star_z[i] < 1)      
       ) {
      star_x[i] = random(-100, 100);
      star_y[i] = random(-100, 100);
      star_z[i] = random(100, 500);
      if(random(0,10) > 8)
        star_t[i] = 1;
      else
        star_t[i] = 0;
    }

    uView.circleFill(int(star_screen_x[i]), int(star_screen_y[i]), star_t[i], WHITE, NORM);
  }
  uView.display();
}  

//
// show title
//

void show_title() {
  int limit = sizeof(bmp);

  for(int i = 0; i < limit; i+=2) {  
    uView.pixel(pgm_read_byte_near(bmp + i), pgm_read_byte_near(bmp + i + 1));
  }

  uView.setCursor(4, 32);
  uView.print("Warp V0.1");
  uView.setCursor(4, 40);
  uView.print("Starfield");
  uView.display();
}
