#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <SimpleDHT.h>

//Pins
int flaps = 3;
int heating = 4;
int vent = 5;
int light = 6;
int led = 7;
int pinDHT11 = 2;

SimpleDHT11 dht11(pinDHT11);

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Credentials MySQL server
IPAddress server_addr(37,59,55,185);
char user[] = "qaHS6nqQwW";
char password[] = "OLampD9032";

//Query status
char query_a[] = "SELECT * FROM qaHS6nqQwW.status_board WHERE track=2";

//Query sensor
char UPDATE_DATA[] = "UPDATE qaHS6nqQwW.sensor_values SET temperature=%s,humidity=%s WHERE track='2'";
char query_b[128];
char temperature_sensor[10];
char humidity_sensor[10];

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
//Read Status
  
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(query_a);
  column_names *cols = cur_mem->get_columns();
  row_values *row = NULL;
  do {
    row = cur_mem->get_next_row();
  
    if (row != NULL) {
 digitalWrite(flaps, atol(row->values[2]));
 digitalWrite(heating, atol(row->values[3]));
 digitalWrite(vent, atol(row->values[4]));
 digitalWrite(light, atol(row->values[5]));
 digitalWrite(led, atol(row->values[6])); }
  } while (row != NULL); {
  delete cur_mem; }

    delay(1000);
}
