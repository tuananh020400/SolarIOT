#include <SoftwareSerial.h>
String inputString;
bool stringComplete = false;
int i = 0;
SoftwareSerial Serial_Arduino(4,5);//RX - 3 - TX - 4
void setup() {
  Serial.begin(9600);
  Serial_Arduino.begin(9600);
  Serial.print("Alo");
}
void loop() {
  Read_UART();
  String text = (String)i;
  Serial_Arduino.println(text);
  i++;
  delay(1000);
}
void Read_UART()
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
      Serial.print("Data nhận được: ");
      Serial.println(inputString);
      inputString = "";
      stringComplete = false;
    }
  }
}
