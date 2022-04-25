
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte diachi[6] = "11111"; //Mảng kí tự dạng chuỗi có 6 kí tự

int nutnhan = 6; boolean gtnutnhan = 0;
int bientro = A0; int gtbientro; int gtbiendoi;

byte mang[7]; //Mảng có 2 phần tử

void setup() 
{
  Serial.begin(9600);

  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) {}
  }   
  radio.openWritingPipe(diachi);
  radio.setPALevel(RF24_PA_MIN); //Cài bộ khuyết địa công suất ở mức MIN
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);  
  radio.stopListening(); //Cài đặt module là TX
  if (!radio.available())
  {
    Serial.println("Chưa kết nối được với RX...!!");
    Serial.println("CHỜ KẾT NỐI.......");
  }

  pinMode(nutnhan, INPUT_PULLUP);
  pinMode(bientro, INPUT);
}

void loop() 
{
  String text = "A1B23.6C60.6D23E";
  char mang[30];
  text.toCharArray(mang,30);
  radio.write(&mang, sizeof(mang));
  Serial.println("Da gui");
  delay(1500);
  
}
