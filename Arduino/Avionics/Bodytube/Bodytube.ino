#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "SdFat.h"


#define BMP_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
#define SD_CONFIG  SdioConfig(FIFO_SDIO)
#define BUFFER_MAX 16
#define CHAMBER_PIN A9
#define ENCODED_FILE_SIZE 32


SdExFat sd;
ExFile file;
Adafruit_BMP3XX bmp;
double temperature;
double pressure;
double altitude;
int reading;
unsigned int buffer_size;
unsigned int time_stamp;
uint8_t e[ENCODED_FILE_SIZE];


int encode(double x, uint8_t* e, int s) {
  uint8_t* d = (uint8_t*) &x;
  for (int i = 0; i < sizeof(double); i++) {
    e[s + i] = d[i];
  }
  return s + 8;
}

int encode(unsigned int x, uint8_t* e, int s) {
  e[s] = (uint8_t)(x >>  0);
  e[s + 1] = (uint8_t)(x >>  8);
  e[s + 2] = (uint8_t)(x >> 16);
  e[s + 3] = (uint8_t)(x >> 24);
  return s + 4;
}

int encode(int x, uint8_t* e, int s) {
  e[s] = (uint8_t)(x >>  0);
  e[s + 1] = (uint8_t)(x >>  8);
  e[s + 2] = (uint8_t)(x >> 16);
  e[s + 3] = (uint8_t)(x >> 24);
  return s + 4;
}

bool create_file() {
  long n = 0;
  String file_name = String(n) + ".txt";
  while (sd.exists(file_name))
  {
    n++;
    file_name = String(n) + ".txt";
  }
  return file.open(file_name.c_str(), O_WRITE | O_CREAT);
}


void setup() {
  delay(1000);

  buffer_size = 0;
  temperature = 0.0;
  pressure = 0.0;
  altitude = 0.0;

  if (!bmp.begin_SPI(BMP_CS)) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (true);
  }

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  
  if (!sd.begin(SdioConfig(FIFO_SDIO))) {
    Serial.println(F("Error starting SD"));
    while (true);
  }

  if(!create_file()) {
    Serial.println(F("Error creating file"));
    while (true);
  }
}

void loop() {
  time_stamp = millis();

  reading = analogRead(CHAMBER_PIN);
  
  if (bmp.performReading()) {
    temperature = bmp.temperature;
    pressure = bmp.pressure;
    altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  }

  encode(time_stamp, e, 0);
  encode(reading, e, 4);
  encode(temperature, e, 8);
  encode(pressure, e, 16);
  encode(altitude, e, 24);

  file.write(e, ENCODED_FILE_SIZE);
  buffer_size++;

  if (buffer_size == BUFFER_MAX) {
    file.sync();
    buffer_size = 0;
  }
}
