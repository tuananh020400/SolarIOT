#include <SoftwareSerial.h>
String inputString;
bool stringComplete = false;
SoftwareSerial Serial_ESP(13,5);// RX D1 noi voi chan D4 - TX D7 noi voi chan D5
.
void setup() {
  Serial.begin(115200);
  Serial_ESP.begin(9600);
  Serial.print("Alo");
}
void loop() {
  Read_UART_ESP();
  Serial_ESP.println("Hello i am ESP32");
  delay(500);
}
void Read_UART_ESP()
{
  while (Serial_ESP.available()) //lấy dữ liệu
  {
    char incharr = (char)Serial_ESP.read();//đọc từng byte
    inputString += incharr;

    if (incharr == '\n')
    {
      stringComplete = true;
    }
    if (stringComplete == true)
    {
      Serial.print("Data nhận được: ");
      Serial.println(inputString);
      inputString = "";
      stringComplete = false;
    }
  }
}
