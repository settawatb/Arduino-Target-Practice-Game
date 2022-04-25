//  Arduino Target Practice Game
//  6351000457 Settawat Buddhakanchana
//  ESP8266 SOURCE CODE / C,C++

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//----------------------------------------
const char *ssid = "XXXX";     //--> Name Wifi / SSID.
const char *password = "XXXX"; //-->  Password wifi .

const char *host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client;

String GAS_ID = "XXXX"; //-->Google Apps Script ID

void setup()
{
  Serial.begin(9600);
  delay(500);

  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  client.setInsecure();
}

void loop()
{
  if (Serial.available() > 0)
  {
    int score_int = Serial.parseInt(); //--> รับค่าจาก Serial print จากบอร์ด Arduino
    sendData(score_int);               //--> รันสคริป
    delay(20000);
  }
}

void sendData(int score_int) //--> Function รัน App Script
{
  if (!client.connect(host, httpsPort))
  {
    return;
  }

  char score_str[3];
  sprintf(score_str, "%d", score_int);

  String url = "/macros/s/" + GAS_ID + "/exec?score=" + score_str; //--> ส่งค่า score ไปเพื่อแอด score ลงใน Googlesheets

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      break;
    }
  }
  String line = client.readStringUntil('\n');
}