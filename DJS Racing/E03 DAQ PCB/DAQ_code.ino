#include <TinyGPS++.h> 
#include <SD.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#define TFT_CS         PB5 //PB13                  
#define TFT_DC         PB4 //PB15                 
#define TFT_RST        PB3//PB14  
#define OA1             PB1//PB4
#define OA2             PB0//PB1
#define OB1            PA1//PB0
#define OB2            PA0//PA3
#define D0             PA15//PB3
#define D1             PA8//PB4
#define D2             PB12//PB5
File dataFile;
TinyGPSPlus gps;    
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST); // Use hardware SPI
double analogValue = 0.0;
int previousvalue=0;
int previousvalue1 = 0;
float previousvalue2 = 0.0;
int previousvalue3 = 0;
int analogValue1 = 0;
double analogValue2 = 0.0;
int analogValue3 = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(PB6,OUTPUT); //PA1
  digitalWrite(PB6,LOW); // enter AT command mode
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.print(F("AT+C001")); // set to channel 1 
  delay(100);
  digitalWrite(PB6,HIGH);// enter transparent mode
  //Serial2.begin(9600);
  Serial.print("Initializing SD card...");

  if (!SD.begin(PB7)) {   //PB9
    Serial.println("initialization failed!");
    while(1);
  }
  Serial.println("initialization done.");
 

  delay(1000);
  SD.mkdir("djs");
  dataFile = SD.open("data.txt", FILE_WRITE);
  Serial.print("OA1    ||    OA2    ||    OB1   ||    OB2");
  dataFile.print("OA1    ||    OA2    ||    OB1   ||    OB2");
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  tft.begin();
  tft.setRotation(2);
  tft.setCursor(20, 60);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(5);
  tft.print("D.J.S.R");
  tft.setCursor(60, 150);
  tft.setTextSize(10);
  tft.print("E02");
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(70, 260);
  tft.setTextSize(2);
  tft.print("Loading...");
  tft.drawRect(0, 280, 240, 20, ILI9341_GREEN);
  for (int x = 0; x <= 240; x++)
  {
    tft.fillRect(0, 280, x, 20, ILI9341_GREEN);
    delay(5);

  }
  tft.setTextColor(ILI9341_RED);
  tft.fillScreen(ILI9341_BLUE);
  tft.setCursor(0,0);
  tft.setTextSize(2);
  tft.print("OA1");
  tft.drawRect(0,25, 240, 30, ILI9341_WHITE);
  tft.setCursor(110,100);
  tft.print("OA2");
  tft.setCursor(0,220);
  tft.print("OB1");
  tft.setCursor(150,220);
  tft.print("OB2");
  dataFile.println();
  dataFile.close();
}

void loop() {
  // put your main code here, to run repeatedly:
  dataFile = SD.open("data.txt", FILE_WRITE);
    double lat_val, lng_val;
    int i;
    for(i=0;i<=7;i++)
    {
      convertADC(i);
      if(i==0) //For sensors whose reading we want on LCD
      {
        tft.setRotation(2);
        testText();
        printValues();
      }
      else
      {
       
        do_ADC();
        Serial.print(analogValue1);
        Serial.print(" ");
        Serial.print(analogValue3);
        Serial.print(" ");
        Serial.print(analogValue);
        Serial.print(" ");
        Serial.print(analogValue2);
        Serial.println();
        printValues();
      }
      
}
        dataFile.println();
        GPSDelay(500);
        lat_val = gps.location.lat();        //Gets the latitude
        lng_val = gps.location.lng(); 
        Serial.println();
        Serial.print("Latitude : ");
        Serial.println(lat_val, 6); 
        Serial1.write((byte)lat_val);
        Serial.println();
        dataFile.print(lat_val, 6);
        Serial.print("Longitude : ");
        Serial.println(lng_val, 6); 
        Serial1.write((byte)lng_val);
        dataFile.print(lng_val, 6); 
        dataFile.println();
        dataFile.close();
}
void printValues()
{
        Serial1.write((byte)analogValue);
        Serial1.write((byte)analogValue1);
        Serial1.write((byte)analogValue2);
        Serial1.write((byte)analogValue3);
        dataFile.print(analogValue1);
        dataFile.print(" ");
        dataFile.print(analogValue3);
        dataFile.print(" ");
        dataFile.print(analogValue);
        dataFile.print(" ");
        dataFile.print(analogValue2);
        dataFile.println();
}
void convertADC(int n)
{
  int rem;
  rem = n%2;
  n = n/2;
  if(rem==1)
  digitalWrite(D0, HIGH);
  else if(rem == 0)
  digitalWrite(D0, LOW);
  rem = n%2;
  n = n/2;
  if(rem==1)
  digitalWrite(D1, HIGH);
  else if(rem == 0)
  digitalWrite(D1, LOW);
  rem = n%2;
  if(rem==1)
  digitalWrite(D2, HIGH);
  else if(rem == 0)
  digitalWrite(D2, LOW);
}
void do_ADC()
{
  analogValue = analogRead(OA1);    
  analogValue1 = analogRead(OA2);
  analogValue2 = analogRead(OB1);
  analogValue3 = analogRead(OB2);
}
unsigned long testText() {
  unsigned long start  = micros();
  do_ADC();
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(3);
  if(previousvalue1<=analogValue1)
  { 
    tft.fillRect(0, 26, analogValue1/17.0667, 29, ILI9341_GREEN);
    previousvalue1 = analogValue1;
    Serial.print(previousvalue1);
    Serial.print("    ");
  }
  
  else
  {
     
    tft.fillRect(0, 25, analogValue1/17.0667, 30, ILI9341_GREEN);
    tft.fillRect(analogValue1/17.0667,26,240,29,ILI9341_BLUE);
    previousvalue1 = analogValue1;
    Serial.print(previousvalue1);
    Serial.print("    ");
  }
  tft.setCursor(150, 250);
  if (analogValue != previousvalue)
  {
    previousvalue = analogValue;
    tft.fillRect(150, 250, 140, 40, ILI9341_BLUE);
    tft.setCursor(150, 250);
    tft.print(previousvalue);
    Serial.print(previousvalue);
    Serial.print("    ");
}else{
    tft.setCursor(150, 250);
    tft.print(previousvalue);
              
    Serial.print(previousvalue);
    Serial.print("    ");
}

   tft.setCursor(0, 250);
   tft.setTextColor(ILI9341_WHITE);  
   tft.setTextSize(3);
   if (analogValue2 != previousvalue2)
  {
    previousvalue2 = analogValue2;
    tft.fillRect(0, 250, 140, 80, ILI9341_BLUE);
    

              tft.setCursor(0, 250);
              tft.print((previousvalue2/4095)*100);
              tft.print("%");
              Serial.print((previousvalue2/4095)*100);
              Serial.print("    ");
              
              //delay(1000);


      }
              else
            { 
              tft.setCursor(0, 250);
              tft.print((previousvalue2/4095)*100);
              Serial.println((previousvalue2/4095)*100);
              tft.print("%");
              Serial.print((previousvalue2/4095)*100);
              Serial.print("    ");

            }
   tft.setCursor(85, 130);
   tft.setTextColor(ILI9341_WHITE);  
   tft.setTextSize(3);
    
  if (analogValue3 != previousvalue3)
  {
    previousvalue3 = analogValue3;
    tft.fillRect(110, 130, 90, 30, ILI9341_BLUE);
    

              tft.setCursor(110, 130);
              tft.print(previousvalue3);
              Serial.println(previousvalue3);
             
              //delay(1000);


      }
              else
            { 
              tft.setCursor(110, 130);
              tft.print(previousvalue3);
              
              Serial.println(previousvalue3);
              

            }
  return micros() - start;
  Serial.println();
}
static void GPSDelay(unsigned long ms)          //Delay for receiving data from GPS
{
  unsigned long start = millis();
  do
  {
    while (Serial.available()) 
    gps.encode(Serial.read());
  } while (millis() - start < ms);
}
 
