#include "HCMS39xx.h"
#include <NanoBLEFlashPrefs.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_H3LIS331.h>
#include <Adafruit_Sensor.h>


// The number of digits supported by the display 
#define DISPLAY_DIGITS 8
// The threshold for an impact to be counted as a strike, in g
#define STRIKE_ACC_THRESHOLD 20
// The threshold at which an impact is said to have ended, in g
#define RESET_ACC_THRESHOLD 20
// The number of cycles it will take to reset the impact 
#define RESET_CYCLES 5
// The threshold to trigger the backwards thrust gesture 
#define GESTURE_ACC_THRESHOLD 17.5
// The threshold to trigger counting of a suburi swing
#define SUBURI_FWD_ACC_THRESHOLD 5
#define SUBURI_OWD_ACC_THRESHOLD 3


Adafruit_H3LIS331 accelerometer = Adafruit_H3LIS331();

NanoBLEFlashPrefs flashPrefs;

typedef struct flashStruct {
  int strikeCount;
  int suburiCount;
} flashData; 

flashData data;

// HCMS39xx(uint8_t num_chars, uint8_t data_pin, uint8_t rs_pin, uint8_t clk_pin, uint8_t ce_pin)
HCMS39xx display(DISPLAY_DIGITS, 5, 6, 9, 10);  


void setup() {

  // Set up the wide-range accelerometer 
  Serial.begin(115200);
  
  if (!accelerometer.begin_I2C()) { 
    Serial.println("Couldn't start");
    while (1) yield();
  }
  accelerometer.setRange(H3LIS331_RANGE_100_G); 
  accelerometer.setDataRate(LIS331_DATARATE_400_HZ);

  // Set up the flash storage 
  int rc = flashPrefs.readPrefs(&data, sizeof(data));

  // Set up the dot-matrix display 
  display.begin();
  display.clear();
  display.print(data.strikeCount);
}


typedef struct sensorStruct {
  float accVertical;
  float accLateral;
  float accLongitudinal;

  float dpsVertical;
  float dpsLateral;
  float dpsLongitudinal;
} sensorData; 

sensorData (*sensorFunction)() = &externalSensor;

void (*modeFunction)(float, float, float) = &keikoModeActive;

int resetCycleCount = 0; 

unsigned long lastCycleTime = millis(); 


void loop() {

  accelerometer.read(); 

  sensors_event_t event;
  accelerometer.getEvent(&event);
  
  float accVertical = event.acceleration.y / SENSORS_GRAVITY_STANDARD;
  float accLateral = event.acceleration.x / SENSORS_GRAVITY_STANDARD;
  float accLongitudinal = event.acceleration.z / SENSORS_GRAVITY_STANDARD;

  unsigned long thisCycleTime = millis(); 
  unsigned long cycleDuration = thisCycleTime - lastCycleTime; 
  lastCycleTime = thisCycleTime; 

  accVertical = abs(accVertical);
  accLateral = abs(accLateral);
  accLongitudinal *= -1;

  Serial.print(accVertical);
  Serial.print('\t');
  Serial.print(accLateral);
  Serial.print('\t');
  Serial.print(accLongitudinal);
  Serial.print('\t');
  Serial.print(cycleDuration);
  Serial.println();

  if (accLongitudinal > GESTURE_ACC_THRESHOLD && accLongitudinal > accVertical && accLongitudinal > accLateral) {

    display.clear();

    if (modeFunction == &keikoModeActive || modeFunction == &keikoModeCooldown) {
      
      modeFunction = &suburiMode;
      resetCycleCount = 0;
      
      display.print("Suburi  ");

      delay(2000);

      display.clear();
      display.print(data.suburiCount);

    } else {

      modeFunction = &keikoModeActive;
      
      display.print("Keiko   ");

      delay(2000);

      display.clear();
      display.print(data.strikeCount);
    }

    return;
  }

  (*modeFunction)(accVertical, accLateral, accLongitudinal);

}


sensorData externalSensor() {

}


sensorData internalSensor() {
  
}


void keikoModeActive(float accVertical, float accLateral, float accLongitudinal) {
  
  // If the forward acceleration is greater than the strike threshold and the orthogonal acceleration, log a strike 
  if (accVertical > STRIKE_ACC_THRESHOLD && accVertical > accLateral && accVertical > abs(accLongitudinal)) {

    // Increment the strike count and print it to the display
    display.clear();
    display.print(++(data.strikeCount));

    // Write the new impact count to the flash memory 
    flashPrefs.writePrefs(&data, sizeof(data));

    // Throw the strike flag 
    modeFunction = &keikoModeCooldown; 
  }
  
}


void keikoModeCooldown(float accVertical, float accLateral, float accLongitudinal) {

  // Check if the current forward acceleration is under the reset threshold
  if (accVertical < RESET_ACC_THRESHOLD) {

    // Check if we've been under the reset threshold for enough cycles to reset the strike flag 
    if (resetCycleCount < RESET_CYCLES) {

      // The last strike is over but we're still cooling down; count another cycle wihtout a strike 
      resetCycleCount++; 
      
    } else {

      // The last strike ended enough cycles ago that we can reset the cycle counter and strike flag 
      resetCycleCount = 0; 
      
      modeFunction = &keikoModeActive;
    }
    
  } else {

    // The current forward acceleration indicates that a strike is still underway; reset the cycle counter 
    resetCycleCount = 0;
  }

}


void suburiMode(float accVertical, float accLateral, float accLongitudinal) {
  
  if (accVertical > SUBURI_FWD_ACC_THRESHOLD && abs(accLongitudinal) > SUBURI_OWD_ACC_THRESHOLD && accVertical > accLateral && accVertical > abs(accLongitudinal)) {

    // Increment the strike count and print it to the display
    display.clear();
    display.print(++(data.suburiCount));

    // Write the new impact count to the flash memory 
    flashPrefs.writePrefs(&data, sizeof(data));
  }

}