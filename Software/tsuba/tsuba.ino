#include "HCMS39xx.h"
#include "font5x7.h"  // This is only needed for example 15 below
#include "string.h"
#include <Arduino_LSM9DS1.h>

#define MAXLEN 20
uint8_t displaydata[MAXLEN];

#define DISPLAY_DIGITS 8
#define DISPLAY_STRING_LEN 9

#define FIRST_SCREEN_DELAY 500
#define SCROLL_DELAY 175
#define END_SCREEN_DELAY 2000

#define STRIKE_CUTOFF 3.9 

// HCMS39xx(uint8_t num_chars, uint8_t data_pin, uint8_t rs_pin, uint8_t clk_pin, uint8_t ce_pin)
HCMS39xx myDisplay(DISPLAY_DIGITS, 5, 6, 9, 10);  

int impactCount = 0;

void setup() {

  myDisplay.begin();

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  
  if (IMU.accelerationAvailable()) {

    float x, y, z;
    
    IMU.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);

    x = abs(x);
    y = abs(y);
    z = abs(z);

    if (x > STRIKE_CUTOFF || y > STRIKE_CUTOFF || z > STRIKE_CUTOFF) {
      
      char greatest; 
      float acc; 

      bool x_y = x > y; 
      bool y_z = y > z; 
      bool z_x = z > x;       

      if (x_y && !z_x) {

        greatest = 'x'; 
        acc = x;

      } else if (y_z && !x_y) {
        
        greatest = 'y'; 
        acc = y;

      } else {

        greatest = 'z'; 
        acc = z;
      }

      char printArr[DISPLAY_STRING_LEN];

      sprintf(printArr, "%c: %f", greatest, acc);

      displayPrint(printArr);
    }

  }

}

void displayPrint(char* input) {

  myDisplay.clear();

  myDisplay.print(input);
}

void scrollPrint(char* input) {

  myDisplay.clear();

  char firstScreen[DISPLAY_STRING_LEN]; 
  memset(firstScreen, '\0', sizeof(firstScreen));

  strncpy(firstScreen, input, DISPLAY_STRING_LEN);

  myDisplay.print(firstScreen); 

  if (firstScreen[DISPLAY_STRING_LEN - 1] == '\0') {
    
    delay(END_SCREEN_DELAY); 

    return;
  }

  delay(FIRST_SCREEN_DELAY); 

  for (int i = DISPLAY_STRING_LEN - 1; input[i] != '\0'; i++) {

    delay(SCROLL_DELAY);

    char printArr[] = {input[i], '\0'}; 

    myDisplay.print(printArr);

  }

  delay(END_SCREEN_DELAY);
}