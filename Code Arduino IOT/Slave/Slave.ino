#include<SPI.h>
#define INPUTBUTTON 2
#define LED 4

volatile bool received;
volatile byte slaveReceived,slaveSend;
int buttonvalue;
int x;

void setup()
{
    Serial.begin(9600);

    pinMode(INPUTBUTTON,INPUT);
    pinMode(LED,OUTPUT);
    pinMode(MISO,OUTPUT);

    SPCR |= _BV(SPE);
    received = false;

    SPI.attachInterrupt();

}
ISR(SPI_STC_vect)
{
    slaveReceived = SPDR;
    received = true;
}

void loop()
{
    if(received)
    {
        if(slaveReceived == 1)
        {
            digitalWrite(LED,HIGH);
        }
        else
        {
            digitalWrite(LED,LOW);
        }

        buttonvalue = digitalRead(INPUTBUTTON);

        if(buttonvalue == HIGH)
        {
            x = 1;
        }
        else
        {
            x = 0;
        }
    }
    slaveReceived = x;
    SPDR = slaveSend;
    delay(200);
}
