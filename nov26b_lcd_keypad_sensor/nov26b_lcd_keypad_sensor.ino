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

int cont=0;
int state=0;
char sign, decimal, unit;
int SetTemp;

//sensor
void setup() {
  Serial.begin(9600);
  
  //sensor
  dht.begin();
  
  //LCD initialization
  mySerial.begin(9600);
  ClearLcd();
  mySerial.write(0x7C);  
  mySerial.write(0x9D);  //backlight fully on  
  mySerial.write(0xFE);  
  mySerial.write(0x0D); //blink cursor
  delay(500); 
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  char keypressed = myKeypad.getKey();

  //Setting temperature
  if(state==1)
  {
    if (keypressed != NO_KEY){
      switch (cont){
        case 0:
          if (keypressed == '+' || keypressed == '-'){          
            sign=keypressed;
            mySerial.print(keypressed);
            cont++;
            break;
          }
          else{
            Invalid(); 
            break;       
          }
          
        case 1:
          if (keypressed >= '0' && keypressed <= '9'){          
            decimal=keypressed;
            mySerial.print(keypressed);
            cont++;
            break;
          }
          else{
            Invalid(); 
            break;       
          }
        
         case 2:
          if (keypressed >= '0' && keypressed <= '9'){          
            unit=keypressed;
            mySerial.print(keypressed);
            cont++;
            break;
          }
          else{
            Invalid(); 
            break;       
          }
    
          //ENTER
         case 3:
          if (keypressed == 'A')
          {          
           ClearLcd();
           mySerial.print("Bachecito :D"); 
           SecondLineLcd();
           mySerial.print("Temp=");
           mySerial.print(sign);
           mySerial.print(decimal);
           mySerial.print(unit);
           mySerial.print("C");
           delay(2000);
           state=2;
           cont=0;
           break;
          }
          else{
            Invalid(); 
            break;       
          }
          
         default:
         cont=0;
         Invalid();
       }
     }  
   }
 
//State for adjusting
if(state==2)
{
  ClearLcd();
  mySerial.print("Adjusting...");
  delay(2000);
  state=0;
  keypressed = NO_KEY;
  convNums();
  Serial.print("Set temp es: ");
  Serial.print(SetTemp);
}

 //Frist state, displays temp. Checks for button press
if(state==0)
  {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    ClearLcd();
    //delay(1000);
    mySerial.print("Act. temp ");
    mySerial.print(t);
    mySerial.print("C");
    SecondLineLcd();
    mySerial.print("Humidity ");
    mySerial.print(h);
    mySerial.print("%");
    if(keypressed != NO_KEY )
    {
      state=1;
      instructions();
    }
  }
}

//Converts units
void convNums()
{
  unit = unit - 48;
  decimal = (decimal - 48) * 10; 
  SetTemp = unit + decimal;
  if(sign == '-')
  {
    SetTemp = SetTemp * -1;
  }
}

//Clear LCD
void ClearLcd()
{
  mySerial.write(0xFE);  
  mySerial.write(0x01); 
}

//Second Line LCD
void SecondLineLcd()
{
  mySerial.write(0xFE);  
  mySerial.write(0xC0);
}

//Print Instructions
void instructions()
{
  ClearLcd();
  mySerial.print("Enter sign and  two digits");
  delay(3000); 
  ClearLcd();  
  mySerial.print("Temp:");
  SecondLineLcd(); 
}

//Invalid character
void Invalid()
{
  ClearLcd();
  mySerial.print("Invalid input");
  delay(1500); 
  instructions(); 
  cont=0;
  state=1;
}

