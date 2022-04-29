/*
 * https://maniacbug.github.io/RF24/classRF24.html
 * 
 * VCC - 3.3v
 * GND - GND
 * CSN - 7
 * CE - 8
 * SCK - 13
 * MOSI - 11
 * MISO - 12
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <SoftwareSerial.h>
#include "Model.h"

RF24 radio(9,10); // CE, CSN
const byte diachi[][6] = {"11110","11111","11112"};

SoftwareSerial Serial_Arduino(4,5);//RX - 4 - TX - 5

byte receivenrf[30];

String inputString;
bool stringComplete = false;

Garden garden1 = Garden(0,0,0,0,0,0,0);
Garden garden2 = Garden(0,0,0,0,0,0,0);
Gate gate = Gate(0,0,0);

void Read_UARTESP();
void XuLyChuoiESP(String chuoinhanESP);
void NRFSetup();
void ReadNRF();
void XulychuoiNRF(String chuoinhanNRF);

void setup() {
  Serial.begin(9600);
  Serial_Arduino.begin(9600);
  Serial.println("Start Module");
  NRFSetup();
}

void loop() {
 Read_UARTESP();
 sendESP();
 ReadNRF ();
 
}

void Read_UARTESP()
{
  while (Serial_Arduino.available()) //lấy dữ liệu
  {
    char incharr = (char)Serial_Arduino.read();//đọc từng byte
    inputString += incharr;

    if (incharr == '\n')
    {
      stringComplete = true;
    }
    if (stringComplete == true)
    {
      Serial.println(inputString);
      XuLyChuoiESP(inputString);
      inputString = "";
      stringComplete = false;
    }
  }
}

void NRFSetup(){
  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
  while (1) {}
  }
  radio.openWritingPipe(diachi[0]); 
  radio.openReadingPipe(1, diachi[1]);
  radio.openReadingPipe(2,diachi[2]);

  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  
  if (!radio.available())
  {
    Serial.println("CHỜ KẾT NỐI.......");
  } 
}

void ReadNRF(){
 radio.stopListening();
 Read_UARTESP();
 const char text[] = "Hello I am GateWay";
 radio.write(&text, sizeof(text));
 delay(10);

 radio.startListening();
 while (!radio.available());
 Read_UARTESP();
 radio.read(&receivenrf,sizeof(receivenrf));
 Serial.println((String)((char*)receivenrf));
 XulychuoiNRF((String)((char*)receivenrf));
 delay(10);
}

void XulychuoiNRF(String chuoinhanNRF){
  int findA = -1;
  int findB = -1;

  findA = chuoinhanNRF.indexOf("A");
  findB = chuoinhanNRF.indexOf("B");

  if(findA >= 0 && findB >= 0){
    String data = chuoinhanNRF.substring(findA + 1, findB);
    if(data == "1"){
      SetCambien(chuoinhanNRF,&garden1);
    }
    else if(data == "2"){
      SetCambien(chuoinhanNRF,&garden2);
    }
  }
}

void SetCambien(String chuoinhanNRF,Garden *garden){
  int findB = -1;
  int findC = -1;
  int findD = -1;
  int findE = -1;

  findB = chuoinhanNRF.indexOf("B");
  findC = chuoinhanNRF.indexOf("C");
  findD = chuoinhanNRF.indexOf("D");
  findE = chuoinhanNRF.indexOf("E");

  if(findB >= 0 && findC >= 0){
    String data = chuoinhanNRF.substring(findB + 1, findC);
    garden->setNhietDo(data.toFloat());
  }
  if(findC >= 0 && findD >= 0){
    String data = chuoinhanNRF.substring(findC + 1, findD);
    garden->setDoAm(data.toFloat());
  }
  if(findD >= 0 && findE >= 0){
    String data = chuoinhanNRF.substring(findD + 1, findE);
    garden->setDoAmDat(data.toFloat());
  }
}

void XuLyChuoiESP(String chuoinhanESP){
  int findA = -1;
  int findB = -1;
  int findC = -1;
  int findD = -1;
  int findE = -1;
  int findF = -1;
  int findG = -1;
  int findH = -1;
  int findI = -1;
  int findJ = -1;
  int findK = -1;

  findA = chuoinhanESP.indexOf("A");
  findB = chuoinhanESP.indexOf("B");
  findC = chuoinhanESP.indexOf("C");
  findD = chuoinhanESP.indexOf("D");
  findE = chuoinhanESP.indexOf("E");
  findF = chuoinhanESP.indexOf("F");
  findG = chuoinhanESP.indexOf("G");
  findH = chuoinhanESP.indexOf("H");
  findI = chuoinhanESP.indexOf("I");
  findJ = chuoinhanESP.indexOf("J");
  findK = chuoinhanESP.indexOf("K");

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

  if (findE >= 0 && findF >= 0){
    String data = chuoinhanESP.substring(findE + 1, findF);
    garden2.setPump(data);
  }

  if (findF >= 0 && findG >= 0){
    String data = chuoinhanESP.substring(findF + 1, findG);
    garden2.setFan(data);
  }

  if (findG >= 0 && findH >= 0){
    String data = chuoinhanESP.substring(findG + 1, findH);
    garden2.setLight(data);
  }

  if (findH >= 0 && findI >= 0){
    String data = chuoinhanESP.substring(findH + 1, findI);
    garden2.setMode(data);
  }

  if (findI >= 0 && findJ >= 0){
    String data = chuoinhanESP.substring(findI + 1, findJ);
    gate.setCheDo(data);
  }

  if (findJ >= 0 && findK >= 0){
    String data = chuoinhanESP.substring(findJ + 1, findK);
    gate.setMayBom(data);
  }

  Serial.println("Garden1");
  garden1.hienthi();
  Serial.println("Garden2");
  garden2.hienthi();
  gate.hienthi();
}

void sendESP(){
  static int last = millis();
  if((millis() - last >= 2000)){
    Serial_Arduino.println(chuoiguiESP());
    last = millis();
  }
}

String chuoiguiESP(){
  String chuoigui = 
  "A" + (String)garden1.getNhietDo() + 
  "B" + (String)garden1.getDoAm() + 
  "C" + (String)garden1.getDoAmDat() + 
  "D" + (String)garden1.getLight() + 
  "E" + (String)garden1.getFan() + 
  "F" + (String)garden1.getPump() +
  "G" + (String)garden1.getMode() +

  "H" + (String)garden2.getNhietDo() + 
  "I" + (String)garden2.getDoAm() + 
  "J" + (String)garden2.getDoAmDat() + 
  "K" + (String)garden2.getLight() + 
  "L" + (String)garden2.getFan() +
  "M" + (String)garden2.getPump() + 
  "N" + (String)garden2.getMode() +

  "O" + (String)gate.getCheDo() + 
  "P" + (String)gate.getMayBom() + 
  "Q" + (String)gate.getDoCao() + "R";
  return chuoigui;
}
