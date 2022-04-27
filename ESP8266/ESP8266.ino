
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
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
bool stringComplete = false;

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
      client.publish(PUB_TOPIC,inputString.c_str());
    //   Serial.print((JSON(inputString)));
    //   client.publish(PUB_TOPIC,(JSON(inputString)).c_str());
      inputString = "";
      stringComplete = false;
    }
  }
}
