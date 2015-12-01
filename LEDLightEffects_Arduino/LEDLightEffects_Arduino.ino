// Arduino program to control Robot Lights fro Vicksburg Middle School Control Freaks
// accepts commands via serial and i2c so it can be controlled by the robot remotely
// intial creation by adevries at digtialexample.com 11/26/2015
// updated and maintined by connor at digitalexample.com

#include <Adafruit_NeoPixel.h>
#define NeoPixelDataPin 6
#define NeoPixelDataPin2 5
#include <Wire.h>

#define ModeChangePin 11
#define ColorBluePin 10
#define ColorGreenPin 9
#define ColorRedPin 8


#define I2C_Slave_Address 0x26
int pixelCount = 8;
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pixelCount * 2, NeoPixelDataPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(pixelCount, NeoPixelDataPin2, NEO_GRB + NEO_KHZ800);
 
uint8_t  mode=254,
         red = 0x00,
         green = 0xFF,
         blue = 0x00,
         brightness = 0x40,
         modeCount = 4,
         offset = 0; // Position of spinny eyes
uint32_t rainbowColor = 0x00FF00;      
uint32_t color  = 0x00FF00; // Start Green
uint32_t prevTime;
boolean allModes = false;
boolean modeComplete = false;

void setup() {

  pinMode(ModeChangePin, INPUT_PULLUP); // If toggled We Change The Mode
  digitalWrite(ModeChangePin, HIGH);       // turn on pullup resistors 
  pinMode(ColorRedPin, INPUT_PULLUP); // If Low make red FF
  digitalWrite(ColorRedPin, HIGH);       // turn on pullup resistors
  pinMode(ColorGreenPin, INPUT_PULLUP); // If Low We Make Green FF
  digitalWrite(ColorGreenPin, HIGH);       // turn on pullup resistors
  pinMode(ColorBluePin, INPUT_PULLUP); // If Low We Make Blue FF
  digitalWrite(ColorBluePin, HIGH);       // turn on pullup resistors

  Wire.begin(I2C_Slave_Address);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  pixels.begin();
  pixels.setBrightness(brightness); // 1/3 brightness
  pixels2.begin();
  pixels2.setBrightness(brightness); // 1/3 brightness
  prevTime = millis();
  Serial.println("Setup Complete");
  Serial.println("Control Freaks LED I2C Slave");
  Serial.println("Accepts Data Serial or I2C ");
  Serial.println("Must add space after the comma to work correctly");
  Serial.println("addressreg, mode, red, green, blue, brightness");
  Serial.println("0, 2, 255, 255, 0, 255");
  Serial.println("1, 255, 0, 0 // just set red, green, blue");
  Serial.println("3, 255 //set Blue to 255");
}



void loop() {
  uint8_t  i;
 
  readDigitalIoPins();
  doAutoModeChange();
  checkForSerialData();
 
  switch(mode) {
    case 0: //all lights off
      if (modeComplete == false){
        for(i=0; i<pixelCount; i++) {
          pixels.setPixelColor(i, 0);
          pixels.setPixelColor(i+pixelCount, 0);
          pixels2.setPixelColor(i, 0);
        }
        pixels.show();
        pixels2.show();
        modeComplete = true;
        Serial.println("Mode 0 turn all lights off");
      }else{
        delay(50);
      }
      break;
   case 1: //all lights on steady
       if (modeComplete == false){
          for(i=0; i <pixelCount; i++) {
            pixels.setPixelColor(i, color);
            pixels.setPixelColor(i + pixelCount, color);
            pixels2.setPixelColor(i, color);
          }
          pixels.show();
          pixels2.show();
          modeComplete = true;
          Serial.println("Mode 1 turn all lights on");
      }else{
        delay(50);
      }
      break;
  
   case 2: // Random sparks - just one LED on at a time!
      i = random(pixelCount);
      pixels.setPixelColor(i, color);
      pixels.setPixelColor(i + pixelCount, color);
      pixels.show();
      pixels2.setPixelColor(i, color);
      pixels2.show();
      delay(10);
      pixels.setPixelColor(i, 0);
      if(modeComplete == false){
        Serial.println("Mode 2 random sparks");
        modeComplete = true;
      }
      break;
      
   case 3: // Spinny wheels (8 LEDs on at a time)
      for(i=0; i<(pixelCount/2); i++) {
        uint32_t c = 0;
        if(((offset + i) & 7) < 2) c = color; // 4 pixels on...
        pixels.setPixelColor(   i, c); // First eye
        pixels.setPixelColor(pixelCount-i, c); // Second eye (flipped)
        pixels.setPixelColor(pixelCount-(i+pixelCount), c); // Second eye (flipped)
        pixels2.setPixelColor(   i, c); // First eye
        pixels2.setPixelColor(pixelCount-i, c); // Second eye (flipped)
      }
      pixels.show();
      offset++;
      delay(100);
      if(modeComplete == false){
        Serial.println("Mode 3 Spinny Wheels");
        modeComplete = true;
      }
      break;
    case 4: // rainbow
      rainbowColor = rainbowColor << 8;
        if (rainbowColor == 0){
          rainbowColor = 0xFF;
        }
      for(i=0; i<(pixelCount); i++) {
        
        pixels.setPixelColor(i, rainbowColor);
        pixels.setPixelColor(i + pixelCount, rainbowColor);
        pixels2.setPixelColor(i, rainbowColor); // Second eye (flipped)
        rainbowColor = rainbowColor << 8;
        if (rainbowColor == 0){
          rainbowColor = 0xFF;
        }
      }
      pixels.show();
      pixels2.show();
      delay(250);
      break;
    case 254:
      //we start in the mode which turns on first three leds red, green, blue so we we know it has started
      if (modeComplete == false){
        pixels.setPixelColor(0, 0xFF0000);
        pixels.setPixelColor(1, 0x00FF00);
        pixels.setPixelColor(2, 0x0000FF);
        pixels.setPixelColor(3, 0xFF0000);
        pixels.setPixelColor(4, 0x00FF00);
        pixels.setPixelColor(5, 0x0000FF);
        pixels.setPixelColor(6, 0xFF0000);
        pixels.setPixelColor(7, 0x00FF00);
        pixels.setPixelColor(8, 0x0000FF);
        pixels.setPixelColor(9, 0xFF0000);
        pixels.setPixelColor(10, 0x00FF00);
        pixels.setPixelColor(11, 0x0000FF);
        pixels.setPixelColor(12, 0xFF0000);
        pixels.setPixelColor(13, 0x00FF00);
        pixels.setPixelColor(14, 0x0000FF);
        pixels.setPixelColor(15, 0xFF0000);
        pixels.setPixelColor(16, 0x00FF00);
        pixels2.setPixelColor(0, 0xFF0000);
        pixels2.setPixelColor(1, 0x00FF00);
        pixels2.setPixelColor(2, 0x0000FF);
        pixels2.setPixelColor(3, 0xFF0000);
        pixels2.setPixelColor(4, 0x00FF00);
        pixels2.setPixelColor(5, 0x0000FF);
        pixels2.setPixelColor(6, 0xFF0000);
        pixels2.setPixelColor(7, 0x00FF00);
        pixels2.setPixelColor(8, 0x0000FF);
        
        pixels.show();
        pixels2.show();
        if(modeComplete == false){
          Serial.println("Mode 254 turn on first 3 RGB");
          modeComplete = true;
        }
        modeComplete = true;
      }
      if (modeComplete == true){
        //add a delay so we don't wind our loop to tight
        delay(50);
      }
      break;
  }
    
  
}

uint32_t getColor(byte red, byte green, byte blue){
  uint32_t newColor;
  newColor =  ((uint32_t)blue & 0xFFFFFFFF);
  newColor = newColor | (((uint32_t)green) << 8);
  newColor =  newColor | (((uint32_t)red) << 16);
  return newColor;
}


void requestEvent() {
      byte registers[] = {mode,red,green,blue,brightness};
      Wire.write(registers,5);
  // as expected by master
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
    
    byte dataPosition = 0;
    boolean receivedData = false;
    if(Wire.available()>1) { 
      //its a write to a register
      dataPosition = Wire.read();
      Serial.println("dataposition " + String(dataPosition));
    }
    while (Wire.available() > 0) { // loop through all the data
      byte byteData = Wire.read();
      Serial.println("dataposition " + String(dataPosition) + ":" + String(byteData));
      receivedData = true;
      setData(dataPosition, byteData);
      dataPosition++;
    }
    Serial.println("I2C Changed values color:" + String(color,HEX) + ", mode:" + String(mode) + ", red:" + String(red) + ", green:" + String(green) + ", blue:" + String(blue)  + ", brightness:" + String(brightness));
}


void setData(byte dataPosition, byte byteData){
  Serial.println("setData( " + String(dataPosition) + "," + String(byteData) + ")");
  switch(dataPosition){
        case 0: //first comes a zero
          mode = byteData;
          if (mode == 255){
            allModes = true;
            mode = 1;
            modeComplete = false;
          }else{
            allModes = false;
            modeComplete = false;
          }
          //if invalid mode turn off
          if (mode > modeCount){
            mode = 0;
            Serial.println("Mode > modeCount! mode = 0");
          }
          Serial.println("mode:" + String(mode) + "," + String(byteData));
          break;
        case 1: //second comes red
            red = byteData;
            color = getColor(red,green,blue);
            Serial.println("red:" + String(color, HEX) + "," +String( byteData));
          break;
        case 2: //third comes green
            green = byteData;
            color = getColor(red,green,blue);
            Serial.println("green:" + String(color, HEX) + "," + String(byteData));
          break;
        case 3: //forth comes blue
            blue = byteData;
            Serial.println("blue:" + String(color, HEX) + "," + String(byteData));
            color = getColor(red,green,blue);
          break;
        case 4: //fith comes brightness
          brightness = byteData;
          Serial.println("brightness:" + String(byteData));
          break;
      }
}

boolean lastChangeMode = false;
boolean lastChangeColor = false;

void readDigitalIoPins(){
  boolean changeMode = (digitalRead(ModeChangePin) == LOW);
  boolean ColorRed = (digitalRead(ColorRedPin) == LOW);
  boolean ColorGreen = (digitalRead(ColorGreenPin) == LOW);
  boolean ColorBlue = (digitalRead(ColorBluePin) == LOW);
  
  if (changeMode != lastChangeMode){
    lastChangeMode = changeMode;
    allModes = false;
    modeComplete = false;
    mode++;                        // Next mode
    if(mode > modeCount) {                 // End of modes?
      mode = 1;                    // Start modes over
    }
    color = getColor((ColorRed ? 0xFF : 0x00), (ColorGreen ? 0xFF : 0x00), (ColorBlue ? 0xFF : 0x00));
    Serial.println("digitalio:" + String(mode) + ":" + String(ColorRed) + ":" +  String(ColorGreen) + ":" + String(ColorBlue) );
  }
//  if (changeColor != lastChangeColor){
//    lastChangeColor = changeColor;
//    if(color == 0xFF0000){
//      color = 0x0000FF; // if red make it blue
//    }else{
//      color = 0xFF0000; // Reset to red
//      
//    } 
//  }
   
}

void  doAutoModeChange() {
  uint8_t  i;
  uint32_t t;
  if (allModes == true){
    t = millis();
    if((t - prevTime) > 8000) {      // Every 8 seconds...
      mode++;                        // Next mode
      if(mode > modeCount) {                 // End of modes?
        mode = 1;                    // Start modes over
        color >>= 8;                 // Next color R->G->B
        if(!color) color = 0xFF0000; // Reset to red
      }
      for(i=0; i<pixelCount; i++) pixels.setPixelColor(i, 0);
      prevTime = t;
      modeComplete = false;
    }
  }
}

byte serialbuffer[6];
void checkForSerialData(){
    int counter = 0;
    int dataPosition = 0;
    while (Serial.available() > 0) {

      int dataByte = Serial.parseInt();
      dataByte =  constrain(dataByte,0,255);
      Serial.println("got " + String(dataByte));
      serialbuffer[counter] = (byte) dataByte;
      counter++;
      // look for the newline. That's the end of your
      // sentence:
      if (Serial.read() == '\n' || Serial.read() == '\r') {
          dataPosition =  serialbuffer[0];
          
          for(int i = 1; i<counter; i++){
              byte byteValue = serialbuffer[i];
              setData(dataPosition, byteValue);
              dataPosition++;
          }
           Serial.println("Serial Changed values color:" + String(color,HEX) + ", mode:" + String(mode) + ", red:" + String(red) + ", green:" + String(green) + ", blue:" + String(blue)  + ", brightness:" + String(brightness));   
      }
    }
}



