#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <SoftwareSerial.h>
#include "Model.h"

RF24 radio(9,10); // CE, CSN
const byte diachi[][6] = {"11110","11111","11112"};

SoftwareSerial Serial_Arduino(4,5);//RX - 4 - TX - 5

String inputString;
bool stringComplete = false;

byte receivenrf[30];
char text[30];

Garden garden1 = Garden(0,0,0,0,0,0,0);
Garden garden2 = Garden(0,0,0,0,0,0,0);
Gate gate = Gate(0,0,0);

void setup() {
  Serial.begin(9600);
  Serial_Arduino.begin(9600);
  NRFSetup();
}

void loop() {
  Read_UARTESP();
  ReadNRF();

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
 String send = 
 "A" + (String)garden1.getPump() + 
 "B" + (String)garden1.getFan() + 
 "C" + (String)garden1.getLight() + 
 "D" + (String)garden1.getMode()+
 "E" + (String)garden2.getPump() + 
 "F" + (String)garden2.getFan() + 
 "G" + (String)garden2.getLight() + 
 "H" + (String)garden2.getMode() +
 "I" + (String)gate.getCheDo() + 
 "J" + (String)gate.getMayBom() + "K";
 static char text[30];
 send.toCharArray(text,30);
 radio.write(&text, sizeof(text));
 delay(10);

 radio.startListening();
 while (!radio.available());
 Read_UARTESP();
 radio.read(&receivenrf,sizeof(receivenrf));
 //Serial.println((String)((char*)receivenrf));
 //XulychuoiNRF((String)((char*)receivenrf));
 delay(10);
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

  gate.hienthi();
}
