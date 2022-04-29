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
 //const char text[] = "Hello I am GateWay";
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
