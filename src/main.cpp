/*
  Titre      : Exam final 
  Auteur     : Anis Aliouachene
  Date       : 09/12/2022
  Description: Alimentation moteur et arduino externe 
  Version    : 0.0.1
*/

// Inculre les libraire necessaire 
#include <Arduino.h>
#include<WIFIConnector_MKR1010.h>
#include<MQTTConnector.h>
#include <Adafruit_NeoPixel.h>

#define PIN_LED 4     // Control signal, connect to DI of the LED
#define NUM_LED 1     // Number of LEDs in a strip

int inputPin = 2;    // input pour le capteur 
int val = 0, ValPrecedent = 0;  // valeur capter par le capteur 
const int RELAY_PIN = 1;  // broche a la quelle le relais se branche 
String Data;                  // variable for reading the pin status
int StatusMotor, StatusRGB;  // status motor et rgb 

// FGonction pour la LED RGB 
void colorWipe(uint32_t c, uint16_t wait); 

// Declaration d'objet 
Adafruit_NeoPixel RGB_Strip = Adafruit_NeoPixel(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);

void setup() {
  // Connection wifi et MQTT 
  wifiConnect(); 
  MQTTConnect();

  pinMode(inputPin, INPUT);   // Inpur pour le capteur 
  pinMode(RELAY_PIN, OUTPUT);    // declare pushbutton as input

  // Start RGB 
  RGB_Strip.begin();
  RGB_Strip.show();
  RGB_Strip.setBrightness(128);    // Set brightness, 0-255 (darkest - brightest)
}

void loop(){
  
  // mqtt loop 
  ClientMQTT.loop();

  val = digitalRead(inputPin);  // read input value
  
  // ENVOI si la valeur est differentes 
  if ( val != ValPrecedent)
  { 

     if (val == 1 )
     {
      StatusMotor = 1; 
      StatusRGB = 1;
     }else {
      StatusMotor = 0; 
      StatusRGB = 0; 
     }
     appendPayload("CptVal", val);
     appendPayload("StatusMotor", StatusMotor);
     appendPayload("StatusLEDRGB", StatusRGB);
     sendPayload(); 
     ValPrecedent = val; 
     
  }
  
  // actionne motor et LED RBG selon la valeur recu par thingsboard 
    if (DataRecu != Data)
  {
    Data = DataRecu; 
    if (DataRecu == "ON")
    {
      digitalWrite(RELAY_PIN, HIGH);  // Mettre le relay on high pour actionner le motor 
      colorWipe(RGB_Strip.Color(0, 255, 0), 1000);  // Green 
    }else if (DataRecu == "OFF")
    {
      digitalWrite(RELAY_PIN, LOW); // Mettre le relay on LOW pour arreter le motor
      colorWipe(RGB_Strip.Color(255, 0, 0), 1000);  // Red 
    }
    
}
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for (uint16_t i = 0; i < RGB_Strip.numPixels(); i++) {
    RGB_Strip.setPixelColor(i, c);
    RGB_Strip.show();
    delay(wait);
  }
}




