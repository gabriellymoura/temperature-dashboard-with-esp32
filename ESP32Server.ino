
// Import required libraries
#include <FirebaseESP32.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define FIREBASE_HOST "*****************" //firebase info
#define FIREBASE_AUTH "**************************" //firebase info

const char* ssid = "********"; //wifi info
const char* password = "*************"; //wifi info
AsyncWebServer server(80);


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const long gmtOffset_sec = -3 * 60 * 60;


FirebaseData firebaseData;
FirebaseJson json;
String epochTime;
String path;

int DS18B20 = 23; 
OneWire ourWire(DS18B20);
DallasTemperature sensors(&ourWire);
                            
float t;
char array[4];

String processor(const String& var){

  if(var == "TEMP_VALUE"){
    sensors.requestTemperatures();
    t = sensors.getTempCByIndex(0);
    dtostrf(t, 4, 2, array);
    return array;

  }
//   Serial.print(var);    
//   Serial.print(": ");  
//   Serial.println(t);

  return String(t);
}

// String readBME280Temperature() {
//   // Read temperature as Celsius (the default)
//     sensors.requestTemperatures();
//     t = sensors.getTempCByIndex(0);
//   // Convert temperature to Fahrenheit
//   //t = 1.8 * t + 32;
//   if (isnan(t)) {
//     Serial.println("Failed to read from sensor!");
//     return "";
//   }
//   else {
//     Serial.println(t);
//     return String(t);
//   }
// }
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(DS18B20, INPUT);
  
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });
  server.on("/temperaturetwo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", String(t).c_str());
  });
  server.begin();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop(){
  sensors.requestTemperatures();
  t = sensors.getTempCByIndex(0);
    
  epochTime = timeClient.getEpochTime();
  path = "/data/T" + epochTime;

   json.clear();
   json.add("temperature", t);

   if (Firebase.setJSON(firebaseData, path, json)) {
      Serial.print("SEND TEMP WITH SUCCESS: ");
      Serial.println(t);
      // pause 5 minutes
      delay(1000);
        
    } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }

//  Firebase.setFloat( fbdo,"temperature", t);
  
}
