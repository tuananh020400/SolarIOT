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

byte receivenrf[4];
String text = "";
float mang[3];

Garden garden1 = Garden(0,0,0,0,0,0,0);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(FAN,OUTPUT);
  pinMode(PUMP,OUTPUT);
  pinMode(LIGHT,OUTPUT);
  pinMode(A0,INPUT);
  //Serial.println("Start Node Module");
  NRFSetup();
}

void loop() {
  if(garden1.getMode() == 1){
    while (1)
    {
      Read_DAD();
      ReadNRF(setThietBiManual);
      if (garden1.getMode() == 0)
      {
        break;
      }
      
    }
  }
  if (garden1.getMode() == 0)
  {
    while (1)
    {
      Read_DAD();
      ReadNRF(setThietBiAuto);
      if (garden1.getMode() == 1)
      {
        break;
      } 
    } 
  }
  
  
}

void NRFSetup(){
  if (!radio.begin()) 
  {
    //Serial.println("Module không khởi động được...!!");
  while (1) {}
  }
  radio.openWritingPipe(diachi[0]); 
  radio.openReadingPipe(1, diachi[1]);

  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  
  if (!radio.available())
  {
    //Serial.println("CHỜ KẾT NỐI.......");
  } 
}

void ReadNRF(void (*setThietBi)()){
  radio.stopListening();
  Read_DHT();
  mang[0] = garden1.getNhietDo();
  mang[1] = garden1.getDoAm();
  mang[2] = garden1.getDoAmDat();
  radio.write(&mang, sizeof(mang));
  delay(10);

  radio.startListening();
  while (!radio.available());
  radio.read(&receivenrf,sizeof(receivenrf));
  //Serial.println((String)((char*)receivenrf));
  XulychuoiNRF(receivenrf);
  setThietBi();
  delay(10);
}

void Read_DHT(){
  garden1.setDoAm(dht.readHumidity()); 
  garden1.setNhietDo(dht.readTemperature() - 3) ;
}

void Read_DAD(){
  float doAmDat = analogRead(A0);
  garden1.setDoAmDat(map(doAmDat,0,1023,100,0));
}

void XulychuoiNRF(byte* chuoinhanESP){
  garden1.setPump(chuoinhanESP[0]);
  garden1.setFan(chuoinhanESP[1]);
  garden1.setLight(chuoinhanESP[2]);
  garden1.setMode(chuoinhanESP[3]);
}

void setThietBiManual(){
    digitalWrite(PUMP,garden1.getPump() == 1?HIGH : LOW);
    digitalWrite(FAN,garden1.getFan()== 1?HIGH : LOW);
    digitalWrite(LIGHT,garden1.getLight()== 1?HIGH : LOW);
}

void setThietBiAuto(){
  if(garden1.getNhietDo() < 30){
    digitalWrite(LIGHT,HIGH);
    digitalWrite(FAN,LOW);
  }
  else if(garden1.getNhietDo() > 35){
    digitalWrite(LIGHT,LOW);
    digitalWrite(FAN,LOW);
  }
  else{
    digitalWrite(LIGHT,LOW);
    digitalWrite(FAN,LOW);
  }
  if(garden1.getDoAmDat() < 70){
    digitalWrite(PUMP,HIGH);
  }
  else{
    digitalWrite(PUMP,LOW);
  }
}
