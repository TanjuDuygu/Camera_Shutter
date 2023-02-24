/*
@Title BLE keyboard, arcade button to phone
@Author Geert Roumen
@Institute Umeå Institute of Design
@Date 29-01-2020
 
@Hardware
 
Any ESP32 dev board would do, but I used the:
Geekworm EASY KIT ESP32-C1
https://geekworm.com/products/official-geekworm-esp32-development-board

Arcade button
Adafruit Large Arcade button white
https://www.adafruit.com/product/1192

D13       Button pin
3.3V      Connected to the other leg of the button
 
In this example sketch one single button will send a spacebar when being pressed.
*/
/*
   Open Camera remote control
  Some remote control buttons and selfie sticks work by transmitting a volume key command, which by default will take a photo, but you can change this from the Settings.
  Open Camera also supports some remote control via a Bluetooth or USB keyboard:
    Function key or numeric keypad "*": toggle popup menu
    "/": toggle exposure menu
    Space or numeric keypad "5": Activate shutter (take photo/video), unless the exposure or popup menu is open, and you are in highlight selection mode (see below).
    Up/down arrows, or numeric keypad "8" and "2": if the exposure or popup menu is open, this enters highlight selection mode:
        For the popup menu, move the highlighted row up or down to select a row, then press Space/"5", then you can change the selected icon in that row with the up/down arrows, then press Space/"5" again to select the button.
        For the exposure menu, move the highlighted row up or down to select a row, then press Space/"5", then you can change the selected value or slider with the up/down arrows.
    "+" or "-": Zoom in or out.
  Source: https://opencamera.sourceforge.io/help.html
  
*/
#include <Shutter.h>
#include <BLEDevice.h>
//#include <Arduino.h>
//#include <BLEScan.h>
//#include <BLEAdvertisedDevice.h>
//Se the name of the bluetooth keyboard (that shows up in the bluetooth menu of your device)
BleKeyboard bleKeyboard("Shutter");


const int OneShut = 12;
const int LongShut = 14;
const int Video = 27;
const int StatusLed = 13;
//Set the old button state to be LOW/false; which means not pressed
boolean oldPinState = LOW;


void setup() {
 esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN, ESP_PWR_LVL_P9);  
  //Start the Serial communication (with the computer at 115200 bits per second)
  Serial.begin(115200);
  //Send this message to the computer
  Serial.println("Starting BLE work!");
  
  //Begin the BLE keyboard/start advertising the keyboard (so phones can find it)
  bleKeyboard.begin();
  //Make the button pin an INPUT_PULLDOWN pin, which means that it is normally LOW, untill it is pressed/ connected to the 3.3V
  pinMode(OneShut, INPUT_PULLDOWN);
pinMode(LongShut, INPUT_PULLDOWN);
pinMode(Video, INPUT_PULLDOWN); 
pinMode(StatusLed, OUTPUT);
digitalWrite (StatusLed, HIGH);
}

void loop() {

  if (bleKeyboard.isConnected()) {
    //if the keyboard is connected to a device
digitalWrite (StatusLed, LOW);
     if (digitalRead(LongShut) == HIGH) {
           bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
           delay(50);      
        
           }else{
            bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
           delay(50);
            }

    if (digitalRead(Video) == HIGH) {
           bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
           delay(50);      
        
           }else{
            bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
           delay(50);
            }
            
    if (digitalRead(OneShut) == HIGH) {
      //If the button pin is pressed (since it is pulled down, it is pressed when it is high
      if (oldPinState == LOW) {
        //if the old Pin state was LOW and the button pin is HIGH than...
        //send the spacebar
       bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    delay(100);  
      }
      oldPinState = HIGH;
    } else {
      oldPinState = LOW;
      digitalWrite (StatusLed, HIGH);
    }
  
  }
  //delay(10);
}