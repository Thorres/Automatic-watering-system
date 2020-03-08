#include "WifiModuleUtils.h"
#include "PlantsUtils.h"
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

#define NB_PLANTE  2
#define PIN_DROITE 22        //pin digital du bouton de controle droit 
#define PIN_CENTRE 24        //pin digital du bouton de controle centre
#define PIN_GAUCHE 26        //pin digital du bouton de controle gauche
#define PIN_POWER_CAPTEUR 30 //pin digital de l'alimentation des capteurs

struct_plante liste_plante[NB_PLANTE];

time_t derniere_lecture;           //time code de la derniere lecture des capteurs
LiquidCrystal_I2C lcd(0x27,20,4);
int tempsarrosage = 4;             //temps d'arrosage en seconde. Ajustable dans le menu 2
int i;
int tempslecture;
/*********************************************************/
Plants myPlants = Plants();
struct_plante plant1;
struct_plante plant2;
struct_plante plant3;
struct_plante plant4;
struct_plante plant5;
struct_plante plant6;

void setup()
{
    lcd.init();
    lcd.backlight();    
    lcd.setCursor(0,0);
    lcd.print("Initializing...");
    // Pins
    pinMode(buttonPin, INPUT);  //temp
    pinMode(ledPin, OUTPUT);    //temp

    pinMode(PIN_DROITE , INPUT);
    pinMode(PIN_CENTRE , INPUT);
    pinMode(PIN_GAUCHE , INPUT);

    // Serial
	  Serial.begin(115200);
    Serial1.begin(115200);
    Serial.println("Serial ports initialized!");

    // Init wifi module
    wifiModule.initialize();
    initAndAddPlants();
    myPlants.mainMenu(lcd);
    derniere_lecture = now();
}

void loop() {
  if(Serial1.available() > 0){
    String commandReceived = wifiModule.printResponse(2000);
  }

  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
      digitalWrite(ledPin, HIGH);
      String body = "Test";
      wifiModule.makeTCPRequest(body);
      digitalWrite(ledPin, LOW);
  }

  if((total_minute()-total_minute(derniere_lecture)) > tempslecture)
  {
    verif_capteur();
    arroser();
  }

  //la soustraction sera negative apres un mois complet, la valeur de lecture est donc reinitialisee
  if((total_minute()-total_minute(derniere_lecture)) < 0)
  {
    derniere_lecture = now();
  }
  if(digitalRead(PIN_DROITE) == HIGH){
    myPlants.showPlants(lcd);
  }
  if(digitalRead(PIN_GAUCHE) == HIGH){
    myPlants.mainMenu(lcd);
  }
  if(digitalRead(PIN_CENTRE) == HIGH){
    wifiModule.makeTCPRequest(wifiModule.constructAddReq(myPlants.plants[0]));
  }
  delay(100);
}

void initAndAddPlants(){
  plant1.lastWaterting = now();
  plant1.humidityLevel = 5;
  plant1.name = "Mini erable";
  plant1.room = "Cuisine";
  plant1.humidityLimit = 500;
  plant1.analogCaptor = 1;
  plant1.pump = 31;
  myPlants.addPlant(plant1);
  wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant1));

  plant2.lastWaterting = now();
  plant2.humidityLevel = 5;
  plant2.name = "Grosse nenuphare";
  plant2.room = "Cuisine";
  plant2.humidityLimit = 500;
  plant2.analogCaptor = 2;
  plant2.pump = 32;
  myPlants.addPlant(plant2);
  wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant2));

  plant3.lastWaterting = now();
  plant3.humidityLevel = 5;
  plant3.name = "Poilue pale";
  plant3.room = "Fenetre entree";
  plant3.humidityLimit = 500;
  plant3.analogCaptor = 3;
  plant3.pump = 33;
  myPlants.addPlant(plant3);
  wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant3));

  plant4.lastWaterting = now();
  plant4.humidityLevel = 5;
  plant4.name = "Poilue fonce";
  plant4.room = "Fenetre cuisine";
  plant4.humidityLimit = 500;
  plant4.analogCaptor = 4;
  plant4.pump = 35;
  myPlants.addPlant(plant4);
  wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant4));

  plant5.lastWaterting = now();
  plant5.humidityLevel = 5;
  plant5.name = "Potos";
  plant5.room = "Fenetre salon";
  plant5.humidityLimit = 500;
  plant5.analogCaptor = 5;
  plant5.pump = 36;
  myPlants.addPlant(plant5);
  wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant5));

  plant6.lastWaterting = now();
  plant6.humidityLevel = 5;
  plant6.name = "Bebe nenuphare";
  plant6.room = "Cuisine";
  plant6.humidityLimit = 500;
  plant6.analogCaptor = 6;
  plant6.pump = 32;
  myPlants.addPlant(plant6);
  wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant6));

}

void verif_capteur() {
  digitalWrite(PIN_POWER_CAPTEUR , HIGH); // Acivation des capteurs

  delay(20);

  //Lecture des valeurs d'humidite
  for(i = 0 ; i < myPlants.plants.size() ; i++)
  {
    myPlants.plants[i].humidityLevel = analogRead(myPlants.plants[i].analogCaptor);
  }
  delay(20);

  digitalWrite(PIN_POWER_CAPTEUR , LOW);   //Desactivation des capteurs
}

void arroser(){
  for(i = 0 ; i < myPlants.plants.size() ; i++)
  {
    if(myPlants.plants[i].humidityLevel > myPlants.plants[i].humidityLimit)
    {
      myPlants.showWatering(lcd, i);
      digitalWrite(myPlants.plants[i].pump , HIGH);
      delay(tempsarrosage * 1000);
      digitalWrite(myPlants.plants[i].pump , LOW);
      myPlants.plants[i].lastWaterting = now();
    }
  }
}


int total_minute()
{
  return (minute(now()) + (60 * hour(now())) + (1440 * day(now())));
}

int total_minute(time_t t)
{
  return (minute(t) + (60 * hour(t)) + (1440 * day(t)));
}