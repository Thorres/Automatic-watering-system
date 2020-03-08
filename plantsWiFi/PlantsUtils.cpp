#include "PlantsUtils.h"


Plants::Plants(){
    Serial.println("Initializing Plants");
    plants.setStorage(storage_array);
    index = 0;
    menuState = MAIN_MENU;
}

void Plants::mainMenu(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Welcome to PlantsApp");
    lcd.setCursor(0,1);
    lcd.print("1 - Navigate Plants");
    lcd.setCursor(0,2);
    lcd.print("2 - Set interval");
    lcd.setCursor(0,3);
    lcd.print("3 - Options");
    menuState = MAIN_MENU;
}

void Plants::showWatering(LiquidCrystal_I2C lcd, int index) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Arrosage de :");
    lcd.setCursor(0,1);
    lcd.print(plants[index].name);
    lcd.setCursor(0,2);
    lcd.print("en cours");
    menuState = WATERING_MENU;
}

void Plants::showPlants(LiquidCrystal_I2C lcd){
    if(index == plants.size()){
        index = 0;
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("name:");
    lcd.print(plants[index].name);
    index = index + 1;
    menuState = PLANTS_MENU;
    delay(100);
}

void Plants::addPlant(struct_plante plant){
    plants.push_back(plant);
    Serial.println(plants.back().name);
}

Plants::~Plants(){}