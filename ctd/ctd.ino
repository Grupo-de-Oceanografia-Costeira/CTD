#include <SPI.h>
#include <SD.h>
#include <Wire.h>                  // BMP085
#include <Adafruit_BMP085.h>       // BMP085
// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// Note that sensors may change the name, so check the sensor's scheme.

// Base name must be six characters or less for short file names.
#define FILE_BASE_NAME "Data"


const uint8_t CS_PIN = 10;        // ChipSelect (MKRZero SD: SDCARD_SS_PIN)
const int gLed =  11;            //
const int rLed = 8;              //
const int LM35 = A0;             // Temperature sensor
Adafruit_BMP085 bmp;
const int sal = A5;
//SoftwareSerial gpsSerial(2, 3); // TX, RX - GPS

File dataFile;
unsigned long int ttime;
float temp;
long lat,lon;

const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
char fileName[] = FILE_BASE_NAME "00.csv";

void setup(){
  pinMode(rLed, OUTPUT);
  pinMode(gLed, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  Serial.begin(9600);

  if (!SD.begin(CS_PIN) and !bmp.begin()) {
    Serial.println("begin failed");
    digitalWrite(rLed, HIGH);
    delay(4000);
    digitalWrite(rLed, LOW);
    return;
  }

  // Insert here condition to GPS data
  // if(!GPS.begin(GPS_PIN){
  //   delay(1000);
  //   digitalWrite(rLed, HIGH);
  //   digitalWrite(gLed, HIGH);
  //   delay(2000);
  //   return;
  // }

  while (SD.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      // Blink 3 times
      Serial.println("Can't create file name");
      digitalWrite(rLed, LOW);
      delay(1000);
      digitalWrite(rLed, HIGH);
      delay(1000);
      digitalWrite(rLed, LOW);
      delay(1000);
      digitalWrite(rLed, HIGH);
      delay(1000);
      digitalWrite(rLed, LOW);
      delay(1000);
      digitalWrite(rLed, HIGH);
      delay(1000);
      digitalWrite(rLed, LOW);
      return;
    }
  }
  dataFile = SD.open(fileName, FILE_WRITE);
  if (!dataFile) {
    // Blink 3 times faster
    Serial.println("open failed");
    delay(200);
    digitalWrite(rLed, HIGH);
    delay(200);
    digitalWrite(rLed, LOW);
    delay(200);
    digitalWrite(rLed, HIGH);
    delay(200);
    digitalWrite(rLed, LOW);
    delay(200);
    digitalWrite(rLed, HIGH);
    delay(200);
    digitalWrite(rLed, LOW);
    return;
  }
  Serial.print("opened: ");
  Serial.println(fileName);
  digitalWrite(gLed,HIGH);
  delay(2000);
  digitalWrite(gLed, LOW);
  delay(500);
  digitalWrite(gLed,HIGH);
  delay(2000);
  digitalWrite(gLed, LOW);
  dataFile.close();
}

void loop(){
  ttime = millis();
  temp = (float(analogRead(LM35))*5/(1023))/0.01;
  if (temp > 28) {
    digitalWrite(rLed, HIGH);
    delay(250);
    digitalWrite(rLed, LOW);
    delay(250);
  }
  else {
    digitalWrite(gLed,HIGH);
    delay(250);
    digitalWrite(gLed, LOW);
    delay(250);
  }
  File dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.print(ttime);
  dataFile.print(",");
  // Record GPS data
  // dataFile.print(",");
  dataFile.print(temp);
  dataFile.print(",");
  dataFile.print(bmp.readTemperature());
  dataFile.print(",");
  dataFile.print(bmp.readPressure());
  dataFile.print(",");
  dataFile.print(bmp.readSealevelPressure());
  dataFile.print(",");
  dataFile.print(bmp.readAltitude(101500));
  dataFile.print(",");
  dataFile.print(analogRead(sal));
  // if the current sea level pressure is known, it will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  // Altitude is in meters
  dataFile.print(",");

  dataFile.close();

  Serial.print("The Temp is: ");
  Serial.println(temp);
  Serial.print("The time is: ");
  Serial.println(ttime);
  Serial.println("");
  Serial.println(analogRead(sal));
}
