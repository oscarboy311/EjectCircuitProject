#include <Adafruit_BMP085.h>
const int ledPin = 13; //assign an int value to the led pin number
const int eFuse = 12; //assign an int value to the electric fuse 

float bias = 0;
float threshold = 0;
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

  // Event Threshold
  float currReading = 0;
  do{
    // Read Sensor
    currReading = bmp.readAltitude();
  }
  while(currReading < threshold);

  // ...
  float prevReading = bmp.readAltitude();
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
