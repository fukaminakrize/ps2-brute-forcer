#include <Arduino.h>

#include "ps2dev.h"
#include "generator.hpp"

const int CLK = 3;
const int DATA = 4;
const int LED = 13;

const byte ENTER = 0x5A;

PS2dev keyboard(CLK, DATA);

String smbls = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`-=\\[];',./ ";
byte codes[49] = {0x1C,0x32,0x21,0x23,0x24,0x2B,0x34,0x33,0x43,0x3B,0x42,0x4B,0x3A,0x31,0x44,0x4D,0x15,0x2D,0x1B,0x2C,0x3C,0x2A,0x1D,0x22,0x35,0x1A,/**/0x70, 0x69, 0x72, 0x7A, 0x6B, 0x73, 0x74, 0x6C, 0x75, 0x7D/**/,0x0E,0x4E,0x55,0x5D,0x54,0x5B,0x4C,0x52,0x41,0x49,0x4A,0x29};

const int pinGeneratorInitState[] = {0, 0, 0, 0, 0};
Generator<5> pinGenerator("0123456789", pinGeneratorInitState);

const unsigned long action_interval = 1000;

void sendString(String str)
{
    str.toUpperCase();
    int i=0;
    int n=0;
    while (str[i]!=0)
    {
        delay(100);
        while ((str[i]!=smbls[n]) && smbls[n]!=0) n++;
        keyboard.keyboard_mkbrk((codes[n]));
        n=0;
        i++;
    }
    delay(100);
    keyboard.keyboard_mkbrk(ENTER);
};

void tryPin() {
    if (pinGenerator.isEndOfStream()) {
        Serial.println("Generator: End of stream");
        while (1) {};
    }

    const char *pin = pinGenerator.next();
    Serial.print("PIN: ");
    Serial.println(pin);
    digitalWrite(LED, HIGH);
    sendString(pin); 
    digitalWrite(LED, LOW);
}

void setup() {
    keyboard.keyboard_init();
    
    Serial.begin(9600);
    Serial.println("Start");

    pinMode(LED, OUTPUT);    
}

void loop() {
    static unsigned long t_last = millis();
    unsigned long t_now = millis();

    unsigned char leds;
    if(keyboard.keyboard_handle(&leds)) {
    }

    if ((t_now - t_last) >= action_interval) {
        tryPin();
        t_last = t_now;
    }
}
