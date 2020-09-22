#include <ArduinoJson.h>
#include "Splitflap.h"
#include <Ethernet.h>
#include <SPI.h>


//hall-sensor pins from the splitflaps (order matters)
int sensPins[32] = {
  2,  3,  4,  5,  6,  7,  8,  9,
  10, 11, 12, 13, 14, 15, 16, 17,
  18, 19, 20, 21, 22, 23, 24, 25,
  26, 27, 28, 29, 30, 31, 32, 33
};    
                
int serialPins[6] = {44,38,42,36,40,34};            //DataEnable, Data, ClockEnable, Clock, LatchEnable, Latch 
Splitflap splitflaps(32, sensPins, 100, serialPins);    //make an object from the class

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,0,178);
IPAddress myDns(192, 168, 0, 1);
EthernetClient client;

String prevtext = "";

void setup() {  
  Serial.begin(9600);

  // initialize ethernet
  /*delay(1000);
  Ethernet.init(53);
  Ethernet.begin(mac, ip,  myDns);
  delay(2000);

  /*splitflaps.enableAll();
  splitflaps.WriteText("whiteRED");
  splitflaps.disableAll();/**/
}

void loop() {
  updatedata();
  //delay(2000);
  
  /*char alphabet[56] = {"abcdefghijklmnopqrstuvxyz-_/ ABCDEFGHIJKLMNOPQRSTUVWXYZ?"};
  for (int i=0; i<52; i++){
    String text = "";
    for (int j=0; j<28; j++){
      text[j] = alphabet[i];
    }
    Serial.println(text);
    splitflaps.WriteText(text);
    delay(10000);
  }*/
  
  //splitflaps.enableAll();
  /*for (int i=0; i<32; i++){
    Serial.print(digitalRead(sensPins[i]));
    //splitflaps.flipSegment(i);
    delay(0);
  }
  Serial.println();
  delay(100);
  //splitflaps.disableAll();/**/
}

String fill(int amount){
  String spaces = "";
  for (int i=0; i<amount; i++){
    spaces += " ";
  }
  return spaces;
}

void updatedata(){
  // Connect to HTTP server
  client.setTimeout(1000);
  if (!client.connect("api.scm-team.be", 80)) {
    Serial.println(F("Connection failed"));
    
    // attempt to reconnect
    Ethernet.begin(mac, ip);
    delay(2000);
    if (!client.connect("api.scm-team.be", 80)) {
      Serial.println(F("couldn't reconnect"));
      return;
    }
  }

  //Serial.println(F("Connected!"));

  // Send HTTP request
  client.println("GET /index.txt HTTP/1.1");
    client.println("Host: api.scm-team.be");
    client.println("User-Agent: arduino-ethernet");
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
  //Serial.println();
  //Serial.println(doc["A"][0]["board"].as<char*>());
  //Serial.println(doc["A"][0]["align"].as<String>());
  //Serial.println(doc["A"][0]["first_text"].as<String>());
  //Serial.println(doc["A"][0]["second_text"].as<String>());
  //Serial.println(doc["A"][0]["icon_index"].as<int>());
  //Serial.println(doc["A"][0]["time"].as<String>());
  //Serial.println();
  //Serial.println(doc["B"][0]["board"].as<char*>());
  //Serial.println(doc["B"][0]["align"].as<String>());
  //Serial.println(doc["B"][0]["first_text"].as<String>());
  //Serial.println(doc["B"][0]["second_text"].as<String>());
  //Serial.println(doc["B"][0]["icon_index"].as<int>());
  //Serial.println(doc["B"][0]["time"].as<String>());


  String textA = doc["A"][0]["first_text"].as<String>();
  String textB = doc["A"][0]["second_text"].as<String>();
  String text((textA + fill(14-textA.length()))+(textB + fill(14-textB.length())));
  if (prevtext != text){
    prevtext = text;
    splitflaps.enableAll();
    Serial.println(text);
    splitflaps.WriteText(text);
    splitflaps.disableAll();
  } else {
    Serial.println("nothing new");
  }

  // Disconnect
  //client.stop();
}
