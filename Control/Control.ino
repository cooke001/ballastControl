/*******************************************************************
    A sketch for controlling a toy car with using a web page 
    hosted on a ESP8266
    
    Main Hardware:
    - NodeMCU Development Board cp2102 (Look for the one with the square chip beside the USB port)
    - NodeMCU Motor Shield (L2932)
    
    Written by Brian Lough
    https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA
 *******************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define up D1 // Motor B
#define down D2

const char* ssid = "boaty24";
const char* password = "drayleighKake";

ESP8266WebServer server(80);

const char *webpage = 
#include "motorPage.h"
;

void handleRoot() {

  server.send(200, "text/html", webpage);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){

  pinMode(up, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(down, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("control")) {
    Serial.println("MDNS Responder Started");
  }

  server.on("/", handleRoot);

  server.on("/up", [](){
    Serial.println("up");
    digitalWrite(up, HIGH);
    server.send(200, "text/plain", "up");
  });

  server.on("/driveStop", [](){
    Serial.println("driveStop");
    digitalWrite(up, LOW);
    digitalWrite(down, LOW);
    server.send(200, "text/plain", "driveStop");
  });

  server.on("/down", [](){
    Serial.println("down");
    digitalWrite(down, HIGH);
    server.send(200, "text/plain", "down");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP Server Started");
}

void loop(void){
  server.handleClient();
}
