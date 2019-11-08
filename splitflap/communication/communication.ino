#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


IPAddress server_addr(37,59,55,185);
char user[] = "qaHS6nqQwW";
char password[] = "L8tuG6egP9";

// Sample query
char query[] = "SELECT * FROM qaHS6nqQwW.status LIMIT 1";

EthernetClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect
  Ethernet.begin(mac_addr);
  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
  }
  else {
    Serial.println("Connection failed."); }
    pinMode(22, OUTPUT);
    pinMode(24, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(28, OUTPUT);
    pinMode(30, OUTPUT);
   
}


void loop() {

  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(query);
  column_names *cols = cur_mem->get_columns();
  row_values *row = NULL;
  do {
    row = cur_mem->get_next_row();

  int dylan[5] = {1,1,1,1,1};



    
    if (row != NULL) {
        
        
        digitalWrite(22, row->values[1]=='1');
        digitalWrite(24, row->values[2]=='1'); 
        digitalWrite(26, row->values[3]=='1');
        digitalWrite(28, row->values[4]=='1'); 
        digitalWrite(30, row->values[5]=='1');
        Serial.print("Status Flaps: ");
        Serial.println(row->values[1]);
        Serial.print("Status Verwarming: ");
        Serial.println(row->values[2]);
        Serial.print("Status Ventilatie: ");
        Serial.println(row->values[3]);
        Serial.print("Status Verlichting: ");
        Serial.println(row->values[4]);
        Serial.print("Status LED: ");
        Serial.println(row->values[5]);
        Serial.print("Ledkleur: ");
        Serial.println(row->values[6]);
        Serial.print("Aangepast door :");
        Serial.print(row->values[7]);
        Serial.print(" om ");
        Serial.println(row->values[8]);
        Serial.println();
    delay(1000);      

    }
  } while (row != NULL);
  delete cur_mem;
}
