#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <SoftwareSerial.h>
#include "Model.h"

#define PUMP 3
#define TRIG_PIN 8
#define ECHO_PIN 7
#define TIME_OUT 5000

RF24 radio(9,10); // CE, CSN
const byte diachi[][6] = {"11110","11111","11112"};

SoftwareSerial Serial_Arduino(4,5);//RX - 4 - TX - 5

String inputString = "";
bool stringComplete = false;

float receivenrf[3];
String chuoiguiESP = "";

Garden garden1 = Garden(0,0,0,0,0,0,0);
Garden garden2 = Garden(0,0,0,0,0,0,0);
Gate gate = Gate(0,0,0);

void setup() {
  Serial.begin(9600);
  Serial_Arduino.begin(9600);
  NRFSetup();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PUMP, OUTPUT);
}

void loop() {
if (gate.getCheDo() == 1)
{
  while (1)
  {
    Read_UARTESP(setPumpManual);
    sendData();
    ReadNRF();
    if (gate.getCheDo() == 0)
    {
      break;
    }
  }
}

if (gate.getCheDo() == 0)
{
  while (1)
  {
    Read_UARTESP(setPumpAuto);
    sendData();
    ReadNRF();
    if (gate.getCheDo() == 1)
    {
      break;
    }
  }
}
}

void Read_UARTESP(void (*setPump)())
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
      setPump();
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
  static byte text[4];
  text[0] = garden1.getPump();
  text[1] = garden1.getFan();
  text[2] = garden1.getLight();
  text[3] = garden1.getMode();
  radio.write(&text, sizeof(text));
  delay(10);

  radio.startListening();
  while (!radio.available());
  radio.read(&receivenrf,sizeof(receivenrf));
  chuoiguiESP = "A1B" + (String)receivenrf[0] + "C" + (String)receivenrf[1] + "D" + (String)receivenrf[2] + "E";
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

  findA = chuoinhanESP.indexOf("A");
  findB = chuoinhanESP.indexOf("B");
  findC = chuoinhanESP.indexOf("C");
  findD = chuoinhanESP.indexOf("D");
  findE = chuoinhanESP.indexOf("E");
  findF = chuoinhanESP.indexOf("F");
  findG = chuoinhanESP.indexOf("G");

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
    gate.setCheDo(data);
  }

  if (findF >= 0 && findG >= 0){
    String data = chuoinhanESP.substring(findF + 1, findG);
    gate.setMayBom(data);
  }

  gate.hienthi();
}

void sendData(){
  static unsigned long last = millis();
  if( millis() - last > 1000){
    DocKhoangCach();
    Serial_Arduino.println(chuoiguiESP);
    Serial_Arduino.print("A3B");
    Serial_Arduino.print(gate.getDoCao());
    Serial_Arduino.println("C");
    last = millis();
  }
}
float getDistance()
{
  long duration, distanceCm;
   
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH, TIME_OUT);
 
  // convert to distance
  distanceCm = duration / 29.1 / 2;
  
  return distanceCm;
}
void DocKhoangCach(){
  long distance = getDistance();

  if (distance <= 0)
  {
    Serial.println("Echo time out !!");
  }
  else
  {   
    Serial.print("Distance to nearest obstacle (cm): ");
    Serial.println(distance);
    gate.setDoCao(33 - distance);
  }
}

void setPumpAuto(){
  if(gate.getDoCao() >= 28){
     digitalWrite(PUMP,LOW);
  }
  else if(gate.getDoCao() <= 5){
    digitalWrite(PUMP,HIGH);
  }

}
void setPumpManual(){
  if(gate.getDoCao() >= 28){
     digitalWrite(PUMP,LOW);
  }
  else{
    gate.getMayBom() == 1?
    digitalWrite(PUMP,HIGH):
    digitalWrite(PUMP,LOW);
  }
}
