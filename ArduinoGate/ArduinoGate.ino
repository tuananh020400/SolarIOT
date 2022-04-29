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

void setup() {
  Serial.begin(9600);
  Serial_Arduino.begin(9600);
}

void loop() {
  Read_UARTESP();

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
