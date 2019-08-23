//대림대학교 컴퓨터 정보학부
//Daelim University College dept.Computer Information


#include <Wire.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>  // Library for LCD
#include "pitches.h"
#include <Servo.h>
LiquidCrystal_I2C TV(0x27, 16, 2);  // I2C LCD -> TV
#define DHTPIN 2 //DHT seonsor pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Servo servo;

int piezoPin = 8; // Piezo buzzer
int irPin = A3;  // IR sensor
int cdsPin = A2; //cds sensor
int swsv = 5; //Switch for Servomoter
int angle = 0; //servo moter angle

int C = 1047; // 도
int D = 1175; // 레 
int E = 1319; // 미 
int F = 1397; // 파 
int G = 1568; // 솔 
int A = 1760; // 라 
int B = 1976; // 시

int notes[25] = { G, G, A, A, G, G, E, G, G, E, E, D, G, G, A, A, G, G, E, G, E, D, E, C };
//학교종이 땡땡땡 멜로디
int tempo = 200;

boolean flag = 1;
boolean flag2 = 1;
int numTones = 5;
//int notes[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, 0, NOTE_B3, NOTE_C4};
//int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

// 가까울 시 울릴 경보의 멜로디 작성

void setup()
{
  // 시리얼 통신을 위해 통신속도(Baudrate)를 9600으로 설정
  Serial.begin(9600);
  pinMode(piezoPin, OUTPUT); // 피에조 핀을 출력핀으로 설정
  dht.begin();
  TV.begin(); // I2C LCD의 기본 설정
  TV.backlight();  // I2C LCD의 백라이트를 켜줌
  TV.setCursor(0, 0); // I2C LCD의 커서위치를 0, 0으로 설정(첫번째 줄, 첫번째 칸)
  TV.print("Looking TV...."); // I2C LCD에 "Looking TV...." 메세지 출력
  servo.attach(7);
  pinMode(swsv, INPUT_PULLUP);
}

void loop()
{
  //TV.clear();
  int cdsValue = analogRead(cdsPin);
  Serial.print("조도 값 : "); //cds Value index
  Serial.println(cdsValue);
  int IRval = analogRead(irPin);

  float humidity = dht.readHumidity(); //humidity - 습도 측정 val
  float temperature = dht.readTemperature(); //temperature - 온도 측정 val

  if(isnan(humidity) || isnan(temperature) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print((int)temperature);
  Serial.print("*C, ");
  Serial.print((int)humidity);
  Serial.println(" %");

  if(digitalRead(swsv) == LOW && flag2 == 1)
  {
    flag2 = 0;
    servo.write(180);
    delay(1000);
  }

  else if(digitalRead(swsv) == LOW && flag2 == 0)
  {
    flag2 = 1;
    servo.write(0);
    delay(1000);
  }
  
  if(cdsValue > 800) {
    bedTone();
    delay(300);
  }


  if (IRval < 100 && flag == 1) {          //TV가 켜져있고, 거리가 가깝다면 TV OFF
    flag = 0;
    TV.clear();
    TV.noBacklight();
    tone(piezoPin, 600, 1000);
    delay(100);
  }

  else if (IRval > 900 && flag == 0) {    //TV가 꺼져있고, 거리가 멀다면 TV ON
    noTone(piezoPin);
    flag = 1;
    TV.backlight();
    TV.print("Looking TV....");
  }
}

void bedTone() {
  for(int i = 0; i < 12; i++) {
    tone(piezoPin, notes[i], tempo);
    delay(300);
  }
}
