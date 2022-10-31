#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define NUM_TC 5

#define MAXCS_TC1   34
#define MAXCS_TC2   35
#define MAXCS_TC3   36
#define MAXCS_TC4   37
#define MAXCS_TC5   38

Adafruit_MAX31855 tc1(MAXCS_TC1, &SPI);
Adafruit_MAX31855 tc2(MAXCS_TC2, &SPI);
Adafruit_MAX31855 tc3(MAXCS_TC3, &SPI);
Adafruit_MAX31855 tc4(MAXCS_TC4, &SPI);
Adafruit_MAX31855 tc5(MAXCS_TC5, &SPI);

double current_temp[NUM_TC];

void setup() {
  Serial.begin(9600);

  while (!Serial) delay(1);

  // Wait for MAX chip to stabilize.
  delay(500);

  // Init MAX31855 breakout boards.
  if (!tc1.begin() || !tc2.begin() || !tc3.begin() || !tc4.begin() || !tc5.begin()) {
    while (1) delay(10);
  }
}

void loop() {

  // Poll temperature from MAX31855s.
  current_temp[0] = tc1.readFahrenheit();
  current_temp[1] = tc2.readFahrenheit();
  current_temp[2] = tc3.readFahrenheit();
  current_temp[3] = tc4.readFahrenheit();
  current_temp[4] = tc5.readFahrenheit();

  // Format output to serial.
  for (int i = 0; i < NUM_TC; i++) {
    if (isnan(current_temp[i]))
      Serial.print("null");
    else
      Serial.print(current_temp[i]);

    if (i < NUM_TC - 1) {
      Serial.print(",");
    }
  }
  
  Serial.println();
  delay(10);
}
