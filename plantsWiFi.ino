#define WEB_SERVER_IP "192.168.0.102"

const int buttonPin = 50;
const int ledPin = 51;
const int ledResponsePin = 53; 

long int counter = 0;
int buttonState = 0;
int ledState = LOW;

String strOne = "{\"Data\": { \"Test\" : \"FromTheArduino Body with ";
String strTwo = strOne + counter;
String body = strTwo + "\"}}";
void setup()
{
    // Pins
    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(ledResponsePin, OUTPUT);
    
    // Serial
	Serial.begin(115200);
    Serial1.begin(115200);
    Serial.println("The beginning...");
    
    // Init wifi module
    initWifiModule();
}

void loop() {
    if(Serial1.available() > 0){
        printResponse(2000);
    }

    buttonState = digitalRead(buttonPin);
    if(buttonState == HIGH){
        digitalWrite(ledPin, HIGH);
        counter = counter + 1;
        strTwo = strOne + counter;
        body = strTwo + "\"}}";
        makeTCPRequest(body);
        digitalWrite(ledPin, LOW);
    }
}

void initWifiModule() {
    Serial1.println("AT+RST");
    printResponse(5000);
    Serial1.println("AT+CWMODE=3");
    printResponse(1000);
    Serial1.println("AT+CIPMUX=1");
    printResponse(1000);
    Serial1.println("AT+CWJAP=\"Willard_2.0\",\"after5a8\"");
    printResponse(10000);
    Serial1.println("AT+CIPSERVER=1");
    printResponse(2000);
    Serial1.println("AT+CIFSR");
    printResponse(2000);
}

void makeTCPRequest(String body) {
    Serial1.println("AT+CIPSTART=0,\"TCP\",\"192.168.0.102\",5000"); // Make the TCP connection
    printResponse(2000);
    Serial1.println("AT+CIPSEND=0,51");                        // Send the length of the data to be sent
    printResponse(2000);
    Serial1.println(body);
    printResponse(2000);
    Serial1.println("AT+CIPCLOSE=0");
    printResponse(2000);
}

bool printResponse(const int timeout){
    String response = "";
    long int time = millis();
    while((time+timeout) > millis()){
        while(Serial1.available()){
            char c = Serial1.read();
            response += c;
        }
    }

    String ledHigh = "Led = HIGH";
    String ledLow = "Led = LOW";
    if(response.indexOf(ledHigh) > 0){
        digitalWrite(ledResponsePin, HIGH);
        Serial.println("Setting led to high");
    }
    else if(response.indexOf(ledLow) > 0){
        digitalWrite(ledResponsePin, LOW);
        Serial.println("Setting led to low");
    }
    Serial.println(response);

    if (response.indexOf("ERROR") > 0){
        return false;
    }
    else
        return true;
}