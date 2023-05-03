#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <MCP23017.h>

const char* ssid = "URP_AP_2.4G";
//String server_name = "http://192.168.0.99:5000/";
String server_name = "http://192.168.0.98:5000/";
unsigned long delay_time = 500;
unsigned long last_time = 0;
uint8_t la;
uint8_t lb;

MCP23017 mcp = MCP23017(0x20);

void init_wifi()
{
  WiFi.begin(ssid);
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
}

String to_string(uint8_t bits)
{
  String command;
  for (int i = 0; i < 8; i++)
  {
    if (bits & 1 == 1)
    {
      command += "0";
    }
    else
    {
      command += "1";
    }
    bits >>= 1;
  }
  return command;
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  mcp.init();
  mcp.portMode(MCP23017Port::A, 0b11111111);
  mcp.portMode(MCP23017Port::B, 0b11111111);
  mcp.writeRegister(MCP23017Register::GPIO_A, 0x00);
  mcp.writeRegister(MCP23017Register::GPIO_B, 0x00);
  init_wifi();
  last_time = millis();
  uint8_t la = mcp.readPort(MCP23017Port::A) - 1;
  uint8_t lb = mcp.readPort(MCP23017Port::B) - 1;
}

void loop()
{
  if ((millis() - last_time) > delay_time)
  {
    if(WiFi.status()== WL_CONNECTED)
    {
      uint8_t na = mcp.readPort(MCP23017Port::A);
      uint8_t nb = mcp.readPort(MCP23017Port::B);
      if (na != la || nb != lb)
      {
        String command = to_string(na) + to_string(nb);
        Serial.print(command);
        Serial.print("\n");
        String server_path = server_name + command;
        HTTPClient http;
        http.begin(server_path.c_str());
        http.GET();
        http.end();
        la = na;
        lb = nb;
      }
    }
    else
    {
      Serial.print("WiFi disconnected\n");
      Serial.print("Reconnecting...\n");
      init_wifi();
    }
    last_time = millis();
  }
}
