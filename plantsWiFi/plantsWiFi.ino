#include "WifiModuleUtils.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimeLib.h>

String SSID = "Willard_2.0";
String Password = "after5a8";
String TCPServer = "192.168.0.102";
String TCPPort = "5000";

const int buttonPin = 50;
const int ledPin = 51;

int buttonState = 0;
int ledState = LOW;

WifiModule wifiModule(SSID, Password, TCPServer, TCPPort);

void loop() {
    if(Serial1.available() > 0){
        String commandReceived = wifiModule.printResponse(2000);
    }

    buttonState = digitalRead(buttonPin);
    if(buttonState == HIGH){
        digitalWrite(ledPin, HIGH);
        String body = "{\"data\": {\"Plant\":{\
            \"PlantName\": \"Ti cactus\",\
            \"humidityLevel\": \"72\"}}}";
        wifiModule.makeTCPRequest(body);
        digitalWrite(ledPin, LOW);
    }
}