//#include "WifiModuleUtils.h"
#include "PlantsUtils.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimeLib.h>

/*#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

String SSID = "Willard_2.0";
String Password = "after5a8";
String TCPServer = "192.168.0.102";
String TCPPort = "5000";*/

//const int buttonPin = 50;
const int ledPin = 51;

int buttonState = 0;
int ledState = LOW;

//variable debug
int minute_passee;
int minute_dern_lecture;

//WifiModule wifiModule(SSID, Password, TCPServer, TCPPort);

#define NB_PLANTE  6
#define PIN_DROITE 38         //pin digital du bouton de controle droit 
#define PIN_CENTRE 36         //pin digital du bouton de controle centre
#define PIN_GAUCHE 34        //pin digital du bouton de controle gauche
#define PIN_POWER_CAPTEUR 11 //pin digital de l'alimentation des capteurs

#define MAIN_MENU 0
#define PLANTS_MENU 1
#define WATERING_MENU 2
#define VALUES_MENU 3
#define MODIFICATION_MENU 4

struct_plante liste_plante[NB_PLANTE];

time_t derniere_lecture;           //time code de la derniere lecture des capteurs
LiquidCrystal_I2C lcd(0x27,20,4);
int i;
int menuState;
int buttonClicked;
int mainIndex;
bool newMenu;
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
    menuState = MAIN_MENU;
    lcd.init();
    lcd.backlight();    
    lcd.setCursor(0,0);
    lcd.print("Initializing...");
    // Pins
    //pinMode(buttonPin, INPUT);  //temp
    pinMode(ledPin, OUTPUT);      //temp

    pinMode(PIN_DROITE , INPUT);
    pinMode(PIN_CENTRE , INPUT);
    pinMode(PIN_GAUCHE , INPUT);

    // Serial
	  Serial.begin(115200);
    //Serial1.begin(115200);
    Serial.println("Serial ports initialized!");

    // Init wifi module
    //wifiModule.initialize();
    initAndAddPlants();
    myPlants.mainMenu(lcd, 0);
    derniere_lecture = now();
}

void loop() {
  /*if(Serial1.available() > 0){
    //String commandReceived = wifiModule.printResponse(2000);
  }*/

  //buttonState = digitalRead(buttonPin);
  /*if(buttonState == HIGH){
      digitalWrite(ledPin, HIGH);
      //String body = "Test";
      //wifiModule.makeTCPRequest(body);
      digitalWrite(ledPin, LOW);
  }*/
  if((total_minute()-total_minute(derniere_lecture)) > myPlants.readingInterval)
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
    buttonClicked = PIN_DROITE;
    //Serial.println("Bouton droit");
  }
  if(digitalRead(PIN_GAUCHE) == HIGH){
    buttonClicked = PIN_GAUCHE;
    //Serial.println("Bouton gauche");
  }
  if(digitalRead(PIN_CENTRE) == HIGH){
    buttonClicked = PIN_CENTRE;
    //Serial.println("Bouton centre");
    //wifiModule.makeTCPRequest(wifiModule.constructAddReq(myPlants.plants[0]));
  }
  manageMenus(buttonClicked);
  buttonClicked = 0;
  delay(100);
}

void initAndAddPlants(){
  //plant1.lastWaterting = now();
  plant1.humidityLevel = 5;
  plant1.name = "Mini erable";
  plant1.room = "Cuisine";
  plant1.humidityLimit = 500;
  plant1.analogCaptor = 1;
  plant1.pump = 31;
  plant1.wateringDuration = 5;
  myPlants.addPlant(plant1, 0);
  delay(200);
  //wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant1));*/

  //plant2.lastWaterting = now();
  plant2.humidityLevel = 5;
  plant2.name = "Gros nenuphare";
  plant2.room = "Cuisine";
  plant2.humidityLimit = 500;
  plant2.analogCaptor = 2;
  plant2.pump = 32;
  plant2.wateringDuration = 5;
  myPlants.addPlant(plant2, 1);
  delay(200);
  //wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant2));

  //plant3.lastWaterting = now();
  plant3.humidityLevel = 5;
  plant3.name = "Poilue pale";
  plant3.room = "Fenêtre entree";
  plant3.humidityLimit = 500;
  plant3.analogCaptor = 3;
  plant3.pump = 33;
  plant3.wateringDuration = 5;
  myPlants.addPlant(plant3, 2);
  delay(200);
  //wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant3));

  //plant4.lastWaterting = now();
  plant4.humidityLevel = 5;
  plant4.name = "Poilue fonce";
  plant4.room = "Cuisine lavabo";
  plant4.humidityLimit = 500;
  plant4.analogCaptor = 4;
  plant4.pump = 35;
  plant4.wateringDuration = 5;
  myPlants.addPlant(plant4, 3);
  delay(200);

  //wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant4));

 // plant5.lastWaterting = now();
  plant5.humidityLevel = 5;
  plant5.name = "Potos";
  plant5.room = "Fenetre salon";
  plant5.humidityLimit = 500;
  plant5.analogCaptor = 5;
  plant5.pump = 36;
  plant5.wateringDuration = 5;
  myPlants.addPlant(plant5, 4);
  delay(200);

  //wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant5));

  //plant6.lastWaterting = now();
  plant6.humidityLevel = 5;
  plant6.name = "Bebe nenuphare";
  plant6.room = "Cuisine";
  plant6.humidityLimit = 500;
  plant6.analogCaptor = 6;
  plant6.pump = 32;
  plant6.wateringDuration = 5;
  myPlants.addPlant(plant6, 5);
  delay(200);

  //wifiModule.makeTCPRequest(wifiModule.constructAddReq(plant6));
  Serial.println(myPlants.plants[0].name);
  Serial.println(myPlants.plants[1].name);
  Serial.println(myPlants.plants[2].name);
  Serial.println(myPlants.plants[3].name);
  Serial.println(myPlants.plants[4].name);
  Serial.println(myPlants.plants[5].name);
  Serial.println(myPlants.plants[6].name);

}

void verif_capteur() {
  digitalWrite(PIN_POWER_CAPTEUR , HIGH); // Acivation des capteurs

  delay(20);

  //Lecture des valeurs d'humidite
  for(i = 0 ; i < /*myPlants.plants.size()*/ NB_PLANTE ; i++)
  {
    myPlants.plants[i].humidityLevel = analogRead(myPlants.plants[i].analogCaptor);
  }
  delay(20);

  digitalWrite(PIN_POWER_CAPTEUR , LOW);   //Desactivation des capteurs
}

void arroser(){
  for(i = 0 ; i < /*myPlants.plants.size()*/  NB_PLANTE; i++)
  {
    if(myPlants.plants[i].humidityLevel > myPlants.plants[i].humidityLimit)
    {
      myPlants.showWatering(lcd, i);
      digitalWrite(myPlants.plants[i].pump , HIGH);
      delay(myPlants.plants[i].wateringDuration * 1000);
      digitalWrite(myPlants.plants[i].pump , LOW);
      //myPlants.plants[i].lastWaterting = now();
    }
  }
}

void arroserOnePLant(int index){
  myPlants.showWatering(lcd, index);
  digitalWrite(myPlants.plants[index].pump , HIGH);
  delay(myPlants.plants[index].wateringDuration * 1000);
  digitalWrite(myPlants.plants[index].pump , LOW);
  //myPlants.plants[index].lastWaterting = now();
}


int total_minute(){
  minute_passee = minute(now()) + (60 * hour(now())) + (1440 * day(now()));
  //Serial.println("Minute passee");
  return (minute(now()) + (60 * hour(now())) + (1440 * day(now())));
}

int total_minute(time_t t){
  //Serial.println("Minute actuel");
  return (minute(t) + (60 * hour(t)) + (1440 * day(t)));
}

void manageMenus(int buttonClicked){
  switch (menuState){
  case MAIN_MENU:
    if(newMenu){
      myPlants.mainMenu(lcd, mainIndex);
      newMenu = false;
    }
    if(buttonClicked == PIN_DROITE){
      if(mainIndex == 2){mainIndex = 0;}
      else {mainIndex++;}
      myPlants.mainMenu(lcd, mainIndex);
    }
    else if(buttonClicked == PIN_GAUCHE){
      if(mainIndex == 0){mainIndex = 2;}
      else {mainIndex--;}
      myPlants.mainMenu(lcd, mainIndex);
    }
    else if(buttonClicked == PIN_CENTRE){
      menuState = mainIndex+1;
      mainIndex = 0;
      newMenu = true;
    }
    break;
  case PLANTS_MENU:
    if(newMenu){
      myPlants.showPlants(lcd, mainIndex);
      Serial.println(mainIndex);
      newMenu = false;
    }
    if(buttonClicked == PIN_DROITE){
      if(mainIndex == 6){
        mainIndex = 0;
        Serial.println(mainIndex);
        myPlants.showPlants(lcd, mainIndex);
      }
      else {
        mainIndex++;
        myPlants.showPlants(lcd, mainIndex);
        Serial.println(mainIndex);
      }
    }
    else if(buttonClicked == PIN_GAUCHE){
      if(mainIndex == 0){
        mainIndex = 6;
        myPlants.showPlants(lcd, mainIndex);
        Serial.println(mainIndex);
      }
      else {
        mainIndex--;
        myPlants.showPlants(lcd, mainIndex);
        Serial.println(mainIndex);
      }
    }
    else if(buttonClicked == PIN_CENTRE){
      if(mainIndex == 6){
        menuState = MAIN_MENU;
        mainIndex = 0;
        newMenu = true;
      }
    }
    break;
  case WATERING_MENU:
    if(newMenu){
      myPlants.manualWatering(lcd, mainIndex);
      newMenu = false;
    }
    if(buttonClicked == PIN_DROITE){
      if(mainIndex == 6){
        mainIndex = 0;
        myPlants.manualWatering(lcd, mainIndex);
      }
      else {
        mainIndex++;
        myPlants.manualWatering(lcd, mainIndex);
      }
    }
    else if(buttonClicked == PIN_GAUCHE){
      if(mainIndex == 0){
        mainIndex = 6;
        myPlants.manualWatering(lcd, mainIndex);
      }
      else {
        mainIndex--;
        myPlants.manualWatering(lcd, mainIndex);
      }
    }
    else if(buttonClicked == PIN_CENTRE){
      if(mainIndex == 6){
        menuState = MAIN_MENU;
        mainIndex = 0;
      }
      else{
        arroserOnePLant(mainIndex);
      }
    newMenu = true;
    }
    break;
  case VALUES_MENU:
    if(newMenu){
      myPlants.selectValueToModify(lcd, mainIndex);
      newMenu = false;
    }
    if(buttonClicked == PIN_DROITE){
      if(mainIndex == 7){
        mainIndex = 0;
      }
      else{
        mainIndex ++;
      }
      myPlants.selectValueToModify(lcd, mainIndex);
    }
    else if(buttonClicked == PIN_GAUCHE){
      if(mainIndex == 0){
        mainIndex = 7;
      }
      else{
        mainIndex --;
      }
      myPlants.selectValueToModify(lcd, mainIndex);
    }
    else if(buttonClicked == PIN_CENTRE){
      if(mainIndex == 7){
        menuState = MAIN_MENU;
        mainIndex = 0;
      }
      else{
        menuState = MODIFICATION_MENU;
      }
      newMenu = true;
    }
    break;
  case MODIFICATION_MENU:
    if(newMenu){
      myPlants.valueModification(lcd, mainIndex);
      newMenu = false;
    }
    if(buttonClicked == PIN_DROITE){
      if(mainIndex == 6){
        myPlants.readingInterval += 5;
      }
      else{
        myPlants.plants[mainIndex].wateringDuration += 1;
      }
      myPlants.valueModification(lcd, mainIndex);
    }
    else if(buttonClicked == PIN_GAUCHE){
      if(mainIndex == 6){
        if(myPlants.readingInterval > 5){
          myPlants.readingInterval -= 5;
        }
      }
      else{
        if(myPlants.plants[mainIndex].wateringDuration > 0){
          myPlants.plants[mainIndex].wateringDuration -= 1;
        }
      }
      myPlants.valueModification(lcd, mainIndex);    
    }
    else if(buttonClicked == PIN_CENTRE){
      menuState = VALUES_MENU;
      mainIndex = 0;
      newMenu = true;
    }
    break;
  default:
    break;
  }
}