#include <RadioLib.h>
#include "SdFat.h"


#define SD_CONFIG  SdioConfig(FIFO_SDIO)
#define PACKET_SIZE 16
#define BUFFER_MAX 32


SX1276 radio = new Module(10, 1, 0, 2);
SdExFat sd;
ExFile file;
unsigned int buffer_size;


unsigned int decode_int(uint8_t* e, int s)
{
  return (uint32_t)e[s] <<  0 | (uint32_t)e[s + 1] <<  8 | (uint32_t)e[s + 2] << 16 | (uint32_t)e[s + 3] << 24;
}


unsigned short decode_short(uint8_t* e, int s)
{
  return (uint16_t)e[s] <<  0 | (uint16_t)e[s + 1] <<  8;
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

  int state = radio.begin(915.0,500.0, 12, 5,RADIOLIB_SX127X_SYNC_WORD,20,6,0);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
  
  if (radio.setOutputPower(20) == -13) {
   Serial.println(F("Selected output power is invalid for this module!"));
   while (true);
  }
  
  radio.setRfSwitchPins(8, 9);

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
  uint8_t e[PACKET_SIZE];
  String str;
  int state = radio.receive(e, PACKET_SIZE);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.print(decode_int(e, 0));
    Serial.print(",");
    Serial.print(decode_int(e, 4));
    Serial.print(",");
    Serial.print(decode_int(e, 8));
    Serial.print(",");
    Serial.print(decode_short(e, 12));
    Serial.print(",");
    Serial.println(decode_short(e, 14));
    file.write(e, PACKET_SIZE);
    buffer_size++;

    if (buffer_size == 32) {
      file.sync();
      buffer_size = 0;
    }
  }
  else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println("Timeout!");
  }
  else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));
  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }
}
