#include <SPI.h>
#include "Communication.h"
#include "adxl372.h"
#include "SdFat.h"


#define CS_AC 10
#define ADXL_INT1_PIN 6
#define SD_CONFIG  SdioConfig(FIFO_SDIO)
#define DATA_SIZE 16
#define BUFFER_MAX 32


unsigned char devId;
struct adxl372_device adxl372;
AccelTriplet_t accel_data;
SdExFat sd;
ExFile file;
unsigned int buffer_size;


void setupImpactDetection() {
  // most comments by me, pieced together from the datasheet
  // software documentation couldn't be worse if they tried
  adxl372_Set_Op_mode(&adxl372, STAND_BY); // turns it off while we set settings
  adxl372_Set_Autosleep(&adxl372, true); // do autosleep
  adxl372_Set_BandWidth(&adxl372, BW_3200Hz); // full sample rate
  adxl372_Set_ODR(&adxl372, ODR_6400Hz); // double Output Data Rate to avoid aliasing(?)
  adxl372_Set_WakeUp_Rate(&adxl372, WUR_52ms); // fastest wakeup rate available
  adxl372_Set_Act_Proc_Mode(&adxl372, LOOPED); // ???. Options are LINKED and LOOPED
  // /* and */ comments from here are from AD
  /* Set Instant On threshold */
  adxl372_Set_InstaOn_Thresh(&adxl372, ADXL_INSTAON_LOW_THRESH); //Low threshold 10-15 G
  /*Put fifo in Peak Detect and Stream Mode */
  adxl372_Configure_FIFO(&adxl372, 512, STREAMED, XYZ_PEAK_FIFO); 
  
  /* Set activity/inactivity threshold */
  adxl372_Set_Activity_Threshold(&adxl372, ACT_VALUE, true, true);
  adxl372_Set_Inactivity_Threshold(&adxl372, INACT_VALUE, true, true);
  
  /* Set activity/inactivity time settings */
  adxl372_Set_Activity_Time(&adxl372, ACT_TIMER);
  adxl372_Set_Inactivity_Time(&adxl372, INACT_TIMER);
  
  /* Set instant-on interrupts and activity interrupts */
   adxl372_Set_Interrupts(&adxl372);
  
  /* Set filter settle time */
  adxl372_Set_Filter_Settle(&adxl372, FILTER_SETTLE_370);

  //added this section based on some stuff form adxl's site:
  adxl372_Set_Op_mode(&adxl372, FULL_BW_MEASUREMENT);
//  delay(20);
//  
//  /* Set operation mode to Instant-On */
//  adxl372_Set_Op_mode(&adxl372, INSTANT_ON);
}

void setupAccel() {
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  delay(500);
  pinMode(CS_AC, OUTPUT);
  pinMode(ADXL_INT1_PIN, INPUT);
  pinMode(ADXL_INT2_PIN, INPUT);

  adxl372_Get_DevID(&adxl372, &devId);
  setupImpactDetection();
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

int encode(float x, uint8_t* e, int s) {
  uint8_t* d = (uint8_t*) &x;
  for (int i = 0; i < sizeof(float); i++) {
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


void setup() {
  
  setupAccel();

  Serial.begin(115200);

  buffer_size = 0;

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
  uint8_t e[DATA_SIZE];

  adxl372_Get_Accel_data(&adxl372, &accel_data);
  
  unsigned int t = micros();
  float x = (float)accel_data.x;  // * 100.0 / 1000.0;
  float y = (float)accel_data.y;  // * 100.0 / 1000.0;
  float z = (float)accel_data.z;  // * 100.0 / 1000.0;
  
  encode(t, e, 0);
  encode(x, e, 4);
  encode(y, e, 8);
  encode(z, e, 12);

  file.write(e, DATA_SIZE);
  buffer_size++;
  
  if (buffer_size == BUFFER_MAX) {
    file.sync();
    buffer_size = 0;
  }
}
