// Plot LM35 data on thingspeak.com using an ESP8266 (Linknode D1)
// Icludes OTA (Over The Air) in order to update this program 
// through WIFI (no USB cable needed).
// By Jorge Munoz
// email: jorgeroden1@gmail.com
// Jan 2017 License under GPL v3
// ESP8266 -> http://linksprite.com/wiki/index.php5?title=LinkNode_D1
// LM35 -> https://www.osepp.com/electronic-modules/sensor-modules/79-lm35-temperature-sensor-module
// Connections (Sensor = Board) : LM35 Vcc = 3.3V, GND , S = A0

#include <ESP8266WiFi.h>
#include <ThingSpeak.h> 
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
 
const char* myAPIKey = "XXXXX";
unsigned long myChannel = 207881;
int fieldNumber = 1;
const char* ssid = "XXXXXX";
const char* password = "XXXXXXXX";
const char* server = "api.thingspeak.com";

WiFiClient client;
    
void setup() {   
               
  Serial.begin(115200);
  delay(10);
  
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // OTA (Load program via WIFI)
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  ThingSpeak.begin(client);
}

void loop() {
  ArduinoOTA.handle(); 
  int reading = analogRead(A0);
  // Linear regression with "some" measured data (see below):
  float voltage = 3.266e-3 * float(reading) + 0.034; 
  float tempC = voltage * 100.0;
  ThingSpeak.writeField(myChannel, fieldNumber, tempC, myAPIKey);
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" Celsius"); 
  Serial.println(" sent to Thingspeak");    
  Serial.println("Next data in 5 minutes...");    
  delay(50000);  
}
/* Data gathered for set up a formula (Temp vs A0) 
 *  by a linear regression. Should have been more... The more data you get 
 * the more precise will be the whole system. Put this data on a Google spreadsheet,
 * draw a scatter chart and get the regression equation: Voltage = 3.266e-3* A0 + 0.034 (R2 = 0.95)
 * This example : https://docs.google.com/spreadsheets/d/1OXbEm3h9tPYfmaF-JNMPKjYKyHc15wS1GgpY4dgYMFM/edit?usp=sharing
analogRead(A0)  Voltage Signal LM35 (voltmeter)
60  0.23
62  0.24
65  0.25
68  0.26
70  0.27
74  0.28
75  0.28
77  0.29
78  0.29
79  0.3
72  0.27
71  0.27
70  0.26
66  0.25
65  0.24
64  0.23
62  0.2
44  0.18
39  0.16
39  0.17
46  0.19
50  0.2
51  0.2
53  0.21
59  0.23
*/
