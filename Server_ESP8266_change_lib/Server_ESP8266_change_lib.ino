//this code for `ESP8266 
//Change Lib 
//Step to Start 
//Chose Broad NodeMCU1.0 
//ESP-12E Module
//upload speed `115200
//Flash Size 4mb


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'


////wemos lolin32 115200 mapping broad oae wmmos oled
//#include <WiFi.h>
//#include <WebServer.h>
//#include <ESPmDNS.h>
//
//#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
///* Put your SSID & Password */
const char* ssid = "ratexx_phone";  // Enter SSID here
const char* password = "pongpong";  //Enter Password here

/* Put IP Address details */
//IPAddress local_ip(10, 0, 0, 8);
//IPAddress gateway(10, 0, 0, 1);
//IPAddress subnet(255, 0, 0, 0);


#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;
ESP8266WebServer server(80);

uint8_t LED1pin = 12;//pin 6
bool LED1status = 0;

uint8_t LED2pin = 13;//pin 7
bool LED2status = 0;

uint8_t LED3pin = 15;//pin 8
bool LED3status = 0;

void StartDNS() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  wifiMulti.addAP(ssid, password);   // add Wi-Fi networks you want to connect to
  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (!MDNS.begin("esp8266")) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
}


void setup() {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(LED3pin, OUTPUT);
//  WiFiClient.softAP(ssid, password);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  StartDNS();
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.on("/led3on", handle_led3on);
  server.on("/led3off", handle_led3off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

  
}

void loop() {
  server.handleClient();
  if (LED1status)
  {
    digitalWrite(LED1pin, HIGH);
  }
  else
  {
    digitalWrite(LED1pin, 0);
  }

  if (LED2status)
  {
    digitalWrite(LED2pin, HIGH);
  }
  else
  {
    digitalWrite(LED2pin, 0);
  }

  if (LED3status)
  {
    digitalWrite(LED3pin, HIGH);
  }
  else
  {
    digitalWrite(LED3pin, 0);
  }





  
}

void handle_OnConnect() {
  LED1status = 0;
  LED2status = 0;
  LED3status = 0;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status));
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true, LED2status, LED3status));
}

void handle_led1off() {
  LED1status = 0;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false, LED2status, LED3status));
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, true, LED3status));
}

void handle_led2off() {
  LED2status = 0;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, false, LED3status));
}

void handle_led3on() {
  LED3status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, true));
}

void handle_led3off() {
  LED3status = 0;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, false));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat, uint8_t led2stat, uint8_t led3stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Server</h1>\n";
  ptr += "<h3>Using Access Point(AP) Mode</h3>\n";

  if (led1stat)
  {
    ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }

  if (led2stat)
  {
    ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";
  }


  if (led3stat)
  {
    ptr += "<p>LED3 Status: ON</p><a class=\"button button-off\" href=\"/led3off\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED3 Status: OFF</p><a class=\"button button-on\" href=\"/led3on\">ON</a>\n";
  }


  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
