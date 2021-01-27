#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <ArduinoJson.h>
#include "Splitflap.h"
#include <Ethernet.h>
#include <SPI.h>

// parameters
#define BOARD           "A"
#define DEBUG           true
#define FLAP_DELAY      200 //ms
#define POLL_DELAY      10000 //ms
#define NUM_RGB_PIXELS  144
#define RGB_BRIGHTNESS  128

// pins
#define RGB_LED_PIN     46
#define ENABLE_PS_PIN   35
#define DHT_PIN         49
#define WHITE_LED_EN    45
#define RGB_STRIP_en    46
#define FAN             2
#define LIGHT_SENS      A0

#define LED_R           3
#define LED_G           4
#define LED_B           5

#define STAT_A          6
#define STAT_B          7
#define STAT_C          8

#define DATA_ENABLE     44
#define DATA            38
#define CLOCK_ENABLE    42
#define CLOCK           36
#define LATCH_ENABLE    40
#define LATCH           34

// hall-sensor pins from the splitflaps
int sensPins[32] = {
    15, 16, 17, 18, 19, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, A1,
    A2, A3, A4, A5, A6, A7, A8, A9,
    A10,A11,A12,A13,A14,A15,32, 33
};

// DataEnable, Data, ClockEnable, Clock, LatchEnable, Latch
int serialPins[6] = {
    DATA_ENABLE,
    DATA,
    CLOCK_ENABLE,
    CLOCK,
    LATCH_ENABLE,
    LATCH
};

// variables, they will be changed automatically
double temperature = 0;
double humidity = 0;
double lightLevel = 0;
uint8_t fanSpeed = 0;
uint8_t whiteLED = 0;
uint32_t RGBLedColor = 0;
uint16_t RGBStripMode = 0;
uint16_t RGBStripColor = 0;
String error = "No Error";
String prevtext = "";
 
Splitflap splitflaps(sensPins, FLAP_DELAY, serialPins);        // make an object from the class
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};      // mac-address of the ethernet module (must be unique)
IPAddress myDns(192, 168, 0, 1);                        // dns server. This is likely your home router
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_RGB_PIXELS, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
DHT dht(DHT_PIN, DHT22);
EthernetClient client;


void setup() {
    Serial.begin(9600);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(LIGHT_SENS, INPUT);
    pinMode(WHITE_LED_EN, OUTPUT);
    pinMode(ENABLE_PS_PIN, OUTPUT);
    
    digitalWrite(ENABLE_PS_PIN, HIGH);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    strip.setBrightness(RGB_BRIGHTNESS);
    
    initEthernet();
    dht.begin();
}

void loop() {
    updateDisplay();
    
    delay(POLL_DELAY);
}

String CombineText(String textA, String textB, String align){
    String text = "";
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
    return text;
}

void updateDisplay(){
    DynamicJsonDocument doc = fetchFromApi();

    Serial.println(doc[BOARD].as<String>());
    if (doc[BOARD].as<String>() != "null") {
        String textA = doc[BOARD]["first_text"].as<String>();
        String textB = doc[BOARD]["second_text"].as<String>();
        String align = doc[BOARD]["align"].as<String>();
        int minutes = doc[BOARD]["minutes"].as<int>();
        int hours = doc[BOARD]["hours"].as<int>();
        int icon = doc[BOARD]["icon_index"].as<int>();
        
        String text = "";
    
        if (textA == "null") {
            textA = "";
        }
    
        if (textB == "null") {
            textB = "";
        }

        text = CombineText(textA, textB, align);
    
        if (prevtext != text){
            prevtext = text;
            digitalWrite(ENABLE_PS_PIN, LOW);
            delay(1000);
            splitflaps.enableAll();
            Serial.println("sending " + text + " to the display... ");
            splitflaps.Send(text, icon, hours, minutes);
            splitflaps.disableAll();
            Serial.println("done.");
            digitalWrite(ENABLE_PS_PIN, HIGH);
        } else {
            Serial.println("nothing new to display");
        }
    } else {
        if (prevtext != "no trains"){ 
            prevtext = "no trains";
            digitalWrite(ENABLE_PS_PIN, LOW);
            String textA = "geen treinen";
            String textB = "vandaag";
            String spaceA = fill((14 - textA.length())/2);
            String spaceB = fill((14 - textB.length())/2);
            String a = spaceA + textA + fill(14-(spaceA.length()+textA.length()));
            String b = spaceB + textB + fill(14-(spaceB.length()+textB.length()));
            splitflaps.Send(a + b, 0, 0, 0);
            splitflaps.disableAll();
            digitalWrite(ENABLE_PS_PIN, HIGH);
        }
    }
}

void updateVariables(){
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    lightLevel = getLightLevel();

    if (temperature < 15){
        fanSpeed = 0;
    } else if (temperature < 20) {
        fanSpeed = 128;
    } else if (temperature > 20) {
        fanSpeed = 255;
    }
}

void updatePeripherals(){
    updateVariables();
    
    analogWrite(FAN, fanSpeed);
    analogWrite(WHITE_LED_EN, whiteLED);
    analogWrite(LED_R, (RGBLedColor & 0xff0000) >> 16);
    analogWrite(LED_G, (RGBLedColor & 0x00ff00) >> 8);
    analogWrite(LED_B, (RGBLedColor & 0x0000ff));
}

void printSensorStatus(){
    for (int i=0; i<32; i++){
        Serial.print(digitalRead(sensPins[i]));
    }
    Serial.println();
    delay(100);
}

void UpdateRGBStrip(){
    uint8_t red = (RGBStripColor & 0xff0000) >> 16;
    uint8_t grn = (RGBStripColor & 0x00ff00) >> 8;
    uint8_t blu = (RGBStripColor & 0x0000ff);

    switch(RGBStripMode) {
        case 0:
            strip.clear();
            break;
        case 1:
            strip.fill(strip.Color(red, grn, blu), 0, NUM_RGB_PIXELS);
            break;
        
    }
}

int getLightLevel(){
    return analogRead(LIGHT_SENS);
}

void RGBLed(int r, int g, int b){
    RGBLedColor = (r << 16) || (g << 8) || (b);
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
    Serial.print("\tconnecting... ");
    client.setTimeout(1000);
    while (!client.connect("api.scm-team.be", 80)){
        Serial.println(F("\tConnection failed"));
        delay(500);
        Serial.println(F("\tTrying again..."));
        delay(1000);
    }

    Serial.println("\tconnected!");
    
    /*if (!client.connect("api.scm-team.be", 80)) {
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
    }*/

    // Send HTTP request
    Serial.print("\tsending request and awaiting... ");
    
    client.println("GET /index.php HTTP/1.1");
    client.println("Host: api.scm-team.be");
    client.println("User-Agent: arduino-ethernet");
    
    String boardh = (String)"board:" + (String)BOARD;
    char boardHeader[boardh.length()];
    boardh.toCharArray(boardHeader, boardh.length());
    client.println(boardHeader);
    //client.println("board:A");
    
    String temph = (String)"temperature:" + String(temperature);
    char tempHeader[temph.length()];
    temph.toCharArray(tempHeader, temph.length());
    client.println(tempHeader);
    //client.print("temperature:123");
    
    String humidh = (String)"humidity:" + String(humidity);
    char humidHeader[humidh.length()];
    humidh.toCharArray(humidHeader, humidh.length());
    client.println(humidHeader);
    //client.println("humidity:456");

    float light = getLightLevel();
    String lighth = (String)"humidity:" + String(light);
    char lightHeader[lighth.length()];
    lighth.toCharArray(lightHeader, lighth.length());
    client.println(lightHeader);
    //client.println("lightLevel:789");

    client.println("error:No_Error");
    client.println("ledIntensity:123");
    client.println("RGBColor:00ff00");
    
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
