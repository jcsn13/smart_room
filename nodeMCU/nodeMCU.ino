/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 2 (connect to TX of other device)
 * TX is digital pin 3 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>
#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#define WIFI_SSID "CACAU"
#define WIFI_PASSWORD "87897685"
#define DATABASE_URL "https://sd-smart-room-cd813-default-rtdb.firebaseio.com/"
#define DATABASE_SECRET "BiZXQRwvenZllelwXbgZzxNppUP73fgQXZ9ozoeF"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;


//Pinos de comunicacao serial com a ST Núcleo
#define Pin_ST_NUCLEO_RX    5  //Pino D1 da placa Node MCU
#define Pin_ST_NUCLEO_TX    4  //Pino D2 da placa Node MCU

SoftwareSerial SSerial(Pin_ST_NUCLEO_RX, Pin_ST_NUCLEO_TX);

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SSerial.begin(115200);

  Serial.println("Serial by hardware!");

  // set the data rate for the SoftwareSerial port
  SSerial.println("L");

  // Firebase

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the database URL and database secret(required) */
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);
}

void loop() // run over and over
{
    delay(100);

    // Set Temperature
//    if(Firebase.RTDB.getInt(&fbdo, "LED")) {
//      if(fbdo.intData() == 1){
//        SSerial.write("D");
//      } else {
//        SSerial.write("L");
//      }
//    }

    delay(100);
    
    // Change LED State
    if(Firebase.RTDB.getInt(&fbdo, "LED")) {
      if(fbdo.intData() == 1){
        SSerial.write("L");
      } else {
        SSerial.write("D");
      }
    }

    delay(100);
    
    // Change AC State
    if(Firebase.RTDB.getInt(&fbdo, "AC")) {
      if(fbdo.intData() == 1){
        SSerial.write("A");
      } else {
        SSerial.write("Z");
      }
    }

    delay(100);

    // Change Door State
    if(Firebase.RTDB.getInt(&fbdo, "DOOR")) {
      if(fbdo.intData() == 1){
        SSerial.write("O");
      } else {
        SSerial.write("C");
      }
    }
}
