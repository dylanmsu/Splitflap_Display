#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <math.h>

//Pins
int flaps = 3;
int heating = 4;
int vent = 5;
int light = 6;
int led = 7;
int tempsens = A5;

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
IPAddress server_addr(192,168,200,15);
IPAddress ip(192,168,200,178);
char user[] = "tijl";
char password[] = "73647364";

//Query status
char READ[] = "SELECT * FROM scmserver.status_board WHERE track=2";

//Query sensor
char UPDATE[] = "UPDATE scmserver.sensor_values SET temperature=%s WHERE track='2'";
char query[128];

EthernetClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Ethernet.begin(mac_addr);
  Serial.println("Verbinden...");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
  }
  else {
    Serial.println("Verbinding mislukt."); }

//Output Pins    
    pinMode(flaps, OUTPUT);
    pinMode(heating, OUTPUT);
    pinMode(vent, OUTPUT);
    pinMode(light, OUTPUT);
    pinMode(led, OUTPUT);
}

void loop() {

//Temperature Reading
  int readVal=analogRead(tempsens);
 double temp =  Thermistor(readVal);
 dtostrf((float)temp, 3, 1, temperature_sensor);
    
//UPDATE Query     
MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
sprintf(query, UPDATE, temperature_sensor);
cur_mem->execute(query);    

//DISPLAY INPUT
    Serial.println("Data Opgeslaan:");
    Serial.print("Temperatuur: ");
    Serial.println(temperature_sensor);

//READ Query
  sprintf(query, READ);
  cur_mem->execute(query);
  column_names *cols = cur_mem->get_columns();
  row_values *row = NULL;
  do {
    row = cur_mem->get_next_row();
  
    if (row != NULL) {

//OUTPUT RELAIS
 digitalWrite(flaps, atol(row->values[2]));
 digitalWrite(heating, atol(row->values[3]));
 digitalWrite(vent, atol(row->values[4]));
 digitalWrite(light, atol(row->values[5]));
 digitalWrite(led, atol(row->values[6])); }
 
  } while (row != NULL); {
  delete cur_mem; }
    delay(1000);
}
