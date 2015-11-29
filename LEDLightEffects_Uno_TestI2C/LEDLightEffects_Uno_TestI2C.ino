// Writes data to an I2C/TWI slave device
// Slave Device is the Control Freaks LED Light Arduino

// Created 28 November 2015

// 


#include <Wire.h>
#define I2C_Slave_Address 0x26
void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.println("Setup Complete");
  Serial.println("Must add space after the comma to work correctly");
  Serial.println("ic2reg, mode, red, green, blue, brightness");
  Serial.println("0, 2, 255, 255, 0, 255");
}

int counter = 0;
int counterMax = 2;

byte outbuffer[6];

void loop() {

  while (Serial.available() > 0) {

    int dataByte = Serial.parseInt();
    Serial.println("got " + String(dataByte));
    outbuffer[counter] = (byte) constrain(dataByte,0,255);
    counter++;
    // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n' || Serial.read() == '\r') {
        Wire.beginTransmission(I2C_Slave_Address); // transmit to device
//        if (counter > 0)
//          Wire.write(buffer[0]);        // register 
//        if (counter > 1)
//          Wire.write(buffer[1]);        // data 1
//        if (counter > 2)
//          Wire.write(buffer[2]);        // data 2
//        if (counter > 3)
//          Wire.write(buffer[3]);        // data 3
//        if (counter > 4)
//          Wire.write(buffer[4]);        // send blue
//        if (counter > 5)
//          Wire.write(buffer[5]);        // send brightness 255 is the brightess
        Wire.write(outbuffer,counter);
        Wire.endTransmission();    // stop transmitting
        Serial.println("wrote data " + String(counter) + " bytes");
        Wire.requestFrom(I2C_Slave_Address, 5);    // request 5 bytes from
       
        int receiveCount = 0;
        while (Wire.available()) { // slave may send less than requested
          if(receiveCount > 0){
            Serial.print(",");         // print the character
          }
          receiveCount++;
          byte c = Wire.read(); // receive a byte as character
          Serial.print(String(c));         // print the character
        }
        if (receiveCount > 0){
          Serial.println("\n " + String(receiveCount) + " bytes received from I2C Slave");
        }
        counter=0;
    }
    
  }
  delay(50);

  
  
   
  
}
