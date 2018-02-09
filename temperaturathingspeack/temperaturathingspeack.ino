//Dichiarazione Pin e variabili

int sensorPin = A0;     
int sensorValue = 0;    
float temperatura = 0.0;  
float voltage = 0.0;

//Importo le librerie necessarie
#include <SPI.h>
#include <WiFi101.h>
 
char ssid[] = "mettiquiilnomedellawifi"; 
char pass[] = "mettiquilapassworddellawifi"; 
int status = WL_IDLE_STATUS;

// Inizializza il wifi
WiFiClient client;

// Impostazioni di thingspeack
char server[] = "api.thingspeak.com";
String writeAPIKey = "mettiquilachiaveapidiscrittura";

unsigned long lastConnectionTime = 0; // traccia il momento dell'ultima connessione
const unsigned long postingInterval = 60L * 1000L; // posta i dati su thingspeack ogni 60 secondi

void setup() {
     
      analogReadResolution(10);
       
  // tentativo di connessione alla wifi
  while ( status != WL_CONNECTED) {
   
    status = WiFi.begin(ssid, pass);

    // aspetta 10 secondi per la connessione
    delay(10000);
  }
}

void loop() {
  // Se l'intervallo di tempo dall'ultima connessione è trascorso
  // Invia di nuovo i dati
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

}

void httpRequest() {
  sensorValue = analogRead(sensorPin); //Leggo i dati dal sensore
  voltage = sensorValue * (3300/1024); //Converto in volt i dati del sensore
  temperatura = (voltage - 500 ) / 10; //Converto i volt in temperatura

  // Creo una stringa di dati da inviare a thingspeack
  String data = String("field1=" + String(sensorValue, DEC) + "&field2=" + String(temperatura, DEC)); 
  
  // Chiudo la connessione prima di inviare di nuovo i dati.
  client.stop();

  // Invio i dati a thingspeack
  if (client.connect(server, 80)) {
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("X-THINGSPEAKAPIKEY: "+writeAPIKey);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.print(data.length());
    client.print("\n\n");
    client.print(data);

    // Annoto l'ora dell'ultima connessione
    lastConnectionTime = millis();
  }
}
