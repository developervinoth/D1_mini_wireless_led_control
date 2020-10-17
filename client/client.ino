#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <U8g2lib.h>
int ledPin=2;
//U8g2 Constructor List - https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#introduction
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4);

const char *ssid = "Your WiFI SSID here";
const char *password = "YOur Wifi Password";

ESP8266WebServer server(80);

void handleSentVar() {
  if (server.hasArg("sensor_reading")) { // this is the variable sent from the client
    int readingInt = server.arg("sensor_reading").toInt();
    Serial.println(readingInt);
    analogWrite(ledPin,readingInt);
    char readingToPrint[5];
    itoa(readingInt, readingToPrint, 10); //integer to string conversion for OLED library
    u8g2.firstPage();
    u8g2.drawUTF8(0, 64, readingToPrint);
    u8g2.nextPage();
    server.send(200, "text/html", "Data received");
  }
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  u8g2.begin();
  u8g2.setFont(u8g2_font_logisoso62_tn);
  u8g2.setFontMode(0);    // enable transparent mode, which is faster

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();
}

void loop() {
  server.handleClient();
}
