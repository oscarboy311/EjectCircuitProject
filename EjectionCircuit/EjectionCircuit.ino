#include <Adafruit_BMP085.h>
#include <Wire.h>
const int ledPin = 13; //assign an int value to the led pin number
const int eFuse = 12; //assign an int value to the electric fuse 
const float threshold = 100;
const float presetAltitude = 0;

float bias = 0;

Adafruit_BMP085 bmp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  while (1)  {
    if (!bmp.begin()) {
      Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    }
    else { 
      digitalWrite(ledPin, HIGH); // LED indicator
      break;
    }
  }
} 

void loop() {
  // Set and print Bias
  setBias(); // About 30 seconds
  Serial.print("Bias = ");
  Serial.print(bias);
  Serial.println(" meters");

  // Event Threshold (Apogee)
  float currReading = 0;
  do{
    // Read Sensor
    currReading = getAltitude();
    Serial.print("Current Altitude: "); 
    Serial.print(currReading);
    Serial.println(" meters");
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

void setBias(){
  float totalAltitudes = 0;
  for(int i = 0; i < 1000; i++){
    totalAltitudes += bmp.readAltitude();
    //delay(10); // wait for 10 ms
  }
  bias = totalAltitudes/1000;
  return;
}

float getAltitude(){
  return bmp.readAltitude() - bias;
}
