#include <ArduinoJson.h>
#include "Splitflap.h"
#include <Ethernet.h>
#include <SPI.h>

//hall-sensor pins from the splitflaps (order matters)
int sensPins[32] = {
  2,  3,  4,  5,  49, 48, 47, 46,
  43, 42, 41, 40, 39, 38, 37, 36,
  35, 34, 33, 32, 31, 30, 29, 28,
  A8, A9, A10,A11,A12,A13,A14,A15
};    
                
int serialPins[6] = {A0,A1,A2,A3,A4,A5};              //DataEnable, Data, ClockEnable, Clock, LatchEnable, Latch 
Splitflap splitflaps(32, sensPins, 100, serialPins);    //make an object from the class

IPAddress server(192,168,0,245);
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,0,177);
EthernetClient client;


void setup() {  
  Serial.begin(9600);

  // initialize ethernet
  delay(1000);
  Ethernet.init(53);
  Ethernet.begin(mac, ip);
  delay(2000);
}

void loop() {
  updatedata();
  delay(2000);
}

void updatedata(){
  // Connect to HTTP server
  client.setTimeout(1000);
  if (!client.connect(server, 80)) {
    Serial.println(F("Connection failed"));
    
    // attempt to reconnect
    Ethernet.begin(mac, ip);
    delay(2000);
    if (!client.connect(server, 80)) {
      Serial.println(F("couldn't reconnect"));
      return;
    }
  }

  //Serial.println(F("Connected!"));

  // Send HTTP request
  client.println("GET http://192.168.0.245/api/getboards HTTP/1.1");
  client.println("Host: 192.168.0.245");
  client.println("Connection: close");
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }
  
  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(20) + JSON_ARRAY_SIZE(2) + 500;
  DynamicJsonDocument doc(capacity);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // Extract values
  //Serial.println(F("Response:"));
  Serial.println();
  Serial.println(doc["A"][0]["board"].as<char*>());
  //Serial.println(doc["A"][0]["align"].as<String>());
  Serial.println(doc["A"][0]["first_text"].as<String>());
  Serial.println(doc["A"][0]["second_text"].as<String>());
  //Serial.println(doc["A"][0]["icon_index"].as<int>());
  //Serial.println(doc["A"][0]["time"].as<String>());
  Serial.println();
  Serial.println(doc["B"][0]["board"].as<char*>());
  //Serial.println(doc["B"][0]["align"].as<String>());
  Serial.println(doc["B"][0]["first_text"].as<String>());
  Serial.println(doc["B"][0]["second_text"].as<String>());
  //Serial.println(doc["B"][0]["icon_index"].as<int>());
  //Serial.println(doc["B"][0]["time"].as<String>());

  // Disconnect
  //client.stop();
}
