#include <Arduino.h>
#include <LittleFS.h>

#include <AsyncJson.h>
#include <ArduinoJson.h>

#include <ESP8266mDNS.h> 
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "ssid";
const char* password = "password";

const int OFF = 0, ON = 1;

volatile int state = ON, //tracks if lights are on or not
             mode_active[] = {OFF, OFF, ON, ON}; // default config
                             //0 for wL1
                             //1 for wL2
                             //2 for yL
                             //3 for fan
                             
int pin[] = {15, 13 , 12, 14};                             

AsyncWebServer server(80);


StaticJsonDocument<200> root;

void lightOn() {
  Serial.println("Turn Active On");
  for(int y=0;y<4;y++)
    digitalWrite(pin[y], !mode_active[y]);
}

void lightOff() {
  Serial.println("Turn Everything Off");
  for(int y=0;y<4;y++)
    digitalWrite(pin[y], !OFF);
}

void informState(AsyncWebServerRequest *request) {

      JsonObject obb = root.to<JsonObject>();
      obb["wLOS"] = mode_active[0];
      obb["wLTS"] = mode_active[1];
      obb["yLS"] = mode_active[2];
      obb["fS"] = mode_active[3];
      obb["aS"] = state;

      String jsonString;
      serializeJson(obb,jsonString);

      Serial.print("Sent: ");
      Serial.println(jsonString);

      request->send(200, "application/json", jsonString);
}

void updateState(AsyncWebServerRequest *request, JsonVariant &json) {

      JsonObject jsonObj = json.as<JsonObject>();

      
      if(jsonObj.containsKey("wLOS")){
          mode_active[0] = jsonObj["wLOS"]; 
      }
      
      if(jsonObj.containsKey("wLTS")){
          mode_active[1] = jsonObj["wLTS"]; 
      }
      
      if(jsonObj.containsKey("yLS")){
          mode_active[2] = jsonObj["yLS"]; 
      }

      if(jsonObj.containsKey("fS")){
          mode_active[3] = jsonObj["fS"]; 
      }

      if(jsonObj.containsKey("aS")){
          state = jsonObj["aS"]; 
      }

      if (state == OFF) 
        lightOff();
      if (state == ON) 
        lightOn();

      request->send(200);
}


void setup() {
  Serial.begin(115200);
  delay(10);

  for(int y=0;y<4;y++)
    pinMode(pin[y],OUTPUT);
    
  lightOn();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println();
  Serial.println("Connected");

  LittleFS.begin();
  Serial.println("File System Intialized");

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.serveStatic("/static/css/", LittleFS, "/static/css/");
  server.serveStatic("/static/js/", LittleFS, "/static/css/");
  server.serveStatic("/static/media/", LittleFS, "/static/media/");

  server.on("/state", HTTP_GET, informState);
  // server.on("/state", HTTP_POST, updateState);

  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/state",updateState);
  server.addHandler(handler); 

  server.begin();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  MDNS.addService("http", "tcp", 80);

  if (MDNS.begin("roomlight")) {
    Serial.println("MDNS started at roomlight.local");
  }
  else{
    Serial.println("MDNS Failed");
  }

}

void loop() {
  MDNS.update();
}
