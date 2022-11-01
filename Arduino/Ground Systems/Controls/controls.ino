#include <MCP23S17.h> // MCP23S17 by Rob Tillaart (https://github.com/RobTillaart/MCP23S17)

#define NVALVES 16
#define BUFFER 1000

MCP23S17 io(10, 0, &SPI);
char command[BUFFER];
int i = 0;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  io.begin();
  io.pinMode8(0, 0);
  io.pinMode8(1, 0);
  for (int i = 0; i < NVALVES; i++)
  {
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
            io.digitalWrite(j-1, 0);
          }
          else
          {
            io.digitalWrite(j-1, 1);
          }
        }
      }
      i = 0;
    }
  }
}