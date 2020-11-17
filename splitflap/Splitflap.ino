#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <ArduinoJson.h>
#include "Splitflap.h"
#include <Ethernet.h>
#include <SPI.h>

#define RGB_LED_PIN 46

#define board "A"
#define polDelay 10000 //ms

Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

// hall-sensor pins from the splitflaps
int sensPins[32] = {
    2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29, 30, 31, 32, 33
};

unsigned long lastMillis;

//int serialPins[6] = {44,38,42,36,40,34};                // DataEnable, Data, ClockEnable, Clock, LatchEnable, Latch 
int serialPins[6] = {2,3,4,5,6,7};                // DataEnable, Data, ClockEnable, Clock, LatchEnable, Latch 
Splitflap splitflaps(sensPins, 200, serialPins);        // make an object from the class

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};      // mac-address of the ethernet module (must be unique)
IPAddress myDns(192, 168, 0, 1);                        // dns server. This is likely your home router

EthernetClient client;
String prevtext = "";


void setup() {
    Serial.begin(9600);
    initEthernet();

    pinMode(48, OUTPUT);
    digitalWrite(48, 1);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    strip.setBrightness(32); // max: 255
}

void loop() {
    /*if (millis() - lastMillis >= 2*60*1000UL)
    {
       lastMillis = millis();  //get ready for the next iteration
    }*/
    updateDisplay();
    rainbowCycle(20);
    delay(10000);
}

void updateDisplay(){
    DynamicJsonDocument doc = fetchFromApi();

    Serial.println(doc[board].as<String>());
    if (doc[board].as<String>() != "null") {
        String textA = doc[board]["first_text"].as<String>();
        String textB = doc[board]["second_text"].as<String>();
        String align = doc[board]["align"].as<String>();
        int minutes = doc[board]["minutes"].as<int>();
        int hours = doc[board]["hours"].as<int>();
        int icon = doc[board]["icon_index"].as<int>();
        
        String text = "";
    
        if (textA == "null") {
            textA = "";
        }
    
        if (textB == "null") {
            textB = "";
        }
        
        if (align == "left"){
            String a = textA + fill(14-textA.length());
            String b = textB + fill(14-textB.length());
            text = a + b;
        } else if (align == "center"){
            String spaceA = fill((14 - textA.length())/2);
            String spaceB = fill((14 - textB.length())/2);
            String a = spaceA + textA + fill(14-(spaceA.length()+textA.length()));
            String b = spaceB + textB + fill(14-(spaceB.length()+textB.length()));
            text = a + b;
        } else if (align == "right"){
            String a = fill(14-textA.length()) + textA;
            String b = fill(14-textB.length()) + textB;
            text = a + b;
        } else {
            Serial.println("unrecognized alignment");
        }
    
        if (prevtext != text){
            prevtext = text;
            splitflaps.enableAll();
            Serial.println("sending " + text + " to the display... ");
            splitflaps.Send(text, icon, hours, minutes);
            splitflaps.disableAll();
            Serial.println("done.");
        } else {
            Serial.println("nothing new to display");
        }
    } else {
        if (prevtext != "no trains"){ 
            prevtext = "no trains";
            String textA = "geen treinen";
            String textB = "vandaag";
            String spaceA = fill((14 - textA.length())/2);
            String spaceB = fill((14 - textB.length())/2);
            String a = spaceA + textA + fill(14-(spaceA.length()+textA.length()));
            String b = spaceB + textB + fill(14-(spaceB.length()+textB.length()));
            splitflaps.Send(a + b, 0, 0, 0);
            splitflaps.disableAll();
        }
        
    }
}

void printSensorStatus(){
    for (int i=0; i<32; i++){
        Serial.print(digitalRead(sensPins[i]));
    }
    Serial.println();
    delay(100);
}

void showAllCharakters(){
    const char alphabet[56] = {"abcdefghijklmnopqrstuvwxyz-_/ ABCDEFGHIJKLMNOPQRSTUVWXYZ?"};
    for (int i=0; i<52; i++){
        String text = "";
        for (int j=0; j<28; j++){
            text[j] = alphabet[i];
        }
        Serial.print("showing: ");
        Serial.println(text);
        splitflaps.Send(text, 0, 0, 0);
        delay(10000);
    }
}

String fill(int amount){
    String spaces = "";
    for (int i=0; i<amount; i++){
        spaces += " ";
    }
    return spaces;
}

void initEthernet() {
    Serial.print("initializing Ethernet... ");
    delay(1000);
    Ethernet.init(53);
    Ethernet.begin(mac);
    delay(2000);
    Serial.println("done.");
}

DynamicJsonDocument fetchFromApi(){
    Serial.println("fetching data from api...");
    // Connect to HTTP server
    Serial.print("\tconnecting... ");
    client.setTimeout(1000);
    if (!client.connect("api.scm-team.be", 80)) {
        Serial.println(F("\tConnection failed"));
        
        // attempt to reconnect
        Ethernet.begin(mac);
        delay(2000);
        if (!client.connect("api.scm-team.be", 80)) {
            Serial.println(F("\tcouldn't reconnect"));
            return;
        }
    }else{
        Serial.println("\tconnected!");
    }

    // Send HTTP request
    Serial.print("\tsending request and awaiting... ");
    client.println("GET /index.php HTTP/1.1");
    client.println("Host: api.scm-team.be");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    if (client.println() == 0) {
        Serial.println(F("Failed to send request"));
        return;
    } else {
        Serial.println("done.");
    }
    
    // Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
        Serial.print(F("\tUnexpected response: "));
        Serial.println(status);
        return;
    } else {
        Serial.println("\tstatus: 200 OK");
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) {
        Serial.println(F("\tInvalid response"));
        return;
    }

    // Allocate the JSON document
    // Use arduinojson.org/v6/assistant to compute the capacity.
    Serial.print("\tparsing json data... ");
    const size_t capacity = JSON_OBJECT_SIZE(20) + JSON_ARRAY_SIZE(2) + 500;
    DynamicJsonDocument doc(capacity);

    // Parse JSON object
    DeserializationError error = deserializeJson(doc, client);
    if (error) {
        Serial.print(F("\tdeserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }
    Serial.println("done.");
    return doc;
}

void rainbowCycle(uint8_t wait) {
    uint16_t i, j;
    
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
