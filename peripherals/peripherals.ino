#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <math.h>

//Pins
int flap_pin = 3;
int heat_pin = 4;
int vent_pin = 5;
int light_pin = 6;
int led_pin = 7;
int wifi_pin = 8;
int tempsens_pin = A5;
int rgb_pin = 2;

//Variables
int num_pixels = 5;
int update_interval = 1;

//Calculate Temperature
double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;
  return Temp;
}
char temperature_sensor[10];

//MAC adress
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Credentials MySQL server
IPAddress server_addr(192,168,100,111);
char user[] = "tijl";
char password[] = "73647364";

//Query status
char READ[] = "SELECT * FROM scmserver.status_board WHERE track=2";

//Query sensor
char UPDATE[] = "UPDATE scmserver.sensor_values SET temperature=%s WHERE track='2'";
char query[128];

EthernetClient client;
MySQL_Connection conn((Client *)&client);

//RGB strip
Adafruit_NeoPixel pixels(num_pixels, rgb_pin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Ethernet.begin(mac_addr);
  //Serial.println("Verbinden...");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
  }
  else {
    Serial.println("Verbinding mislukt."); } 

//Output Pins    
    pinMode(flap_pin, OUTPUT);
    pinMode(heat_pin, OUTPUT);
    pinMode(vent_pin, OUTPUT);
    pinMode(light_pin, OUTPUT);
    pinMode(led_pin, OUTPUT);
    pinMode(wifi_pin, OUTPUT);

//Start RGB strip
  pixels.begin();
}

void loop() {

//Temperature Reading
  int readVal=analogRead(tempsens_pin);
 double temp =  Thermistor(readVal);
 dtostrf((float)temp, 3, 1, temperature_sensor);
    
//UPDATE Query
MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
sprintf(query, UPDATE, temperature_sensor);
cur_mem->execute(query);    

/*
//DISPLAY INPUT
    Serial.println("Data Opgeslaan:");
    Serial.print("Temperatuur: ");
    Serial.println(temperature_sensor); */

//READ Query
  sprintf(query, READ);
  cur_mem->execute(query);
  column_names *cols = cur_mem->get_columns();
  row_values *row = NULL;
  do {
    row = cur_mem->get_next_row();
  
    if (row != NULL) {

//OUTPUT RELAIS
 digitalWrite(flap_pin, atol(row->values[2]));
 digitalWrite(heat_pin, atol(row->values[3]));
 digitalWrite(vent_pin, atol(row->values[4]));
 digitalWrite(light_pin, atol(row->values[5]));
 digitalWrite(led_pin, atol(row->values[6]));
 digitalWrite(wifi_pin, atol(row->values[7]));
 
 for(int i=0; i<num_pixels; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 255));
    pixels.show();  }
 }
 
  } while (row != NULL); {
  delete cur_mem; }
    delay(update_interval*1000);
}
