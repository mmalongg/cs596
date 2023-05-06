#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <Wire.h>
#include <string.h>
#include <MFRC522.h>
#include <SPI.h>
#include "SPIFFS.h"
#include <FS.h>
#include <Adafruit_CAP1188.h> // capacitive touch sensor
#include <Servo.h>
#include <time.h>


#define servo_PIN 2
#define RST_PIN 32 // Configurable, see typical pin layout above
#define CS_PIN 33 // Configurable, see typical pin layout above, aka SDA pin on rfid reader
#define SCK 25
#define MISO 27
#define MOSI 26

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0; // Adjust according to your timezone
const int   daylightOffset_sec = 0; // Adjust according to your DST (Daylight Saving Time)

char ssid[] = "Fadi's Galaxy S22 Ultra";    // your network SSID (name) 
char pass[] = "19981998"; // your network password (use for WPA, or use as key for WEP)
String path = "";

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

// CAP1188 connection
#define CAP1188_I2C_ADDR 0x29
Adafruit_CAP1188 cap = Adafruit_CAP1188();

String txt = "Place an RFID card near the reader:";
bool lockUnlocked = false;
bool attempt = false;
const int lineHeight = 20;
int currentLine = 0;
const int LOCKED_POS = 0;
const int UNLOCKED_POS = 90;
const uint8_t NUM_OF_BYTES_IN_KEY = 4;
const byte predefinedKey[NUM_OF_BYTES_IN_KEY] = {0x71, 0x3F, 0xA3, 0x89};

Servo myservo; // create servo object to control a servo
MFRC522 mfrc522(CS_PIN, RST_PIN); // Create MFRC522 instance
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

bool compareKeys(byte key1[], const byte key2[], uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    if (key1[i] != key2[i]) {
      return false;
    }
  }
  return true;
}

void printToTFT(const String &text) {
  if (currentLine * lineHeight + lineHeight > tft.height() || text == txt) {
    tft.fillScreen(TFT_BLACK);
    currentLine = 0;
  }
  tft.setCursor(0, currentLine * lineHeight);
  tft.println(text);
  if (text == txt)
    currentLine += 2;
  else
    currentLine++;
}

void unlock() {
  myservo.write(UNLOCKED_POS);
  Serial.println("Lock unlocked");
  printToTFT("Lock unlocked");
  delay(3000);
  myservo.write(LOCKED_POS);
  Serial.println("Lock locked");
  printToTFT("Lock locked");
  delay(3000);
  printToTFT(txt);
}

String getLocalTimeStr() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "Failed to obtain time";
  }

  char dateTimeStr[20];
  strftime(dateTimeStr, sizeof(dateTimeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(dateTimeStr);
}
void setup() {
  Serial.begin(115200); // Initialize serial communications with the PC
  while (!Serial); // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  myservo.attach(servo_PIN); // attaches the servo on port 27 to the servo object
  myservo.write(LOCKED_POS);
  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, HIGH);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  tft.init();
  tft.setRotation(1); // Set the display orientation
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  printToTFT(txt);

  SPI.begin(SCK, MISO, MOSI);            // Init SPI bus CLK, MISO, MOSI
  mfrc522.PCD_Init();     // Init MFRC522
  delay(4);               // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  // Initialize I2C for CAP1188
  Wire.begin();
  if (!cap.begin(CAP1188_I2C_ADDR)) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  bool rfidDetected = false;
  bool capDetected = false;

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("Card UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      if (i < mfrc522.uid.size - 1) {
        Serial.print(", ");
      }
    }
    Serial.println();

    if (compareKeys(mfrc522.uid.uidByte, predefinedKey, NUM_OF_BYTES_IN_KEY)) {
      Serial.println("Key matched!");
      printToTFT("Key matched!");
      rfidDetected = true;
      attempt = true;
    } else {
      Serial.println("Key did not match");
      printToTFT("Key did not match");
      printToTFT("Try again");
      attempt = true;
      delay(3000);
      printToTFT(txt);
    }
    mfrc522.PICC_HaltA();
  }

  uint8_t touched = cap.touched();
  for (uint8_t i = 0; i < 8; i++) {
    if (touched & (1 << i)) {
      // Serial.print("Button ");
      // Serial.print(i + 1);
      // Serial.println(" touched");
      capDetected = true;
    }
  }

  if (rfidDetected && capDetected) {
    unlock();
    attempt = true;
    lockUnlocked = true;
  }

  if (attempt == true){

  if (lockUnlocked == true) {
    path = kPath +  String("Successful%20attempt%20at:%20" + getLocalTimeStr());

  }
  else {
    path = kPath + String("Failed attempt at: " + getLocalTimeStr());

  }
  path.replace(" ", "%20");
  Serial.println(path);
  int err =0;
  
  WiFiClient c;
  HttpClient http(c);
  
  err = http.get(kHostname, kPort, path.c_str());

  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();

    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                Serial.print(c);
               
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
        }
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();
 }
 attempt = false;
 lockUnlocked = false;
}

