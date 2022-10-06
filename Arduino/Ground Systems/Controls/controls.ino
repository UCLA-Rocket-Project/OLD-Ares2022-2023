#include <MCP23S17.h>

#define NVALVES 16
#define BUFFER 1000

int pins[NVALVES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

MCP23S17 io(&SPI, 10, 0);
char command[BUFFER];
int i = 0;

void setup()
{
  Serial.begin(9600);
  io.begin();
  for (int i = 0; i < NVALVES; i++)
  {
    io.pinMode(i, OUTPUT);
    io.digitalWrite(i, HIGH);
  }
}

void loop()
{
  if (i == BUFFER)
  {
    i = 0;
  }
  
  if (Serial.available() > 0)
  {
    command[i] = Serial.read();
    i++;
    
    if (command[i - 1] == '\n')
    {
      for (int j = 0; j < i; j++)
      {
        Serial.print(command[j]);
      }
      i--;

      if (i == NVALVES + 2 && command[0] == 'S' && command[i - 1] == 'E')
      {
        for (int j = 1; j < i - 1; j++)
        {
          if (command[j] == '1')
          {
            io.digitalWrite(i, LOW);
          }
          else
          {
            io.digitalWrite(i, HIGH);
          }
        }
      }
      i = 0;
    }
  }
}
