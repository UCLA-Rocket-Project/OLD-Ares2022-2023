#include <Wire.h>
#include <SPI.h>
#include <RadioLib.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "SdFat.h"


#define spiPort SPI
#define NUM_DIGITS 7
#define BMP_CS 2
#define SEALEVELPRESSURE_HPA (1013.25)
#define SD_CONFIG  SdioConfig(FIFO_SDIO)
#define PACKET_SIZE 16
#define DATA_SIZE 42
#define BUFFER_MAX 12
#define METER_FOOT 3.28084


SX1276 radio = new Module(15, 34, 33, 35);
SFE_UBLOX_GNSS myGNSS;
Adafruit_BMP3XX bmp;
const uint8_t csPin = 25;
SdExFat sd;
ExFile file;
unsigned int last_gps;
unsigned int last_radio;
unsigned int time_stamp;
int long_latitude;
int long_longitude;
unsigned int latitude;
unsigned int longitude;
int gps_altitude;
unsigned short siv;
unsigned int buffer_size;
double temperature;
double pressure;
double bmp_altitude;
unsigned short short_altitude;


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

int encode(unsigned short x, uint8_t* e, int s) {
  e[s] = (uint8_t)(x >>  0);
  e[s + 1] = (uint8_t)(x >>  8);
  return s + 2;
}

unsigned int truncate(long coordinate) {
  unsigned int t = 0;
  unsigned int digits[NUM_DIGITS];
  for (int i = NUM_DIGITS - 1; i >= 0; i--) {
    if (coordinate < 0) {
      digits[i] = -(coordinate % 10);
    }
    else {
      digits[i] = coordinate % 10;
    }
    coordinate /= 10;
  }

  for (int i = 0; i < NUM_DIGITS; i++) {
    t = (t * 10) + digits[i];
  }

  return t;
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
  
  Serial.begin(115200);

  buffer_size = 0;
  
  int state = radio.begin(915.0,500.0,12,5,RADIOLIB_SX127X_SYNC_WORD,20,6,0); //-23dBm
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  if (radio.setOutputPower(20) == -13) {
    Serial.println(F("Selected output power is invalid for this module!")); 
    while (true);
  }
  
  radio.setRfSwitchPins(40, 41);

  last_radio = millis();

  spiPort.begin();
  if (myGNSS.begin(spiPort, csPin, 4000000) == false) 
  {
    Serial.println(F("u-blox GNSS not detected on SPI bus. Please check wiring. Freezing."));
    while (true);
  }

  myGNSS.setPortOutput(COM_PORT_SPI, COM_TYPE_UBX);
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT);
  last_gps = millis();
  
  if (!sd.begin(SdioConfig(FIFO_SDIO))) {
    Serial.println(F("Error starting SD"));
    while (true);
  }

  if(!create_file()) {
    Serial.println(F("Error creating file"));
    while (true);
  }
  
  if (! bmp.begin_SPI(BMP_CS)) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (true);
  }

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void loop() {
  uint8_t e[PACKET_SIZE];
  uint8_t f[DATA_SIZE];
  bool new_data = false;

  time_stamp = millis();
  
  if (time_stamp - last_gps > 1000) {
    long_latitude = myGNSS.getLatitude();
    long_longitude = myGNSS.getLongitude();
    latitude = truncate(long_latitude);
    longitude = truncate(long_longitude);
    gps_altitude = myGNSS.getAltitude();
    siv = myGNSS.getSIV();
    last_gps = time_stamp;
    new_data = true;
  }

  if (bmp.performReading()) {
    temperature = bmp.temperature;
    pressure = bmp.pressure / 100.0;
    bmp_altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) * METER_FOOT;
    if (bmp_altitude < 0.0) {
      short_altitude = 0;
    }
    else {
      short_altitude = (unsigned short) bmp_altitude;
    }
    new_data = true;
  }
  
  encode(time_stamp, e, 0);
  encode(latitude, e, 4);
  encode(longitude, e, 8);
  encode(short_altitude, e, 12);
  encode(siv, e, 14);
  
  encode(time_stamp, f, 0);
  encode(long_latitude, f, 4);
  encode(long_longitude, f, 8);
  encode(gps_altitude, f, 12);
  encode(siv, f, 16);
  encode(temperature, f, 18);
  encode(pressure, f, 26);
  encode(bmp_altitude, f, 34);
  
  if (time_stamp - last_radio > 500) {
    int state = radio.transmit(e, PACKET_SIZE);
    if (!(state == RADIOLIB_ERR_NONE || state == RADIOLIB_ERR_TX_TIMEOUT)) {
      Serial.print(F("failed, code "));
      Serial.println(state);
    }
    last_radio = time_stamp;
  }
  
  if (new_data) {
    file.write(f, DATA_SIZE);
    buffer_size++;
  }

  if (buffer_size == BUFFER_MAX) {
    file.sync();
    buffer_size = 0;
  }
}
