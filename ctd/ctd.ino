#include <SPI.h>
#include <SD.h>
// Base name must be six characters or less for short file names.
#define FILE_BASE_NAME "Data"


const uint8_t CS_PIN = 10; // ChipSelect (MKRZero SD: SDCARD_SS_PIN)
const int gLed =  11;      //
const int rLed = 8;        //
const int LM35 = A0;       // Temperature sensor

File dataFile;
unsigned long int ttime;
float temp;

const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
char fileName[] = FILE_BASE_NAME "00.csv";

void setup(){
  pinMode(rLed, OUTPUT);
  pinMode(gLed, OUTPUT);
  Serial.begin(9600);

  if (!SD.begin(CS_PIN)) {
    Serial.println(F("begin failed"));
    digitalWrite(rLed, HIGH);
    return;
  }
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
  dataFfile = SD.open(fileName, FILE_WRITE);
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
  dataFile.print(temp);
  dataFile.close();

  Serial.print("The Temp is: ");
  Serial.println(temp);
  Serial.print("The time is: ");
  Serial.println(time);
  Serial.println("");
}
