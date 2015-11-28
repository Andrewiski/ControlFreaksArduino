// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#define I2C_Slave_Address 0x26
void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
}

int counter = 0;
int counterMax = 2;

byte buffer[6];

void loop() {

  while (Serial.available() > 0) {

    int dataByte = Serial.parseInt();
    buffer[counter] = (byte) constrain(dataByte,0,255);
    counter++;
    // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n') {
        Wire.beginTransmission(I2C_Slave_Address); // transmit to device
        if (counter > 0)
          Wire.write(buffer[0]);        // register 
        if (counter > 1)
          Wire.write(buffer[1]);        // data 1
        if (counter > 2)
          Wire.write(buffer[2]);        // data 2
        if (counter > 3)
          Wire.write(buffer[3]);        // data 3
        if (counter > 4)
          Wire.write(buffer[4]);        // send blue
        if (counter > 5)
          Wire.write(buffer[0]);        // send brightness 255 is the brightess
        Wire.endTransmission();    // stop transmitting
        Serial.println("wrote data " + String(counter) + " bytes");
        Wire.requestFrom(I2C_Slave_Address, 5);    // request 6 bytes from slave device #8
        while (Wire.available()) { // slave may send less than requested
          byte c = Wire.read(); // receive a byte as character
          Serial.print(String(c)+ ",");         // print the character
        }
        counter=0;
    }
    
  }
  
//  switch (counter){
//    case 0:
//        Wire.beginTransmission(I2C_Slave_Address); // transmit to device 0xa0
//        Wire.write(0x00);        // register 0x00
//        Wire.write(0x03);        // sendmode
//        Wire.write(0x00);              // send red
//        Wire.write(0xFF);              // send green
//        Wire.write(0xFF);              // send blue
//        Wire.write(0x80);              // send brightness 128 is the brightness
//        Serial.println("wrote 0,3,0,255,255,128");
//        Wire.endTransmission();    // stop transmitting
//        
//        break;
//     case 1:
//        Wire.beginTransmission(I2C_Slave_Address); // transmit to device 0xa0
//        Wire.write(0x00);        // register 0x00
//        Wire.write(0x01);        // sendmode
//        Wire.write(0xFF);              // send red
//        Wire.write(0x00);              // send green
//        Wire.write(0xFF);              // send blue
//        Wire.write(0xFF);              // send brightness 255 is the brightess
//        Wire.endTransmission();    // stop transmitting
//        Serial.println("wrote 0,1,255,0,255,255");
//        break;
//     case 2:
//        Wire.beginTransmission(I2C_Slave_Address); // transmit to device 0xa0
//        Wire.write(0x00);        // register 0x00
//        Wire.write(0x02);        // sendmode
//        Wire.write(0xFF);              // send red
//        Wire.write(0xFF);              // send green
//        Wire.write(0x00);              // send blue
//        Wire.write(0x00);              // send brightness 255 is the brightess
//        Wire.endTransmission();    // stop transmitting
//        Serial.println("wrote 0,2,255,255,0,255");
//        break;
//  }
  
  
   
  
}
