#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          //https://github.com/esp8266/Arduino
#endif

#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define LED 16
#define PIN_AP 4

const char *ID = "GateWay";
const char *PUB_TOPIC = "subscribe";
const char *SUB_TOPIC = "publish";
const char *BROKER = "broker.emqx.io";

String inputString;
char mqttText[50];
bool stringComplete = false;

WiFiClient espClient;
PubSubClient client(espClient);
SoftwareSerial Serial_ESP(13,5);// RX D1 noi voi chan D4 - TX D6 noi voi chan 3

void callback(char *topic, byte *payload, unsigned int length)
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
  Serial_ESP.println(response);
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
      Serial.println("resetar"); //tenta abrir o portal
      if(!wifiManager.startConfigPortal("ESP32configue") ){
        Serial.println("Falha ao conectar");
        wifiManager.resetSettings();
        delay(2000);
        ESP.restart();
        delay(1000);
      }
  }
}

void setup() {
  Serial.begin(115200);
  Serial_ESP.begin(9600);
  pinMode(PIN_AP, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
  wifiManager.autoConnect("ESP32configue");
  Serial.println("WiFi Connected");
  Serial.println(wifiManager.getWiFiSSID(true));
  Serial.println(wifiManager.getWiFiPass(true));
  client.setServer(BROKER,1883);
}

void loop() {
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
  Read_UART_ESP();
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
      // Serial.print("Data nhận được: ");
      // Serial.print(inputString);
      Serial.println((JSON(inputString)));
      client.publish(PUB_TOPIC,(JSON(inputString)).c_str());
      inputString = "";
      stringComplete = false;
    }
  }
}
String JSON(String inputString){
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
  int findL = -1;
  int findM = -1;
  int findN = -1;
  int findO = -1;
  int findP = -1;
  int findQ = -1;
  int findR = -1;
  String html;

  findA = inputString.indexOf("A");
  findB = inputString.indexOf("B");
  findC = inputString.indexOf("C");
  findD = inputString.indexOf("D");
  findE = inputString.indexOf("E");
  findF = inputString.indexOf("F");
  findG = inputString.indexOf("G");
  findH = inputString.indexOf("H");
  findI = inputString.indexOf("I");
  findJ = inputString.indexOf("J");
  findK = inputString.indexOf("K");
  findL = inputString.indexOf("L");
  findM = inputString.indexOf("M");
  findN = inputString.indexOf("N");
  findO = inputString.indexOf("O");
  findP = inputString.indexOf("P");
  findQ = inputString.indexOf("Q");
  findR = inputString.indexOf("R");

  if(findA >= 0 && findB >= 0  && findC >= 0 && findD >= 0 && findE >= 0 && findF >= 0 && findG >= 0 && findH >= 0 && findI >= 0 && findJ >= 0 && findK >= 0 && findL >= 0 && findM >= 0 && findN >= 0 && findO >= 0 && findP >= 0 && findQ >= 0 && findR >= 0)
  {
   html =
  "{ \"nhietdo\": " + inputString.substring(findA + 1, findB) + 
  ", \"doam\": " + inputString.substring(findB + 1, findC) + 
  ", \"doamdat\": "+ inputString.substring(findC + 1, findD) +
  ", \"light\" : " + inputString.substring(findD + 1, findE) +
  ", \"fan\" : " + inputString.substring(findE + 1, findF) +
  ", \"pump\" : " + inputString.substring(findF + 1, findG) +
  ", \"mode\" : " + inputString.substring(findG + 1, findH) + " }";
//  ", \"nhietdo1\": " + inputString.substring(findH + 1,findI) +
//  ", \"doam1\": " + inputString.substring(findI + 1, findJ) + 
//  ", \"doamdat1\": " + inputString.substring(findJ + 1, findK) +
//  ", \"light1\" : " + inputString.substring(findK + 1, findL) + 
//  ", \"fan1\" : " + inputString.substring(findL + 1, findM) +
//  ", \"pump1\" : " + inputString.substring(findM + 1, findN) +
//  ", \"mode1\" : " + inputString.substring(findN + 1, findO) +
//  ", \"chedo\" : " + inputString.substring(findO + 1, findP) + 
//  ", \"maybom\" : " + inputString.substring(findP + 1,findQ) + 
//  ", \"docao\" : " + inputString.substring(findQ + 1, findR) + " }";
  }

  return html;

}
    
