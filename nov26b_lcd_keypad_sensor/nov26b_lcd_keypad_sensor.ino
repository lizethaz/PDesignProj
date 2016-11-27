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
  mySerial.write(0x7C);  
  mySerial.write(0x9D);  //backlight fully on  
  mySerial.write(0xFE);  
  mySerial.write(0x0E);
  delay(3000); 

 // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  mySerial.print("Temperature:");
  SecondLineLcd();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  char keypressed = myKeypad.getKey();

  if (keypressed != NO_KEY)
 {
    mySerial.print(keypressed);
    
    //Delete
    if (keypressed=='A')
    {
      ClearLcd();
      mySerial.print("Temperature:");
      SecondLineLcd(); 
      }  
      
    //Enter  
    if (keypressed=='D')
    {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      }

    if (keypressed=='B' || keypressed=='C')
    {
      Invalid();
      }  
      
    }   
//    else
//    mySerial.print(keypressed); 
}

//function clear LCD
void ClearLcd()
{
  mySerial.write(0xFE);  
  mySerial.write(0x01); 
}

//function second line
void SecondLineLcd()
{
  mySerial.write(0xFE);  
  mySerial.write(0xC0);
}

void Invalid()
{
  ClearLcd();
  mySerial.print("Invalid");
  delay(1000); 
  ClearLcd();
  mySerial.print("Temperature:");
  SecondLineLcd(); 
}

//Test
