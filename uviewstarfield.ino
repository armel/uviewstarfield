#include <MicroView.h>
#include <avr/pgmspace.h>

#define X 32
#define Y 24
#define STAR 80

static const unsigned char bmp [] PROGMEM = {  // starfleet badge for fun
  0x1f, 0x02, 0x1e, 0x03, 0x20, 0x03, 0x1e, 0x04, 0x21, 0x04, 0x1d, 0x05, 0x21, 0x05, 0x1d, 0x06, 0x22, 0x06, 0x1c, 0x07, 0x22, 0x07, 0x1c, 0x08, 0x23, 0x08, 0x1c, 0x09, 0x23, 0x09, 
  0x1b, 0x0a, 0x24, 0x0a, 0x19, 0x0b, 0x1a, 0x0b, 0x1b, 0x0b, 0x24, 0x0b, 0x25, 0x0b, 0x26, 0x0b, 0x27, 0x0b, 0x17, 0x0c, 0x18, 0x0c, 0x1b, 0x0c, 0x24, 0x0c, 0x28, 0x0c, 0x29, 0x0c, 
  0x16, 0x0d, 0x1a, 0x0d, 0x25, 0x0d, 0x2a, 0x0d, 0x15, 0x0e, 0x1a, 0x0e, 0x25, 0x0e, 0x2b, 0x0e, 0x14, 0x0f, 0x1a, 0x0f, 0x25, 0x0f, 0x2c, 0x0f, 0x14, 0x10, 0x19, 0x10, 0x26, 0x10, 
  0x2c, 0x10, 0x14, 0x11, 0x19, 0x11, 0x26, 0x11, 0x2c, 0x11, 0x14, 0x12, 0x19, 0x12, 0x26, 0x12, 0x27, 0x12, 0x2c, 0x12, 0x15, 0x13, 0x18, 0x13, 0x27, 0x13, 0x2b, 0x13, 0x16, 0x14, 
  0x18, 0x14, 0x20, 0x14, 0x21, 0x14, 0x22, 0x14, 0x27, 0x14, 0x2a, 0x14, 0x17, 0x15, 0x18, 0x15, 0x1f, 0x15, 0x23, 0x15, 0x27, 0x15, 0x28, 0x15, 0x29, 0x15, 0x18, 0x16, 0x1e, 0x16, 
  0x24, 0x16, 0x28, 0x16, 0x18, 0x17, 0x1d, 0x17, 0x1e, 0x17, 0x1f, 0x17, 0x20, 0x17, 0x21, 0x17, 0x22, 0x17, 0x23, 0x17, 0x24, 0x17, 0x28, 0x17, 0x17, 0x18, 0x1c, 0x18, 0x25, 0x18, 
  0x28, 0x18, 0x17, 0x19, 0x1b, 0x19, 0x26, 0x19, 0x28, 0x19, 0x17, 0x1a, 0x1a, 0x1a, 0x27, 0x1a, 0x17, 0x1b, 0x19, 0x1b, 0x18, 0x1c };

int sensor_pin = A0;      // select the input pin for the potentiometer
int star_count = 10;
int star_value = 10;      // variable to store the value coming from the sensor
int star_value_old = 10;

byte cls = 1;             // need to clear screen or not

float star_x[STAR], star_y[STAR], star_z[STAR]; 
byte star_screen_x[STAR], star_screen_y[STAR], star_t[STAR];

void setup() {
  randomSeed(analogRead(sensor_pin));    // init random seed
  digitalWrite(sensor_pin, HIGH);        // internal pull up
  pinMode(sensor_pin, INPUT);            // make pin as input
  uView.begin();                         // start microview
  uView.clear(PAGE);                     // clear page
  
  for (int i = 0; i < STAR; i++) {       // init all stars
    initStar(i);
  }
  
  showTitle();                           // show title
  delay(1000);                           // wait 1 seconde
  uView.clear(PAGE);                     // and clear page again
}

void loop() {
  star_value = analogRead(sensor_pin);    // read sensorPin
  star_value = map(star_value, 0, 1023, 10, 85);
  if(abs(star_value - star_value_old) > 2) {
    star_value_old = star_value;
    star_count = star_value;
    
    if(star_count <= 80) {
      uView.setCursor(26, 21);
      uView.circle(31, 24, 10);
      uView.circleFill(32, 25, 8, BLACK, NORM);
      uView.print(star_count);
      uView.display();
      delay(100);
      uView.clear(PAGE);
    }
    
  }
  
  if(star_count > 80) {
    showTitle();
  }
  else {
    cls = 0;
    showStarfield();
  }
}

//
// init star
//

void initStar(int i) {
  star_x[i] = random(-100, 100);
  star_y[i] = random(-100, 100);
  star_z[i] = random(100, 500);
  if(random(0, 10) > 8)
    star_t[i] = 1;
  else
    star_t[i] = 0;
}

//
// show starfield
//

void showStarfield() {
  for (int i = 0; i < star_count; i++) {

    uView.circleFill(int(star_screen_x[i]), int(star_screen_y[i]), star_t[i], BLACK, NORM);
    
    star_z[i] = star_z[i] - 5;

    star_screen_x[i] = star_x[i] / star_z[i] * 100 + X;
    star_screen_y[i] = star_y[i] / star_z[i] * 100 + Y;

    if(
        (star_screen_x[i] < 0)  ||
        (star_screen_x[i] > 63) || 
        (star_screen_y[i] < 0)  ||
        (star_screen_y[i] > 63) ||
        (star_z[i] < 1)      
      ) {
      initStar(i);
    }

    uView.circleFill(int(star_screen_x[i]), int(star_screen_y[i]), star_t[i], WHITE, NORM);
  }
  uView.display();
}  

//
// show title
//

void showTitle() {
  int limit = sizeof(bmp);

  if(cls == 0)
    clearScreen();
  
  for(int i = 0; i < limit; i+=2) {  
    uView.pixel(pgm_read_byte_near(bmp + i), pgm_read_byte_near(bmp + i + 1));
  }

  uView.setCursor(2, 32);
  uView.print("uView v0.2");
  uView.setCursor(4, 40);
  uView.print("Starfield");
  uView.display();
  
  cls = 1;
}

//
// clear screen
//

void clearScreen() {
  for(int i = 0; i < Y; i++) {
    uView.lineH(0, i, 2*X, BLACK, NORM);
    uView.lineH(0, i + 1, 2*X, WHITE, NORM);
    uView.lineH(0, 2*Y - i, 2*X, BLACK, NORM);
    uView.lineH(0, 2*Y - i -1, 2*X, WHITE, NORM);
    uView.display();
    delay(25);
  }
  uView.clear(PAGE);                     // clear page  
} 

