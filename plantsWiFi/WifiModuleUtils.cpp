#include "Arduino.h"
#include "WifiModuleUtils.h"

WifiModule::WifiModule(String SSID, String Password, String TCPServer, String TCPPort){
    _SSID = SSID;
    _Password = Password;
    _WifiConnectionString = "AT+CWJAP=\"" + SSID + "\",\"" + Password + "\"";
    _TCPConnectionString = "AT+CIPSTART=0,\"TCP\",\"" + TCPServer + "\"," + TCPPort;
}

void WifiModule::initialize(){
    Serial.println("********** Beginning initialization of the WifiModule **********");
    Serial1.println("AT+RST");
    printResponse(5000);
    Serial1.println(_WifiConnectionString);
    printResponse(10000);
    Serial1.println("AT+CIFSR");
    printResponse(2000);
    Serial1.println("AT+CWMODE=3");
    printResponse(1000);
    Serial1.println("AT+CIPMUX=1");
    printResponse(1000);
    Serial1.println("AT+CIPSERVER=1");
    printResponse(2000);
    Serial.println("********** Initialization finished! **********");
}

void WifiModule::makeTCPRequest(String body) {
    Serial1.println(_TCPConnectionString);
    printResponse(2000);
    String sendLength = "AT+CIPSEND=0," + String(body.length());
    Serial.println("sendLength : " + sendLength);
    Serial1.println(sendLength);                       
    printResponse(2000);
    Serial1.println(body);
    printResponse(3000);
    Serial1.println("AT+CIPCLOSE=0");
    printResponse(2000);
}

String WifiModule::printResponse(long int timeout){
    String response = "";
    long int time = millis();
    while((time+timeout) > millis()){
        while(Serial1.available()){
            char c = Serial1.read();
            response += c;
        }
    }

    Serial.println(response);
    return response;
}

