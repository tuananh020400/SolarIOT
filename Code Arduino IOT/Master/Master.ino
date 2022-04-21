#include <SPI.h>
#define push 2
#define LED 4

int x;
int value;

void setup()
{
    Serial.begin(9600);
    pinMode(push,INPUT);
    pinMode(LED,OUTPUT);
    pinMode(SS,OUTPUT);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV8);
    digitalWrite(SS,HIGH);
}

void loop()
{
    byte m_send,m_receive;
    value = digitalRead(push);
    if(value == HIGH)
    {
        x = 1;
    }
    else
    {
        x = 0;
    }

    digitalWrite(SS,LOW);

    m_send = x;
    m_receive = SPI.transfer(m_send);

    if(m_receive == 1)
    {
        digitalWrite(LED,HIGH);
    }
    else
    {
        digitalWrite(LED,HIGH);
    }
    delay(1000);
}
