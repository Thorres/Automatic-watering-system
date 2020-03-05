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
    initModule();
}

void loop() {
    buttonState = digitalRead(buttonPin);
    if(!Serial1.available()){
        while(!Serial1.available()){
            delay(100);
        }
        printResponse(2000);
    }
    if(buttonState == HIGH){
        digitalWrite(ledPin, HIGH);
        counter = counter + 1;
        Serial.write(counter);
        strTwo = strOne + counter;
        body = strTwo + "\"}}";
        makeTCPRequest(body);
        digitalWrite(ledPin, LOW);
    }
}

void initModule() {
    Serial1.println("AT+RST");
    printResponse(5000);
    Serial1.println("AT+CWMODE=3");
    printResponse(5000);
    Serial1.println("AT+CIPMUX=1");
    printResponse(5000);
    Serial1.println("AT+CWJAP=\"Willard_2.0\",\"after5a8\"");
    printResponse(10000);
    Serial1.println("AT+CIPSERVER=1");
    printResponse(2000);
    Serial1.println("AT+CIFSR");
    printResponse(2000);
}

void makeTCPRequest(String body) {
    Serial1.println("AT+CIPSTART=\"TCP\",\"192.168.0.102\",5000"); // Make the TCP connection
    printResponse(2000);
    Serial1.println("AT+CIPSEND=51");                        // Send the length of the data to be sent
    printResponse(2000);
    Serial1.println(body);
    printResponse(2000);
    Serial1.println("AT+CIPCLOSE");
    printResponse(2000);
}

void printResponse(const int timeout){
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
}