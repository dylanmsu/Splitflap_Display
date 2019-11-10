#include <SimpleDHT.h>
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
int pinDHT11 = 34;
SimpleDHT11 dht11(pinDHT11);

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server_addr(37,59,55,185);
char user[] = "qaHS6nqQwW";
char password[] = "OLampD9032";

char UPDATE_DATA[] = "UPDATE qaHS6nqQwW.sensor_values SET temperature=%s,humidity=%s WHERE track='2'";
char query[128];
char temperature_sensor[10];
char humidity_sensor[10];

EthernetClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Ethernet.begin(mac_addr);
  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
  }
  else {
    Serial.println("Connection failed."); }
  
}

void loop() {
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    delay(10000);
    return;
  }

  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    dtostrf((float)temperature, 3, 1, temperature_sensor);
    dtostrf((float)humidity, 3, 1, humidity_sensor);
    Serial.print("Temperatuur: ");
    Serial.println(temperature_sensor);
    Serial.print("Vochtigheid: ");
    Serial.println(humidity_sensor);
    sprintf(query, UPDATE_DATA, temperature_sensor, humidity_sensor);
    cur_mem->execute(query);
    delete cur_mem;

  delay(10000); }
   
