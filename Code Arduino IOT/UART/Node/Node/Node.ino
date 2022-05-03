#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Model.h"
#include "DHT.h"

#define DHTPIN 3
#define FAN 2
#define PUMP 4 
#define LIGHT 8

const int DHTTYPE = DHT11;  
DHT dht(DHTPIN, DHTTYPE);


RF24 radio(9,10); // CE, CSN
const byte diachi[][6] = {"11111","11110"};

byte receivenrf[30];
String text = "";
char mang[30];

Garden garden1 = Garden(0,0,0,0,0,0,0);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(FAN,OUTPUT);
  pinMode(PUMP,OUTPUT);
  pinMode(LIGHT,OUTPUT);
  pinMode(A0,INPUT);
  Serial.println("Start Node Module");
  NRFSetup();
}

void loop() {
  ReadNRF(setThietBiManual);
  //Read_Sensor();
}

void NRFSetup(){
  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
  while (1) {}
  }
  radio.openWritingPipe(diachi[0]); 
  radio.openReadingPipe(1, diachi[1]);

  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  
  if (!radio.available())
  {
    Serial.println("CHỜ KẾT NỐI.......");
  } 
}

void ReadNRF(void (*setThietBi)()){
 radio.stopListening();
 Read_DAT();
 Read_DHT();
 text = "A1B" + (String)garden1.getNhietDo() + "C" + (String)garden1.getDoAm() + "D" + (String)garden1.getDoAmDat()+ "E";
 text.toCharArray(mang,30);
 radio.write(&mang, sizeof(mang));
 delay(10);

 radio.startListening();
 while (!radio.available());
 radio.read(&receivenrf,sizeof(receivenrf));
 Serial.println((String)((char*)receivenrf));
 XulychuoiNRF((String)((char*)receivenrf));
 setThietBi();
 delay(10);
}

void Read_DHT(){
  garden1.setDoAm(dht.readHumidity()); 
  garden1.setNhietDo(dht.readTemperature()) ;
}

void Read_DAT(){
  static float doAmDat = analogRead(A0);
  garden1.setDoAmDat(map(doAmDat,0,1023,0,100));
}

void Read_Sensor(){
  static int lastSensor = millis();
  if(millis() - lastSensor >= 500){
    Read_DAT();
    Read_DHT();
    text = "A1B" + (String)garden1.getNhietDo() + "C" + (String)garden1.getDoAm() + "D" + (String)garden1.getDoAmDat()+ "E";
    lastSensor = millis();
  }
}
void XulychuoiNRF(String chuoinhanESP){
  int findA = -1;
  int findB = -1;
  int findC = -1;
  int findD = -1;
  int findE = -1;

  findA = chuoinhanESP.indexOf("A");
  findB = chuoinhanESP.indexOf("B");
  findC = chuoinhanESP.indexOf("C");
  findD = chuoinhanESP.indexOf("D");
  findE = chuoinhanESP.indexOf("E");

   if (findA >= 0 && findB >= 0){
    String data = chuoinhanESP.substring(findA + 1, findB);
    garden1.setPump(data);
  }

  if (findB >= 0 && findC >= 0){
    String data = chuoinhanESP.substring(findB + 1, findC);
    garden1.setFan(data);
  }

  if (findC >= 0 && findD >= 0){
    String data = chuoinhanESP.substring(findC + 1, findD);
    garden1.setLight(data);
  }

  if (findD >= 0 && findE >= 0){
    String data = chuoinhanESP.substring(findD + 1, findE);
    garden1.setMode(data);
  }
}

void setThietBiManual(){
    digitalWrite(PUMP,garden1.getPump() == 1?HIGH : LOW);
    digitalWrite(FAN,garden1.getFan()== 1?HIGH : LOW);
    digitalWrite(LIGHT,garden1.getLight()== 1?HIGH : LOW);
}
