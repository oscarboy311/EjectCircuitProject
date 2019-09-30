#include <Adafruit_BMP085.h>
const int ledPin = 13; //assign an int value to the led pin number
const int eFuse = 12; //assign an int value to the electric fuse 
const float threshold = 100;
const float presetAltitude = 0;

float bias = 0;

Adafruit_BMP085 bmp;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  while (1)  {
    Serial.begin(9600); 
    if (!bmp.begin()) {
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }
    else { 
      digitalWrite(ledPin, HIGH); //power on for the pew signal
      break;
    }
  }
} 

void loop() {
  // Set Bias
  bias = getBias();
  Serial.print("Bias = ");
  Serial.print(bias);
  Serial.println(" meters");

  // Event Threshold (Apogee)
  float currReading = 0;
  do{
    // Read Sensor
    currReading = getAltitude();
  }
  while(currReading < threshold);
  Serial.println("Threshold reached!");

  // Verify descent
  float tally = 0;
  float prevReading = 0;
  bool descending = false;
  while(!descending){
    prevReading = getAltitude();
    delay(100);
    currReading = getAltitude();
    if(currReading < prevReading){
      tally++;
      if(tally > 30){
        descending = true;
      }
    }
  }
  Serial.println("Rocket is descending!");

  // Activate Relay + Turn on LED
  digitalWrite(eFuse, HIGH);

  // Verify if preset altitude has been reached
  bool presetReached = false;
  while(!presetReached){
    currReading = getAltitude();
    if(currReading < presetAltitude){
      presetReached = true;
    }  
  }
  Serial.println("Preset altitude reached!");
  
  // Turn off LED
  digitalWrite(eFuse, LOW);

  // END
  while(true);
  
}

float getBias(){
  float totalAltitudes = 0;
  int countvalue = 0;
  while (countvalue < 1000){
    totalAltitudes += bmp.readAltitude();
    countvalue++;
    delay(10);//wait for 10 miliseconds
  }
  return (totalAltitudes/1000);
}

float getAltitude(){
  return bmp.readAltitude() - bias;
}
