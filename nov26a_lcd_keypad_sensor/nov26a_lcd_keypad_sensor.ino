#include <SoftwareSerial.h>
#include "DHT.h"  //include sensor
#include <Keypad.h>

#define DHTPIN A4 //pin sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302)

SoftwareSerial mySerial(10, A5); // RX, TX

DHT dht(DHTPIN, DHTTYPE);

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'+', '0', '-', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte colPins[numCols] = {9,8,7,6}; //Rows 0 to 3
byte rowPins[numRows]= {5,4,3,2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//sensor
void setup() 
{
  Serial.begin(9600);
  
  //sensor
  dht.begin();
  
  //LCD
  mySerial.begin(9600);
  ClearLcd();
//  mySerial.write(0xFE);  
//  mySerial.write(0x01);  //clear display
 mySerial.write(0x7C);  
 mySerial.write(0x9D);  //backlight fully on  
  delay(3000); 

 // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();


  

  mySerial.print("Press A to set temp");
 
//  while (keypressed != NO_KEY && keypressed !='A')
//  {
//    mySerial.print("Temp=");  
//    char keypressed = myKeypad.getKey();
//    delay(500);
//    mySerial.print(keypressed);
//    Serial.print(keypressed);
//    }
 
}

void loop() 
{
  char keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY)
 {
  if(keypressed == 'D')
    ClearLcd();
  else
    mySerial.print(keypressed);
 } 
}

void ClearLcd(){
  mySerial.write(0xFE);  
  mySerial.write(0x01); 
}
