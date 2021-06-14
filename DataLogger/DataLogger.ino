#include <MS5837.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

MS5837 sensor;
//SSID and Password of your WiFi router
const char* ssid = "boaty24";
const char* password = "drayleighKake";
#define LED 2      //On board LED
int dist=2;
int a,b;
float depth = 69;
ESP8266WebServer server(80); //Server on port 80
const char MAIN_page[] PROGMEM = R"=====(
<!doctype html>
<html>
<head>
  <title>Data Logger</title>
  <h1 style="text-align:center; color:red;">Iot Design Pro</h1>
  <h3 style="text-align:center;">NodeMCU Data Logger</h3>
  <style>
  canvas{
    -moz-user-select: none;
    -webkit-user-select: none;
    -ms-user-select: none;
  }
  /* Data Table Styling*/ 
  #dataTable {
    font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
    border-collapse: collapse;
    width: 100%;
    text-align: center;
  }
  #dataTable td, #dataTable th {
    border: 1px solid #ddd;
    padding: 8px;
  }
  #dataTable tr:nth-child(even){background-color: #f2f2f2;}
  #dataTable tr:hover {background-color: #ddd;}
  #dataTable th {
    padding-top: 12px;
    padding-bottom: 12px;
    text-align: center;
    background-color: #050505;
    color: white;
  }
  </style>
</head>
<body>
<div>
  <table id="dataTable">
    <tr><th>Time</th><th>Ballast Distance (Steps)</th><th>Depth (m)</th></tr>
  </table>
</div>
<br>
<br>  
<script>
var Tvalues = [];
var Hvalues = [];
var timeStamp = [];
setInterval(function() {
  // Call a function repetatively with 5 Second interval
  getData();
}, 1000); //5000mSeconds update rate
 function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
     //Push the data in array
  var time = new Date().toLocaleTimeString();
  var txt = this.responseText;
  var obj = JSON.parse(txt); 
      Tvalues.push(obj.Temperature);
      Hvalues.push(obj.Humidity);
      timeStamp.push(time);
  //Update Data Table
    var table = document.getElementById("dataTable");
    var row = table.insertRow(1); //Add after headings
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    var cell3 = row.insertCell(2);
    cell1.innerHTML = time;
    cell2.innerHTML = obj.Temperature;
    cell3.innerHTML = obj.Humidity;
    }
  };
  xhttp.open("GET", "readData", true); //Handle readData server on ESP8266
  xhttp.send();
}
</script>
</body>
</html>

)=====";
 

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void readData() {

 String data = "{\"Temperature\":\""+ String(dist) +"\", \"Humidity\":\""+ String(depth) +"\"}";

 server.send(200, "text/plane", data); //Send ADC value, temperature and humidity JSON to client ajax request
 delay(500);
   int available = Wire.requestFrom(8,2);
 a = Wire.read();
 b = Wire.read();
 dist = a << 8 | b; 
 sensor.read();
 depth = sensor.depth()+0.2;
 Serial.print(dist);
 Serial.print(",");
 Serial.println(depth);
 
}

void setup ()
{
  Serial.begin(115200);
  
  Wire.begin();

while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
 
  sensor.setModel(MS5837::MS5837_30BA);
 sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
 
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/readData", readData); //This page is called by java Script AJAX
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop(void){
   digitalWrite(LED,LOW); //Toggle LED on data request ajax
  server.handleClient();          //Handle client requests
}
 
