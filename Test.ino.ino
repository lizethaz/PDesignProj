/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>
#include "DHT.h"  //include sensor

#define DHTPIN A4 //pin sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302)

SoftwareSerial mySerial(10, A5); // RX, TX

DHT dht(DHTPIN, DHTTYPE);

//sensor
void setup() {
    //sensor
    dht.begin();
    
    //LCD
    mySerial.begin(9600);
    mySerial.write(0xFE);  
    mySerial.write(0x01);  //clear display
    mySerial.write(0x7C);  
    mySerial.write(0x9D);  //backlight fully on  
    delay(3000); 
}

void loop() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) {
        mySerial.print("Failed to read from DHT");
    } 
    else {
         mySerial.write(0xFE);  
         mySerial.write(0x01);  //clear display
         mySerial.write(0x7C);  
         mySerial.write(0x9D);  //backlight fully on
         
         delay(1000);
         mySerial.print("Temp:");
         mySerial.print(t);
         mySerial.print("*C");
                  
         mySerial.write(0xFE);  
         mySerial.write(0xC0);  //second line LCD
         
         mySerial.print("Humidity:");
         mySerial.print(h);
         mySerial.print("%");
                 
         delay(2000);  
         //Clears Display
         mySerial.write(0xFE);  
         mySerial.write(0x01); 
    
    }
}


