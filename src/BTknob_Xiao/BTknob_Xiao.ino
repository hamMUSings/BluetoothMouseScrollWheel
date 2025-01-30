//////////////////////////////
// BT Tunning Knob / Scroll Wheel v 1.1
// by hamMUSings
// released under  GPL-3.0 license 
// https://github.com/hamMUSings/BluetoothMouseScrollWheel
// 10/2024
/////////////////////////////

#include <BleMouse.h>
#include <Encoder.h>

//Initializing Key objects/libraries
BleMouse bleMouse;
Encoder myEnc(D8, D7);  //D01, D01

//Setup for encoder -> mouse scroll calculations
int lastpos = 0; 
int currentpos = 0;
int posabsdiff = 0;
int posdiff = 0;

//Setup for 2 millisecond 'delay' between sending BLE command or else BLE disconnects
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 2; //the value is a number of milliseconds, ie 1 second

// Varialbles for the enable button and setup button refraction period settings
#define ENABLE_LED_PIN D10
#define ENABLE_BUTTON_PIN D9
bool Enabled = true;
unsigned long pressMillis = 0;
const unsigned long buttonperiod = 1000; //the value is a number of milliseconds, ie 1 second

void setup() {

bleMouse.begin();
Serial.begin(115200);
startMillis = millis();
pressMillis = 0;
//Serial.println("Rebooted..."); //initial start time

// Initialize pins
pinMode(ENABLE_BUTTON_PIN, INPUT_PULLUP);
pinMode(ENABLE_LED_PIN, OUTPUT);
digitalWrite(ENABLE_LED_PIN, LOW);

}

void loop() {

  //Check to see what the Enable / Disable Switch is set to and toogle if pressed along with refraction period
  if (digitalRead(ENABLE_BUTTON_PIN) == LOW){
    currentMillis = millis(); 
    if (currentMillis - pressMillis >= buttonperiod){
      if (Enabled == true){
        digitalWrite(ENABLE_LED_PIN, HIGH);
        Enabled = false;
        pressMillis = millis();
        //Serial.println(Enabled);
      }
      else if (Enabled == false){
        digitalWrite(ENABLE_LED_PIN, LOW);
        Enabled = true;
        pressMillis = millis();
        myEnc.write(0); //resets encoder position to not jump 
        //Serial.println(Enabled);
      }
    }
  }
    
 //If a device is connected to BT mouse do the following
 if(bleMouse.isConnected()) {
  currentpos=myEnc.read();
  //Serial.println(currentpos);
  currentpos = currentpos /4;
  //Serial.println(currentpos);
  currentMillis = millis(); 
  if (currentMillis - startMillis >= period)  //test whether 2ms has elapsed
    {
      if (Enabled == true)
      {
      //Serial.println("Over 2ms");  
      posdiff=lastpos - currentpos;
      lastpos=currentpos;
      bleMouse.move(0,0,posdiff);
      Serial.println(currentpos);
      startMillis = millis();
      }
    }
  }
}

