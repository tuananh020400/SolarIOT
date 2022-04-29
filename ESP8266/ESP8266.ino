
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include "Model.h"

#define LED 16
#define PIN_AP 4

const char *ID = "GateWay";
const char *PUB_TOPIC = "subscribe";
const char *SUB_TOPIC = "publish";
const char *BROKER = "broker.emqx.io";

String inputString;
bool stringComplete = false;

Garden garden1 = Garden(0,0,0,0,0,0,0);
Garden garden2 = Garden(0,0,0,0,0,0,0);
Gate gate = Gate(0,0,0);

WiFiClient espClient;
PubSubClient client(espClient);
SoftwareSerial Serial_ESP(13,5);// RX D1 noi voi chan D4 - TX D6 noi voi chan 3

void wifiSetup();
void callback(char *topic, byte *payload, unsigned int length);
void connectMQTT();
void reconnect();
void resetWifi();

void setup() {
  Serial.begin(115200);
  Serial_ESP.begin(9600);
  pinMode(PIN_AP, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  wifiSetup();
}

void loop() {
  connectMQTT();
  Read_UART_ESP();
  sendData();
}

void wifiSetup(){
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP32configue");
  Serial.println("WiFi Connected");
  Serial.println(wifiManager.getWiFiSSID(true));
  Serial.println(wifiManager.getWiFiPass(true));
  client.setServer(BROKER,1883);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  while (1)
  {
      String response;
    for(int i = 0; i < length ; i++)
    {
        response += (char) payload[i];
    }
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(response);
      XuLyChuoiMQTT(response);
      sendMQTT();
      sendArduino();
      break;
  }
}

void connectMQTT(){
    WiFiManager wifiManager;
  if ( digitalRead(PIN_AP) == LOW) {
      Serial.println("resetar"); //tenta abrir o portal
      if(!wifiManager.startConfigPortal("ESP32configue") ){
        Serial.println("Falha ao conectar");
        wifiManager.resetSettings();
        delay(2000);
        ESP.restart();
        delay(1000);
      }
  }
      if ((WiFi.status() == WL_CONNECTED))
  {
    static int lastLed = millis();
    if(millis() - lastLed > 500){
       digitalWrite(LED, HIGH);}
    if(millis() - lastLed > 1000){
       digitalWrite(LED, LOW);
       lastLed = millis();
       }
  }
  else{}
  if(!client.connected())
    reconnect();
  client.loop();
  client.setCallback(callback);
}
void reconnect()
{
  while(!client.connected())
  {
    Serial.print("Connect to MQTT Server...");
    if(client.connect(ID))
    {
      client.subscribe(SUB_TOPIC);
      Serial.println("Connected");
      Serial.print("Subscibe to ");
      Serial.println(SUB_TOPIC);
      Serial.println("\n");
    }
    else
    {
      Serial.println("Try again in 2 seconds");
      delay(2000);
    }
  }
}

void resetWifi()
{
  WiFiManager wifiManager;
  if ( digitalRead(PIN_AP) == LOW) {
      Serial.println("RESET ESP"); //tenta abrir o portal
      if(!wifiManager.startConfigPortal("ESP32configue") ){
        Serial.println("STA MODE");
        wifiManager.resetSettings();
        delay(2000);
        ESP.restart();
        delay(1000);
      }
  }
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
      Serial.print(inputString);
      XulychuoiUART(inputString);
      inputString = "";
      stringComplete = false;
    }
  }
}

void XulychuoiUART(String chuoinhanUART){
  int findA = -1;
  int findB = -1;

  findA = chuoinhanUART.indexOf("A");
  findB = chuoinhanUART.indexOf("B");

  if(findA >= 0 && findB >= 0){
    String data = chuoinhanUART.substring(findA + 1, findB);
    if(data == "1"){
      SetCambien(chuoinhanUART,&garden1);
    }
    else if(data == "2"){
      SetCambien(chuoinhanUART,&garden2);
    }
  }
}

void SetCambien(String chuoinhan,Garden *garden){
  int findB = -1;
  int findC = -1;
  int findD = -1;
  int findE = -1;

  findB = chuoinhan.indexOf("B");
  findC = chuoinhan.indexOf("C");
  findD = chuoinhan.indexOf("D");
  findE = chuoinhan.indexOf("E");

  if(findB >= 0 && findC >= 0){
    String data = chuoinhan.substring(findB + 1, findC);
    garden->setNhietDo(data.toFloat());
  }
  if(findC >= 0 && findD >= 0){
    String data = chuoinhan.substring(findC + 1, findD);
    garden->setDoAm(data.toFloat());
  }
  if(findD >= 0 && findE >= 0){
    String data = chuoinhan.substring(findD + 1, findE);
    garden->setDoAmDat(data.toFloat());
  }
}

void XuLyChuoiMQTT(String chuoinhanESP){
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

String JsonGarden1(){
    static String html;
    html = 
    "{ \"nhietdo\": " + (String)garden1.getNhietDo() + 
    ", \"doam\": " + (String)garden1.getDoAm() + 
    ", \"doamdat\": "+ (String)garden1.getDoAmDat() +
    ", \"light\" : " + (String)garden1.getLight() +
    ", \"fan\" : " + (String)garden1.getFan() +
    ", \"pump\" : " + (String)garden1.getPump() +
    ", \"mode\" : " + (String)garden1.getMode();

    return html;
}

String JSONGarden2(){
    static String html;
    html = 
    ", \"nhietdo1\": " + (String)garden2.getNhietDo() + 
    ", \"doam1\": " + (String)garden2.getDoAm() + 
    ", \"doamdat1\": "+ (String)garden2.getDoAmDat() +
    ", \"light1\" : " + (String)garden2.getLight() +
    ", \"fan1\" : " + (String)garden2.getFan() +
    ", \"pump1\" : " + (String)garden2.getPump() +
    ", \"mode1\" : " + (String)garden2.getMode();
    
    return html;
}

String JsonGate(){
  static String html;
  html = 
  ", \"chedo\" : " + (String)gate.getCheDo() + 
  ", \"maybom\" : " + (String)gate.getMayBom() + 
  ", \"docao\" : " + (String)gate.getDoCao() + " }";
  return html;
}

void sendArduino(){
  static String send;
  send = 
  "A" + (String)garden1.getPump() + 
  "B" + (String)garden1.getFan() + 
  "C" + (String)garden1.getLight() + 
  "D" + (String)garden1.getMode() + "E";
  Serial_ESP.println(send);
  send =
  "E" + (String)garden2.getPump() + 
  "F" + (String)garden2.getFan() + 
  "G" + (String)garden2.getLight() + 
  "H" + (String)garden2.getMode() + "I";
  Serial_ESP.println(send);
  send = 
  "I" + (String)gate.getCheDo() + 
  "J" + (String)gate.getMayBom() + "K";
  Serial.println(send);
  Serial_ESP.println(send);
}
void sendMQTT(){
    client.publish(PUB_TOPIC,JsonGarden1().c_str());
    client.publish(PUB_TOPIC,JSONGarden2().c_str());
    client.publish(PUB_TOPIC,JsonGate().c_str());
}

void sendData(){
  static int last = millis();
  if( millis() - last > 2000){
    sendMQTT();
    sendArduino();
    last = millis();
  }
}
