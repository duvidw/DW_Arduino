// David Weinstein 25 May, 2019
#include "ESP8266WiFi.h"
#include "String.h"

const char *ssid = "David_W_1";
const char *password = "genetrix1";

WiFiServer wifiServer(80);

String myCommand;
const int LEDPIN = LED_BUILTIN;
bool bLedBlink = false;
//bool LED_ST = HIGH;
//=======================================================
//=== led blink BY time Intercal ========================
//=======================================================
unsigned long previousMillis = 0; // will store last time LED was updated
int ledState = LOW;               // ledState used to set the LED
long intervalON = 500;
long intervalOFF = 200;
long intervalActive = intervalOFF;
//=======================================================
//=== Analog output =====================================
//=======================================================
int myPWM = GPIO_ID_PIN0;
//=======================================================
void LedBlink(int iPin)
{
  // update time
  unsigned long currentMillis = millis();
  // Check for blinking
  if (bLedBlink)
  {
    if (ledState == LOW)
    {
      intervalActive = intervalOFF;
    }
    else
    {
      intervalActive = intervalON;
    }
    if (currentMillis - previousMillis >= intervalActive)
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW)
      {
        ledState = HIGH;
      }
      else
      {
        ledState = LOW;
      }
      // set the LED with the ledState of the variable:
      digitalWrite(iPin, ledState);
    }
  }
  else
  {
    // Stay in state;
    digitalWrite(iPin, ledState);
  }
}
//=======================================================
String GetStringCommand(WiFiClient client)
{
  String sinp;
  char c = 0;
  while (client.available() > 0)
  {
    c = client.read();
    if (c == '\n' || c == '\r')
    {
      break;
    }
    sinp += c;
  }
  return sinp;
}
int GetCommandInt(WiFiClient client)
{
  String sinp;
  int iCommand;
  char c = 0;
  while (client.available() > 0)
  {
    c = client.read();
    if (c == '\n' || c == '\r')
    {
      break;
    }
    sinp += c;
  }
  iCommand = sinp.toInt();
  return iCommand;
}
int GetCommandAndValue(WiFiClient client, String& strVal)
{
  String strCmd;
  int iCommand = 0;
  // strCommand <int=string>
  String strInp = GetStringCommand(client);
  Serial.print(">>>");
  Serial.print(strInp);
  Serial.println("<<<");
  
  // if '=' exists, return the int and strVal fill with the string after the =
  int i1 = strInp.indexOf('=');
  if (i1 < 0)
  {
    iCommand = strInp.toInt();
  }
  else if (i1 == 0)
  {
    strVal = strInp.substring(i1 + 1);
  }
  else
  {
    strCmd = strInp.substring(0, i1);
    strVal = strInp.substring(i1 + 1);
    iCommand = strCmd.toInt();
  }
  return iCommand;
}
void setup()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(myPWM, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

  wifiServer.begin();

  Serial.println();
  Serial.print("ledpin = (");
  Serial.print(LEDPIN);
  Serial.print(") - ST=<");
  Serial.print(ledState);
  Serial.println(">");
}
void PrintLedSet(String strMsg)
{
  Serial.print(strMsg);
  Serial.print("ledpin = (");
  Serial.print(LEDPIN);
  Serial.print(") - ST=<");
  Serial.print(ledState);
  Serial.print(">");
  Serial.print("-- Blink={");
  Serial.print(bLedBlink);
  Serial.println("}");
}
void loop()
{
  LedBlink(LEDPIN);
  int iValOld = 0;
  int iValNew = 0;
  WiFiClient client = wifiServer.available();
  int iMyCommand = 0;
  if (client)
  {
    if (client.connected())
    {
      Serial.print("remoteIP:");
      Serial.println(client.remoteIP());
      Serial.print("remotePort :");
      Serial.println(client.remotePort());
    }
    while (client.connected())
    {
      LedBlink(LEDPIN);
      String strVal;
      String strMsg;
      while (client.available() > 0)
      {
        LedBlink(LEDPIN);
//        iMyCommand = GetCommandInt(client);
        iMyCommand = GetCommandAndValue(client, strVal);
        Serial.print("***Command=");
        Serial.print(iMyCommand);
        Serial.print(" val=<");
        Serial.print(strVal);
        Serial.println(">");
        switch (iMyCommand)
        {
        case 0:
          break;
        case 20:
          Serial.println("Led OFF");
          ledState = HIGH;
          bLedBlink = false;
          PrintLedSet("Command 20--");
          break;
        case 21:
          Serial.println("Led ON");
          ledState = LOW;
          bLedBlink = false;
          PrintLedSet("Command 21--");
          break;
        case 22:
          Serial.println("Led Blink");
          bLedBlink = true;
          PrintLedSet("Command 22--");
          break;
        case 1:
          client.println("Hi, David!");
          break;
        case 2:
          iValNew = strVal.toInt();
          iValOld = analogRead(myPWM);
          analogWrite(myPWM,iValNew);
          strMsg = "Change pwm from " + String(iValOld) + " to " + String(iValNew);
          client.println(strMsg);
          break;
        default:
          Serial.print("Unknown Command:<");
          Serial.print(iMyCommand);
          Serial.println(">");
          break;
        }
        if (iMyCommand == 99)
        {
          break;
        }
      }
      delay(10);
    }
    delay(10);
    client.stop();
    delay(10);
    Serial.println("Client disconnected");
  }
}
