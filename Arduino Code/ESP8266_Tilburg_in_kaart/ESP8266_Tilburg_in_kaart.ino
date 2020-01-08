//*****************************************************************************************************************************//
//Made By: Chris Minheere & Robert Donker
//Code written by: Chris Minheere
//Data by: Robert Donker
//
//https://github.com/chrisminheere
//8-01-2020
//Tilburg in kaart.
//Hardware: ESP8266, Adafruit_NeoPixel strip, 16-Channel 12-bit PWM Driver, i2C 16x2 LCD.

//*****************************************************************************************************************************//

#define BLYNK_PRINT Serial
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


char auth[] = "##";   //Authentication ky for the Blynk app
const char* KNOWN_SSID[] = {"##", "##", "##", "##", "##", "##"}; //Put all Your WiFi Network Names
const char* KNOWN_PASSWORD[] = {"##", "##", "##", "##", "##", "##"}; //Put the WiFi Passwords in same order. For Open networks leave the password blank inside the double quotes.
const int KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); // number of known networks

const int buttonPin1 = D0;
const int buttonPin2 = D3;
const int buttonPin3 = D4;
const int buttonPin4 = D5;
const int buttonPin5 = D7;
const int buttonPin6 = D8;

int buttonState1 = 1;
int buttonState2 = 1;
int buttonState3 = 1;
int buttonState4 = 1;
int buttonState5 = 1;
int buttonState6 = 1;


#define LED_PIN D6
#define LED_COUNT 11
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int input = 0;

void setup() {
  boolean wifiFound = false;
  int i, n;

  Serial.begin(9600);
  pwm.begin();
  lcd.begin();
  lcd.backlight();

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600);
  Wire.setClock(400000);

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

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected to");
  lcd.setCursor(0, 1);
  lcd.print(KNOWN_SSID[n]);

  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  Blynk.begin(auth, ssid, pass);
  strip.begin();
  strip.setBrightness(70); // 0/255
  strip.show(); // Initialize all pixels to 'off'

  Serial.println(" ");
  Serial.println("Made By: Chris Minheere & Robert Donker      8-11-2019 until 08-01-2020");
  Serial.println("Code written by: Chris Minheere");
  Serial.println("Data by: Robert Donker");
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

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);

  delay(3000);
  lcd.clear();
  lcd.print("Tilburg In Kaart");
  lcd.setCursor(5, 1);
  lcd.print("Ready");
}

void loop() {
  Blynk.run();

  if (Serial.available()) {
    input = Serial.parseInt(); //read int or parseFloat for ..float...
    checkData();
  }

  if (WiFi.status() == WL_CONNECTED) {
  }//Check WiFi connection status

  else {
    Serial.println("Error in WiFi connection");
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Error");
    lcd.setCursor(0, 0);
    lcd.print("WiFi connection");
  }

  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  buttonState5 = digitalRead(buttonPin5);
  buttonState6 = digitalRead(buttonPin6);
  delay(100);

  if (buttonState1 == LOW) {
    dataSet0();
  }
  else if (buttonState2 == LOW) {
    dataSet2();
  }
  else if (buttonState3 == LOW) {
    dataSet5();
  }
  else if (buttonState4 == LOW) {
    dataSet3();
  }
  else if (buttonState5 == LOW) {
    dataSet1();
  }
  //  else if (buttonState6 == LOW) {
  //    dataSet4();
  //  }
}

void checkData() {
  if (input == 1) {
    dataSet0();
  }
  else if (input == 2) {
    dataSet1();
  }
  else if (input == 3) {
    dataSet2();
  }
  else if (input == 4) {
    dataSet3();
  }
  else if (input == 5) {
    dataSet4();
  }
  else if (input == 6) {
    dataSet5();
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
  dataSet0();
}
BLYNK_WRITE(V1) {
  dataSet1();
}
BLYNK_WRITE(V0) {
  dataSet2();
}
BLYNK_WRITE(V2) {
  dataSet3();
}
BLYNK_WRITE(V5) {
  dataSet4();
}
BLYNK_WRITE(V3) {
  dataSet5();
}


void dataSet0() {
  Serial.println("---------------------");
  Serial.println("CO2 (Koolstofdioxide)");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");

  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("CO2 ");
  lcd.setCursor(0, 1);
  lcd.print("Koolstofdioxide");

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

  pwm.setPin(0, 4096);
  pwm.setPin(1, 0);
  pwm.setPin(2, 0);
  pwm.setPin(3, 0);
  pwm.setPin(4, 0);
  pwm.setPin(5, 0);
}

void dataSet1() {
  Serial.println("---------------------");
  Serial.println("Gasverbruik");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Gasverbruik");

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
  pwm.setPin(0, 0);
  pwm.setPin(1, 4096);
  pwm.setPin(2, 0);
  pwm.setPin(3, 0);
  pwm.setPin(4, 0);
  pwm.setPin(5, 0);
}

void dataSet2() {
  Serial.println("---------------------");
  Serial.println("Elektriciteitsverbruik");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Elektriciteits-");
  lcd.setCursor(0, 1);
  lcd.print("-verbruik");

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
  pwm.setPin(0, 0);
  pwm.setPin(1, 0);
  pwm.setPin(2, 4096);
  pwm.setPin(3, 0);
  pwm.setPin(4, 0);
  pwm.setPin(5, 0);
}

void dataSet3() {
  Serial.println("---------------------");
  Serial.println("Energielabel woningen");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Energielabel");
  lcd.setCursor(4, 1);
  lcd.print("woningen");

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
  pwm.setPin(0, 0);
  pwm.setPin(1, 0);
  pwm.setPin(2, 0);
  pwm.setPin(3, 4096);
  pwm.setPin(4, 0);
  pwm.setPin(5, 0);
}

void dataSet4() {
  Serial.println("---------------------");
  Serial.println("Laadpalen Auto's");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Laadpalen");
  lcd.setCursor(5, 1);
  lcd.print("Auto's");

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
  pwm.setPin(0, 0);
  pwm.setPin(1, 0);
  pwm.setPin(2, 0);
  pwm.setPin(3, 0);
  pwm.setPin(4, 4096);
  pwm.setPin(5, 0);
}

void dataSet5() {
  Serial.println("---------------------");
  Serial.println("Zonnestroom opgewekt");
  Serial.println("---------------------");
  Serial.println(" ");
  Serial.println(" ");

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Zonnestroom");
  lcd.setCursor(4, 1);
  lcd.print("Opgewekt");

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
  pwm.setPin(0, 0);
  pwm.setPin(1, 0);
  pwm.setPin(2, 0);
  pwm.setPin(3, 0);
  pwm.setPin(4, 0);
  pwm.setPin(5, 4096);
}


void eror() {
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("ERROR");

  for (int i = 0; i <= 5; i++) {
    pwm.setPin(i, 0);
  }

  for (int j = 0; j <= 10; j++) {
    for (int i = 0; i <= 11; i++) {
      strip.setPixelColor(i, 0, 255, 0);
    }
    strip.show();
    delay(70);
    for (int i = 0; i <= 11; i++) {
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    delay(70);
  }
  lcd.clear();
  lcd.print("Tilburg In Kaart");
  lcd.setCursor(5, 1);
  lcd.print("Ready");
}



// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {

  for (int i = 0; i <= 5; i++) {
    pwm.setPin(i, 0);
  }
  for (long firstPixelHue = 0; firstPixelHue < 1 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}
