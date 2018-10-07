#include <Wire.h>
#include "DHT.h"
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX

byte sw1 = 2;
byte sw2 = 3;
byte sw3 = 4;
byte sw4 = 5;
String Str;

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 7
#define S1 8
#define S2 9
#define S3 10
#define sensorOut 11

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

unsigned long showTime;

unsigned long previousTime = 0;
int setTime = 45;
int xy = 0;
int start = 0;
void setup()
{
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);


  // initialize the LCD
  Serial.begin(9600);
  mySerial.begin(115200);

  lcd.begin();
  dht.begin();
  lcd.setCursor(0, 0);
  lcd.print("Hello (^-^)");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Oil clean :)");
  delay(2000);

  menu();
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(sw4, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

}

void loop()
{



  if (digitalRead(sw1) == 0) {
    while (1) {
      readcolor();
      if (digitalRead(sw4) == 0) {
        Send(Str);
        Cclear();
        back();
        break;
      }
    }


  }
  if (digitalRead(sw2) == 0) {
    showTime = millis();

    while (1) {
      showTime = millis();
      readhumdi();
      if (digitalRead(sw4) == 0) {
        Send(Str);
        Cclear();
        back();
        break;
      }
    }


  }
  if (digitalRead(sw3) == 0) {
    while (1) {
      readethanol();
      if (digitalRead(sw4) == 0) {
        Send(Str);
        Cclear();
        back();
        break;
      }
    }


  }




}

void readcolor() {

  lcd.setCursor(0, 0);
  lcd.print("redcolor.......");
  delay("1000");

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  // You must replace with your own values. Here's an example:
  // redColor = map(redFrequency, 70, 120, 255,0);
  redColor = map(redFrequency, 352, 412, 255, 0);

  // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  // You must replace with your own values. Here's an example:
  // greenColor = map(greenFrequency, 100, 199, 255, 0);
  greenColor = map(greenFrequency, 462, 470, 255, 0);

  // Printing the GREEN (G) value
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  // You must replace with your own values. Here's an example:
  // blueColor = map(blueFrequency, 38, 84, 255, 0);
  blueColor = map(blueFrequency, 288 , 329, 255, 0);

  // Printing the BLUE (B) value
  Serial.print(" B = ");
  Serial.print(blueFrequency);
  delay(100);

  // Checks the current detected color and prints
  // a message in the serial monitor
  if (redColor > greenColor && redColor > blueColor) {
    Serial.println(" - RED detected!");
    Str = "RED detected";
  }
  if (greenColor > redColor && greenColor > blueColor) {
    Serial.println(" - GREEN detected!");
    Str = "GREEN detected";
  }
  if (blueColor > redColor && blueColor > greenColor) {
    Serial.println(" - BLUE detected!");
    Str = "BLUE detected";

  }

  lcd.setCursor(0, 1);
  lcd.print("color:");
  lcd.setCursor(6, 1);
  lcd.print(Str);

}

void Cclear() {
  Serial.print("cclear");
  Str = "";
  lcd.clear();
}

void menu() {
  lcd.setCursor(0, 0);
  lcd.print("1:color2:mixoil");
  lcd.setCursor(0, 1);
  lcd.print("3:ethanol....");
}

void back() {
  Serial.print("back");
  lcd.setCursor(0, 0);
  lcd.print("1:color2:mixoil");
  lcd.setCursor(0, 1);
  lcd.print("3:ethanol");
}

void readhumdi() {

  float first = 0;
  float last = 0;
  start = 1;
  Serial.print("Start...");
  lcd.setCursor(0, 0);
  lcd.print("readHumdi.......");
  delay("1000");
  lcd.setCursor(0, 1);
  lcd.print("Humdi:");

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println("Failed to read from DHT sensor!");
    return;
  }
  lcd.setCursor(6, 1);
  lcd.print(h);

  lcd.setCursor(11, 1);
  lcd.print("s:");
  lcd.setCursor(13, 1);
  lcd.print(xy);

  first = h;

  if (start == 1) //เมื่อ start = 1 จะเป็นการวน loop จับเวลาไปเรื่อยๆ จนกว่าจะครบกำหนด
  {
    Serial.print("setTime: ");
    Serial.print(setTime); //แสดงค่าเวลาที่ตั้งไว้
    Serial.print("   showTime: ");
    Serial.print(showTime); //แสดงเวลา millis() ของ Arduino ซึ่งจะเป็นค่าที่วิ่งไปเรื่อยๆ
    Serial.print("   previousTime: ");
    Serial.print(previousTime); //แสดงค่าสุดท้ายของ previousTime หลังจากที่กดปุ่ม A ซึ่งจะเป็นค่าคงที่ ไม่วิ่งตาม millis()
    Serial.print("   Time: ");
    Serial.println((showTime - previousTime) / 1000); //แสดงผลการคำนวณว่าจับเวลาไปกี่วิแล้ว
    xy = (showTime - previousTime) / 1000;
    if (xy > 45) {
      previousTime = showTime;
      xy = 0;
    }


    if (xy >= setTime) //ถ้าเวลาที่จับได้ มากกว่าหรือเท่ากับเวลาที่ตั้งไว้...
    {
      h = dht.readHumidity();
      t = dht.readTemperature();
      f = dht.readTemperature(true);


      if (isnan(h) || isnan(t) || isnan(f)) {
        //Serial.println("Failed to read from DHT sensor!");
        return;
      }
      xy = 0;
      start = 0;
      previousTime = 0;
      last = h;
      Serial.print("Finish!");
      lcd.setCursor(8, 1);
      lcd.print(h);
      Cclear
      ();
      if (last >= first) {
        Str =  "No Mix oil";
        lcd.setCursor(0, 1);
        lcd.print(" No MIX");
        delay(1000);

      } else {
        Str = "Have mix Oil";
        lcd.setCursor(0, 1);
        lcd.print("MIX");
        delay(1000);
      }
    }
  }











}
void Send(String str) {
  int i = 0;
  while (str[i] != '\0') {
    mySerial.write(str[i]);
    i++;
  }
}

void readethanol() {
  lcd.setCursor(0, 0);
  lcd.print("readethanol...");
  delay("1000");
  lcd.setCursor(0, 1);
  lcd.print("ethanol : ");


  float warter; // 10 ml
  float ethanol;

  float s1  = analogRead(A0);  // s4 11.65   //  20.58
  float s2  = analogRead(A1)+30;  // 9.65 // 17.05 ml
  float s3  = analogRead(A2)+25;  // 7.65 // 13.52 ml
  float s4  = analogRead(A3)+82  ; // 5.65  10 ml

  Serial.print("s1: ");
  Serial.print(s1);

  Serial.print("\ts2 : ");
  Serial.print(s2);


  Serial.print("\ts3: ");
  Serial.print(s3);


  Serial.print("\ts4: ");
  Serial.println(s4);

  if (s2 > s1) {
    warter = 10.1;
  }
  else  if (s2 > s3) {
    warter = 13.52;
  }
  else if (s3 > s4) {
    warter = 17.05;
  }

  ethanol = (warter - 10) * 2;
  lcd.setCursor(0, 1);
  lcd.print("ethanol : ");
  lcd.setCursor(10, 1);
  lcd.print(ethanol);

  Str = "Ethanol = :  " + String(ethanol) + "% .";


}

