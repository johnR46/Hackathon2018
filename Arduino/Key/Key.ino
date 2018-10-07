
/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

const byte ROWS = 1; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
    {'1','2','3','4'}
};

byte rowPins[ROWS] = {}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9,10,11,12}; //connect to the column pinouts of the keypad

Keypad Key = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );; 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKeypad = Key.getKey();
  
  if (customKeypad){
    Serial.println(customKeypad);
  }
}
