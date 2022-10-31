#include <MCP23S17.h>

#define NVALVES 16
#define BUFFER 1000

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
        modify_command(command);
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

void modify_command(char* a)
{
  if (a[0] == '1')
    a[0] = '0';

  if (a[9] == '1')
    a[0] = '1';
  else if (a[9] == '0')
    a[0] = '0';
}
