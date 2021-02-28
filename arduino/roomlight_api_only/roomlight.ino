#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> 

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

ESP8266WebServer server(80);


StaticJsonDocument<200> root;


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

  server.on("/state", HTTP_GET, informState);
  server.on("/state", HTTP_POST, updateState); 

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

void informState() {

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

      server.send(200, "application/json", jsonString);
}

void updateState() {

      deserializeJson(root, server.arg("plain"));
      Serial.print("Got: ");
      Serial.println(server.arg("plain"));
      
      if(root.containsKey("wLOS")){
          mode_active[0] = root["wLOS"]; 
      }
      
      if(root.containsKey("wLTS")){
          mode_active[1] = root["wLTS"]; 
      }
      
      if(root.containsKey("yLS")){
          mode_active[2] = root["yLS"]; 
      }

      if(root.containsKey("fS")){
          mode_active[3] = root["fS"]; 
      }

      if(root.containsKey("aS")){
          state = root["aS"]; 
      }

      if (state == OFF) 
        lightOff();
      if (state == ON) 
        lightOn();

      server.send(200);
  }

void loop() {
  MDNS.update();
  server.handleClient();
}
