#include <WiFi.h>
#include <HttpClient.h>
#include <Wire.h>
#include <string.h>
#include <MFRC522.h>
#include <SPI.h>
#include "SPIFFS.h"
#include <FS.h>

// RFID pins defenition
#define RST_PIN         13
#define SS_PIN          21
#define SDA_PIN         21
#define MOSI_PIN        23
#define MISO_PIN        19
#define SCK_PIN         22


// This example downloads the URL "http://arduino.cc/"

char ssid[] = "Fadi's Galaxy S22 Ultra";    // your network SSID (name) 
char pass[] = "19981998"; // your network password (use for WPA, or use as key for WEP)

// Name of the server we want to connect to
const char kHostname[] = "18.118.226.168";

// Port
//const char kPort[] = ":5000";
uint16_t kPort = 5000;

// Path to download (this is the bit after the hostname in the URL
// that you want to download)
const char kPath[] = "/?var=";

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

void setup() {
  Serial.begin(9600);

  // We start by connecting to a WiFi network
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");

  // Initialize DHT20 sensor

 // dht20.begin(DHT20_SDA, DHT20_SCL);

  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {

  
//   // Wait a few seconds between measurements
//   delay(5000);
//  // dht20.read();
//   // Read temperature and humidity values
//  // float temperature = dht20.getTemperature();
//   float humidity = dht20.getHumidity();

//   //dht20.isConnected();
//   Serial.print("Temperature: ");
//   Serial.print(temperature);
//   Serial.print(" °C, Humidity: ");
//   Serial.print(humidity);
//   Serial.println(" %");

//   // Check if any reads failed and exit early (to try again).
//   if (isnan(temperature) || isnan(humidity)) {
//     Serial.println("Failed to read from DHT20 sensor!");
//     return;
//   }
  
// String path = kPath + String("Temperature=") + String(temperature) + String("C+Humidity=") + String(humidity) + String("%");

//   int err =0;
  
//   WiFiClient c;
//   HttpClient http(c);
  
//   err = http.get(kHostname, kPort, path.c_str());

//   if (err == 0)
//   {
//     Serial.println("startedRequest ok");

//     err = http.responseStatusCode();

//     if (err >= 0)
//     {
//       Serial.print("Got status code: ");
//       Serial.println(err);

//       // Usually you'd check that the response code is 200 or a
//       // similar "success" code (200-299) before carrying on,
//       // but we'll print out whatever response we get

//       err = http.skipResponseHeaders();
//       if (err >= 0)
//       {
//         int bodyLen = http.contentLength();
//         Serial.print("Content length is: ");
//         Serial.println(bodyLen);
//         Serial.println();
//         Serial.println("Body returned follows:");
      
//         // Now we've got to the body, so we can print it out
//         unsigned long timeoutStart = millis();
//         char c;
//         // Whilst we haven't timed out & haven't reached the end of the body
//         while ( (http.connected() || http.available()) &&
//                ((millis() - timeoutStart) < kNetworkTimeout) )
//         {
//             if (http.available())
//             {
//                 c = http.read();
//                 // Print out this character
//                 Serial.print(c);
               
//                 bodyLen--;
//                 // We read something, reset the timeout counter
//                 timeoutStart = millis();
//             }
//             else
//             {
//                 // We haven't got any data, so let's pause to allow some to
//                 // arrive
//                 delay(kNetworkDelay);
//             }
//         }
//       }
//       else
//       {
//         Serial.print("Failed to skip response headers: ");
//         Serial.println(err);
//       }
//     }
//     else
//     {    
//       Serial.print("Getting response failed: ");
//       Serial.println(err);
//     }
//   }
//   else
//   {
//     Serial.print("Connect failed: ");
//     Serial.println(err);
//   }
//   http.stop();

//   // And just stop, now that we've tried a download
//   while(1);
}