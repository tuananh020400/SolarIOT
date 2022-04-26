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

byte receivenrf[30];
String text = "";
char mang[30];

Garden garden1 = Garden(0,0,0,0,0,0,0);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(FAN,OUTPUT);
  pinMode(PUMP,OUTPUT);
  pinMode(LIGHT,OUTPUT);
  pinMode(A0,INPUT);
  Serial.println("Start Node Module");
  NRFSetup();
}

void loop() {
  ReadNRF();
  Read_Sensor();
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

void Read_DHT(){
  garden1.setDoAm(dht.readHumidity()); 
  garden1.setNhietDo(dht.readTemperature()) ;
  Serial.print("Nhiet do: ");
  Serial.println(garden1.getNhietDo());               
  Serial.print("Do am: ");
  Serial.println(garden1.getDoAm());
}

void Read_DAT(){
  static float doAmDat = analogRead(A0);
  garden1.setDoAmDat(map(doAmDat,0,1023,0,100));
  Serial.print("Do am dat");
  Serial.println(garden1.getDoAmDat());
}

void Read_Sensor(){
  static int lastSensor = millis();
  if(millis() - lastSensor >= 500){
    Read_DAT();
    Read_DHT();
    text = "A1B" + (String)garden1.getNhietDo() + "C" + (String)garden1.getDoAm() + "D" + (String)garden1.getDoAmDat()+ "E";
    //sprintf(text,"A1B%fC%fD%fE",
    lastSensor = millis();
  }
}
