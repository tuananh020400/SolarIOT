#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Model.h"

RF24 radio(9,10); // CE, CSN
const byte diachi[][6] = {"11111","11110"};

byte receivenrf[30];

Garden garden1 = Garden(0,0,0,0,0,0,0);

void setup() {
  Serial.begin(9600);
  Serial.println("Start Node Module");
  NRFSetup();
}

void loop() {
  ReadNRF ();

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

void ReadNRF(){
 radio.stopListening();
 String text = "A1B23.6C60.6D23E";
 char mang[30];
 text.toCharArray(mang,30);
 radio.write(&mang, sizeof(mang));
 delay(10);

 radio.startListening();
 while (!radio.available());
 radio.read(&receivenrf,sizeof(receivenrf));
 Serial.println((String)((char*)receivenrf));
 //XulychuoiNRF((String)((char*)receivenrf));
 delay(10);
}
