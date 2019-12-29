//*****************************************************************************************************************************//
//Made By: Chris Minheere & Robert Donker
//Code written by: Chris Minheere
//
//https://github.com/chrisminheere
//8-11-2019
//Tilburg in kaart.
//Hardware: Arduino Uno, Adafruit_NeoPixel strip.


//Sketch uses 6450 bytes (19%) of program storage space. Maximum is 32256 bytes.
//Global variables use 452 bytes (22%) of dynamic memory, leaving 1596 bytes for local variables. Maximum is 2048 bytes.
//*****************************************************************************************************************************//

#define BLYNK_PRINT Serial

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "wcbPWzIFxP_SgWoQgSapkyh-UKsMaudT";   //Authentication ky for the Blynk app /Robert///29f716f65f2c4da59e006a0bb2a33440 /Chris///ab2d125aaa7440d58e7ef38e8f02a07f
const char* KNOWN_SSID[] = {"Fam-Minheere 2.4GHz", "Fam D. Tuin", "Fam Donker", "Fam D. boven", "PowerMoto", "BlynkKar"}; //Put all Your WiFi Network Names
const char* KNOWN_PASSWORD[] = {"Minheere@asus", "Donk1966", "Donk1966", "Donk1966", "Power12!!", "BlynkKar"}; //Put the WiFi Passwords in same order. For Open networks leave the password blank inside the double quotes.
const int KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); // number of known networks






//char json[] = "[{\"button\":\"test\",\"aantal\":1}]";
//DynamicJsonDocument doc(1024);
//deserializeJson(doc, json);









#define LED_PIN D6
#define LED_COUNT 11
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int input = 0;

void setup() {
  boolean wifiFound = false;
  int i, n;

  Serial.begin(9600);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");

  // WiFi.scanNetworks will return the number of networks found
  Serial.println(F("Scan start"));
  int nbVisibleNetworks = WiFi.scanNetworks();
  Serial.println(F("Scan Completed"));
  if (nbVisibleNetworks == 0) {
    Serial.println(F("No networks found. Reset to try again"));
    while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
  }

  // if you arrive here at least some networks are visible
  Serial.print(nbVisibleNetworks);
  Serial.println(" network(s) found");

  // check if we recognize one by comparing the visible networks
  // one by one with our list of known networks
  for (i = 0; i < nbVisibleNetworks; ++i) {
    Serial.println(WiFi.SSID(i)); // Print current SSID
    for (n = 0; n < KNOWN_SSID_COUNT; n++) { // walk through the list of known SSID and check for a match
      if (strcmp(KNOWN_SSID[n], WiFi.SSID(i).c_str())) {
        Serial.print(F("\tNot matching "));
        Serial.println(KNOWN_SSID[n]);
      } else { // we got a match
        wifiFound = true;
        break; // n is the network index we found
      }
    } // end for each known wifi SSID
    if (wifiFound) break; // break from the "for each visible network" loop
  }

  if (!wifiFound) {
    Serial.println(F("No Known Network identified. Reset to try again"));
    while (true);
  }

  const char* ssid = (KNOWN_SSID[n]);
  const char* pass = (KNOWN_PASSWORD[n]);
  Serial.println(WiFi.localIP());


  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  Blynk.begin(auth, ssid, pass);
  strip.begin();
  strip.setBrightness(70); // 0/255
  strip.show(); // Initialize all pixels to 'off'

  Serial.println(" ");
  Serial.println("Made By: Chris Minheere & Robert Donker      8-11-2019");
  Serial.println("Code written by: Chris Minheere");
  Serial.println("https://github.com/chrisminheere");
  Serial.println(" ");
  Serial.println(" ");

  Serial.println("_____legenda_____");
  Serial.println(" ");
  Serial.println("1 = CO2 (Koolstofdioxide)");
  Serial.println("2 = Gasverbruik per huishoudens");
  Serial.println("3 = Elektriciteitsverbruik per huishoudens");
  Serial.println("4 = Energielabel woningen");
  Serial.println("5 = Laadpalen Auto's");
  Serial.println("6 = Zonnestroom opgewekt");
  Serial.println("_________________");
  Serial.println(" ");
  Serial.println(" ");
}

void loop() {
  Blynk.run();

  if (Serial.available()) {
    input = Serial.parseInt(); //read int or parseFloat for ..float...
    checkData();
  }










  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http; //Declare object of class HTTPClient


    //    const size_t CAPACITY = JSON_OBJECT_SIZE(2);
    //    StaticJsonDocument<CAPACITY> doc;
    StaticJsonDocument<200> doc;
    JsonArray array = doc.to<JsonArray>();
    JsonObject nested = array.createNestedObject();
    nested["button"] = "test";
    nested["aantal"] = 1;
    serializeJson(array, Serial);

char JSONmessageBuffer[300];
size_t serializeJsonPretty(const array& doc, String& output);
    Serial.println(JSONmessageBuffer);

    http.begin("https://api.powerbi.com/beta/9de629ec-ffcf-4793-b6d1-0cad7e8d74aa/datasets/a3e58707-f4f3-44ba-b106-6066a76ed38b/rows?tenant=9de629ec-ffcf-4793-b6d1-0cad7e8d74aa&UPN=RoDo%40finext.nl&key=A3CKElZcciICPkSt18fVq4cLbPxpDghCnlnkjxXGIfKVTJG%2BQ0W6tA4boe8hUF6dm66NAGv%2FhXYO5MV5QPWu9g%3D%3D"); //Specify request destination
    http.addHeader("Content-Type", "application/json"); //Specify content-type header
    int httpCode = http.POST(JSONmessageBuffer); //Send the request
    String payload = http.getString();  //Get the response payload

    Serial.println(httpCode); //Print HTTP return code
    Serial.println(payload); //Print request response payload


    http.end();  //Close connection
  }
  else {
    Serial.println("Error in WiFi connection");
  }
  delay(500);  //Send a request every 30 seconds

}











void checkData() {
  if (input == 1) {
    dataSet1();
    Serial.println("---------------------");
    Serial.println("CO2 (Koolstofdioxide)");
    Serial.println("---------------------");
    Serial.println(" ");
    Serial.println(" ");
  }

  else if (input == 2) {
    dataSet2();
    Serial.println("---------------------");
    Serial.println("Gasverbruik per huishoudens");
    Serial.println("---------------------");
    Serial.println(" ");
    Serial.println(" ");
  }

  else if (input == 3) {
    dataSet3();
    Serial.println("---------------------");
    Serial.println("Elektriciteitsverbruik per huishoudens");
    Serial.println("---------------------");
    Serial.println(" ");
    Serial.println(" ");
  }

  else if (input == 4) {
    dataSet4();
    Serial.println("---------------------");
    Serial.println("Energielabel woningen");
    Serial.println("---------------------");
    Serial.println(" ");
    Serial.println(" ");
  }

  else if (input == 5) {
    dataSet5();
    Serial.println("---------------------");
    Serial.println("Laadpalen Auto's");
    Serial.println("---------------------");
    Serial.println(" ");
    Serial.println(" ");
  }

  else if (input == 6) {
    dataSet6();
    Serial.println("---------------------");
    Serial.println("Zonnestroom opgewekt");
    Serial.println("---------------------");
    Serial.println(" ");
    Serial.println(" ");
  }


  else if (input == 0) {
    eror();
  }

  else if (input == 9) {
    rainbow(20);
  }

  else {
    eror();
  }

}


BLYNK_WRITE(V4) {
  dataSet1();
  Serial.println("---------------------");
  Serial.println("CO2 (Koolstofdioxide)");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");
}

BLYNK_WRITE(V1) {
  dataSet2();
  Serial.println("---------------------");
  Serial.println("Gasverbruik per huishoudens");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");
}

BLYNK_WRITE(V0) {
  dataSet3();
  Serial.println("---------------------");
  Serial.println("Elektriciteitsverbruik per huishoudens");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");
}

BLYNK_WRITE(V2) {
  dataSet4();
  Serial.println("---------------------");
  Serial.println("Energielabel woningen");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");
}

BLYNK_WRITE(V5) {
  dataSet5();
  Serial.println("---------------------");
  Serial.println("Laadpalen Auto's");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");
}

BLYNK_WRITE(V3) {
  dataSet6();
  Serial.println("---------------------");
  Serial.println("Zonnestroom opgewekt");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");
}


void dataSet1() {
  strip.setPixelColor(0, 0, 0, 255);
  strip.setPixelColor(1, 255, 0, 75);
  strip.setPixelColor(7, 245, 10, 0);
  strip.setPixelColor(9, 243, 12, 0);
  strip.setPixelColor(8, 239, 16, 0);
  strip.setPixelColor(2, 238, 17, 0);
  strip.setPixelColor(4, 237, 18, 0);
  strip.setPixelColor(5, 235, 20, 0);
  strip.setPixelColor(10, 230, 25, 0);
  strip.setPixelColor(6, 213, 42, 0);
  strip.setPixelColor(3, 0, 255, 0);
  strip.show();
}

void dataSet2() {
  strip.setPixelColor(0, 128, 255, 0);
  strip.setPixelColor(1, 0, 0, 0);
  strip.setPixelColor(2, 255, 0, 75);
  strip.setPixelColor(9, 230, 25, 0);
  strip.setPixelColor(8, 221, 34, 0);
  strip.setPixelColor(7, 184, 71, 0);
  strip.setPixelColor(5, 160, 95, 0);
  strip.setPixelColor(10, 148, 107, 0);
  strip.setPixelColor(4, 145, 110, 0);
  strip.setPixelColor(6, 107, 148, 0);
  strip.setPixelColor(3, 0, 255, 0);
  strip.show();
}

void dataSet3() {
  strip.setPixelColor(0, 0, 255, 0);
  strip.setPixelColor(10, 145, 110, 0);
  strip.setPixelColor(8, 255, 0, 50);
  strip.setPixelColor(9, 208, 47, 0);
  strip.setPixelColor(1, 16, 239, 0);
  strip.setPixelColor(5, 139, 116, 0);
  strip.setPixelColor(4, 197, 58, 0);
  strip.setPixelColor(7, 195, 60, 0);
  strip.setPixelColor(3, 0, 255, 0);
  strip.setPixelColor(2, 186, 69, 0);
  strip.setPixelColor(6, 49, 206, 0);
  strip.show();
}

void dataSet4() {
  strip.setPixelColor(0, 200, 255, 0);
  strip.setPixelColor(1, 255, 0, 75);
  strip.setPixelColor(3, 163, 92, 0);
  strip.setPixelColor(2, 145, 110, 0);
  strip.setPixelColor(9, 129, 126, 0);
  strip.setPixelColor(5, 89, 166, 0);
  strip.setPixelColor(7, 89, 166, 0);
  strip.setPixelColor(10, 72, 183, 0);
  strip.setPixelColor(8, 69, 186, 0);
  strip.setPixelColor(4, 67, 188, 0);
  strip.setPixelColor(6, 0, 255, 0);
  strip.show();
}

void dataSet5() {
  strip.setPixelColor(0, 0, 255, 255);
  strip.setPixelColor(10, 0, 255, 0);
  strip.setPixelColor(1, 3, 252, 0);
  strip.setPixelColor(8, 12, 243, 0);
  strip.setPixelColor(9, 27, 228, 0);
  strip.setPixelColor(2, 32, 223, 0);
  strip.setPixelColor(7, 44, 211, 0);
  strip.setPixelColor(5, 62, 193, 0);
  strip.setPixelColor(4, 67, 188, 0);
  strip.setPixelColor(6, 90, 165, 0);
  strip.setPixelColor(3, 255, 0, 75);
  strip.show();
}

void dataSet6() {
  strip.setPixelColor(0, 255, 0, 0);
  strip.setPixelColor(8, 0, 255, 0);
  strip.setPixelColor(7, 7, 248, 0);
  strip.setPixelColor(5, 29, 226, 0);
  strip.setPixelColor(4, 31, 224, 0);
  strip.setPixelColor(6, 56, 199, 0);
  strip.setPixelColor(2, 76, 179, 0);
  strip.setPixelColor(9, 79, 176, 0);
  strip.setPixelColor(1, 127, 128, 0);
  strip.setPixelColor(10, 160, 95, 0);
  strip.setPixelColor(3, 255, 0, 75);
  strip.show();
}






void eror() {
  for (int j = 0; j <= 10; j++) {

    for (int i = 0; i <= 9; i++) {
      strip.setPixelColor(i, 0, 255, 0);
    }
    strip.show();
    delay(70);
    for (int i = 0; i <= 9; i++) {
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    delay(70);
  }
}



// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  for (long firstPixelHue = 0; firstPixelHue < 1 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}
