#include <Ticker.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

#include "Splitflap.h"
#include "config.h"

// hall-sensor pins from the splitflaps
int sensPins[32] = {
    S0,  S1,  S2,  S3,  S4,  S5,  S6,  S7,
    S8,  S9,  S10, S11, S12, S13, S14, S15,
    S16, S17, S18, S19, S20, S21, S22, S23,
    S24, S25, S26, S27, S28, S29, S30, S31
};

// serial pins of the splitflap driver boards
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

void updateDisplay();

Splitflap splitflaps(sensPins, FLAP_DELAY, serialPins);        // make an object from the class
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};      // mac-address of the ethernet module (must be unique)
IPAddress myDns(192, 168, 0, 1);                        // dns server. This is likely your home router
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_RGB_PIXELS, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
DHT dht(DHT_PIN, DHT22);
EthernetClient client;
Ticker pollTimer(updateDisplay, POLL_DELAY); 

void setup() {
    if (DEBUG) Serial.begin(9600);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(LIGHT_SENS, INPUT);
    pinMode(WHITE_LED_EN, OUTPUT);
    pinMode(ENABLE_PS_PIN, OUTPUT);

    pinMode(RGB_STRIP_EN, OUTPUT);
    digitalWrite(RGB_STRIP_EN, 1);
    
    enablePSU();

    strip.begin();
    strip.show();
    strip.setBrightness(RGB_BRIGHTNESS);
    
    initEthernet();
    dht.begin();
    pollTimer.start();

    strip.setPixelColor(10, 255);
}

void loop() {
    pollTimer.update();
    //showAllCharakters();
    //§printSensorStatus();
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
        if (DEBUG) Serial.println("unrecognized alignment");
    }
    return text;
}

void updateDisplay(){
    
    DynamicJsonDocument doc = fetchFromApi();
    if (DEBUG) Serial.println(doc[BOARD].as<String>());
    if (doc[BOARD].as<String>() != "null") {
        String textA = doc[BOARD]["first_text"].as<String>();
        String textB = doc[BOARD]["second_text"].as<String>();
        String align = doc[BOARD]["align"].as<String>();
        int minutes = doc[BOARD]["minutes"].as<int>();
        int hours = doc[BOARD]["hours"].as<int>();
        int icon = doc[BOARD]["icon_index"].as<int>();
        whiteLED = doc[BOARD]["white_led"].as<int>();
        String stripColor = doc[BOARD]["rgb_strip"]["color"].as<String>();
        RGBStripMode = doc[BOARD]["rgb_strip"]["mode"].as<int>();

        Serial.println(doc[BOARD]["rgb_strip"]["color"].as<String>());
        RGBStripColor = (int) strtol( &stripColor[1], NULL, 16);
        Serial.println(int( strtol( &stripColor[1], NULL, 16)));

        updatePeripherals();
    
        if (textA == "null") {
            textA = "";
        }
    
        if (textB == "null") {
            textB = "";
        }

        String text = CombineText(textA, textB, align);
    
        if (prevtext != text){
            prevtext = text;
            
            delay(1000);
            if (DEBUG) Serial.println("sending " + text + " to the display... ");
            splitflaps.Send(text, icon, hours, minutes);
            if (DEBUG) Serial.println("done.");
        } else {
            if (DEBUG) Serial.println("nothing new to display");
        }
    } else {
        if (prevtext != "no trains"){ 
            prevtext = "no trains";
            splitflaps.Send(fill(28),0,-1,-1);
        }
    }
    pollTimer.start();
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

    Serial.println(RGBStripColor);
    Serial.println(RGBStripMode);
    UpdateRGBStrip();
    
    analogWrite(FAN, fanSpeed);
    analogWrite(WHITE_LED_EN, whiteLED);
    analogWrite(LED_R, (RGBLedColor & 0xff0000) >> 16);
    analogWrite(LED_G, (RGBLedColor & 0x00ff00) >> 8);
    analogWrite(LED_B, (RGBLedColor & 0x0000ff));
}

void printSensorStatus(){
    for (int i=0; i<32; i++){
        if (DEBUG) Serial.print(digitalRead(sensPins[i]));
    }
    if (DEBUG) Serial.println();
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

void enablePSU() {
    digitalWrite(ENABLE_PS_PIN, LOW);
}

void disablePSU() {
    digitalWrite(ENABLE_PS_PIN, HIGH);
}

void RGBLed(int r, int g, int b){
    RGBLedColor = (r << 16) || (g << 8) || (b);
}

void showAllCharakters(){
    String alphabet = "abcdefghijklmnopqrstuvwxyz-_/ ABCDEFGHIJKLMNOPQRSTUVWXYZ?";
    for (int i=0; i<58; i++){
        
        String text = "";
        for (int j=0; j<28; j++){
            text += String(alphabet[i]);
        }

        Serial.print("showing: ");
        Serial.println(String(text));
        splitflaps.Send(text, 0, 0, 0);
        delay(15000);
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
    if (DEBUG) Serial.print("initializing Ethernet... ");
    delay(1000);
    Ethernet.init(53);
    Ethernet.begin(mac);
    delay(2000);
    if (DEBUG) Serial.println("done.");
}

char *stringToCharArray(String string){
    char Array[string.length()];
    string.toCharArray(Array, string.length());
    return Array;
}

DynamicJsonDocument fetchFromApi(){
    if (DEBUG) Serial.println("fetching data from api...");
    if (DEBUG) Serial.print("\tconnecting... ");
    client.setTimeout(1000);
    int numReconnect = 0;
    while (!client.connect(HOSTNAME, 80)){
        numReconnect += 1;
        if (DEBUG) Serial.println(F("\tConnection failed"));
        delay(500);
        if (DEBUG) Serial.println(F("\tTrying again..."));
        delay(1000);
        if (numReconnect >= 10){
            error = "Unable to connect";
            return;
        }
    }

    if (DEBUG) Serial.println("\tconnected!");
    
    // Send HTTP request
    if (DEBUG) Serial.println("\tsending request and awaiting... ");

    char *reqHeader = stringToCharArray(String("GET ") + ROUTE + String(" HTTP/1.1"));
    client.println(reqHeader);
    //client.println("GET /index.php HTTP/1.1");

    char *hostheader = stringToCharArray(String("Host: ") + HOSTNAME);
    client.println(hostheader);
    //client.println("Host: api.scm-team.be")

    client.println("User-Agent: arduino-ethernet");

    char *boardheader = stringToCharArray(String("board: ") + BOARD);
    client.println(boardheader);
    //client.println("board:B");
    
    String temph = String("temperature:") + String(temperature);
    char tempHeader[temph.length()];
    temph.toCharArray(tempHeader, temph.length());
    client.println(tempHeader);
    if (DEBUG) Serial.println(tempHeader);
    //client.println("temperature:123");
    
    String humidh = String("humidity:") + String(humidity);
    char humidHeader[humidh.length()];
    humidh.toCharArray(humidHeader, humidh.length());
    client.println(humidHeader);
    if (DEBUG) Serial.println(humidHeader);
    //client.println("humidity:456");

    float light = getLightLevel();
    String lighth = String("lightLevel:") + String(light);
    char lightHeader[lighth.length()];
    lighth.toCharArray(lightHeader, lighth.length());
    client.println(lightHeader);
    if (DEBUG) Serial.println(lightHeader);
    //client.println("lightLevel:789");

    client.println("error: No_Error");
    client.println("ledIntensity:123");
    client.println("RGBColor:hhh");

        
    /*
     * board:A
     * temperature:24.5
     * humidity:60
     * error:No_Error
     * lightLevel:789
     * ledIntensity:128
     */
    
    client.println("Connection: close");
    if (client.println() == 0) {
        if (DEBUG) Serial.println(F("Failed to send request"));
        return;
    } else {
        if (DEBUG) Serial.println("done.");
    }
    
    // Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
        if (DEBUG) Serial.print(F("\tUnexpected response: "));
        if (DEBUG) Serial.println(status);
        return;
    } else {
        if (DEBUG) Serial.println("\tstatus: 200 OK");
    }
    
    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) {
        if (DEBUG) Serial.println(F("\tInvalid response"));
        return;
    }

    // Allocate the JSON document
    // Use arduinojson.org/v6/assistant to compute the capacity.
    if (DEBUG) Serial.print("\tparsing json data... ");
    const size_t capacity = JSON_OBJECT_SIZE(20) + JSON_ARRAY_SIZE(2) + 500;
    DynamicJsonDocument doc(capacity);

    // Parse JSON object
    DeserializationError error = deserializeJson(doc, client);
    if (error) {
        if (DEBUG) Serial.print(F("\tdeserializeJson() failed: "));
        if (DEBUG) Serial.println(error.c_str());
        return;
    }
    if (DEBUG) Serial.println("done.");
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
