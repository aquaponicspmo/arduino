
/*
  temperaturabot

  Questo sketch mostra come programmare un Arduino MKR1000 Wi-Fi per leggere 
  la temperatura da un sensore di tipo TMP36 e inviare il dato della temperatura all'utente, 
  ogni vota che questo lo chiede ad un bot Telegram.

  This example code is in the public domain.

*/

int sensorPin = A0; //Dichiaro il pin collegato al sensore di temperatura     
int sensorValue = 0;  //Inizializzo una variabile che conterrà il valore di lettura del sensore  
float voltage = 0.0; //Inizializzo una variabile che conterrà il valore del voltaggio proveniente dal sensore
float temperature = 0.0;  //Inizializzo una variabile che conterrà il valore della temperatura
String temperatura = "zero gradi"; //Inizializzo una variabile di tipo stringa per il valore della temperatura 

//Importo le librerie necessarie alla compilazione dello sketch

#include <WiFi101.h>
#include <SPI.h>
#include <TelegramBot.h>
#include <Adafruit_SleepyDog.h> 

// Connetto l'Arduino MKR1000 al Wi-Fi 

char ssid[] = "nomewifi";             // Tra virgolette inserisco il nome della rete Wi-Fi
char pass[] = "passwordwifi";           // Tra virgolette inserisco la password del Wi-Fi


// Inizializzo il BOT Telegram 

const char BotToken[] = "numerotoken"; //Tra virgolette inserisco il token del bot, fornito da @BotFather

WiFiSSLClient client;
TelegramBot bot (BotToken, client);


void setup() {

  Serial.begin(115200);
  while (!Serial) {}
  delay(3000);


  // Stampo sul monitor seriale le informazioni sul tentativo di connessione alla Wi-Fi
  
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");


  bot.begin();
  Watchdog.enable(10000); // Questa funzione riavvia in automatico Arduino nel caso non risponda  

}

void loop() {
  
    Watchdog.reset(); 

    message m = bot.getUpdates(); // Leggo i messaggi provenienti dal bot
    
    if ( m.text.equals("temperatura")){ // Controllo se l'utente ha inviato un messaggio col testo temperatura
      
      sensorValue = analogRead(sensorPin); //Leggo il valore dal sensore
      voltage = sensorValue * (3300/1024); //Converto il valore del sensore in volt
      temperature = (voltage - 500 ) / 10; //Converto i volt in temperatura in base alle specifiche del sensore TMP36
      temperatura = String(temperature); //Converto la temperatura da numero a stringa
      
      Serial.println(temperatura); //Stampo sul monitor seriale il valore della temperatura
      bot.sendMessage(m.chat_id, "la tempertura è di "+temperatura+" °c");  // Invio la temperatura all'utente via chat Telegram
    
    } else {
      Serial.println("nessun messaggio corretto"); //Se l'utente non invia un messaggio corretto, stampo sul monitor seriale un avviso 
    }

}
