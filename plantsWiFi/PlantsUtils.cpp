#include "PlantsUtils.h"


Plants::Plants(){
    Serial.println("Initializing Plants");
    //plants.setStorage(storage_array);
    index = 0;
}

void Plants::mainMenu(LiquidCrystal_I2C lcd, int index){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Welcome to PlantsApp");
    lcd.setCursor(((20-mainMenus[index].length())/2),2);
    lcd.print(mainMenus[index]);
    Serial.println(mainMenus[index]);
}

void Plants::showWatering(LiquidCrystal_I2C lcd, int index) {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Arrosage de ->");
    lcd.setCursor(((20-plants[index].name.length())/2),1);
    lcd.print(plants[index].name);
    lcd.setCursor(6,2);
    lcd.print("en cours");
}

void Plants::showPlants(LiquidCrystal_I2C lcd, int index){
    if(index == 6){
        lcd.clear();
        lcd.setCursor(7,1);
        lcd.print("Return");
        lcd.setCursor(4,2);
        lcd.print("to main menu");
    }
    else{
        lcd.clear();
        lcd.setCursor(((15-plants[index].name.length())/2),0);
        lcd.print("Name:");
        lcd.print(plants[index].name);
        lcd.setCursor(((14-plants[index].room.length())/2),1);
        lcd.print("Piece:");
        lcd.print(plants[index].room);
        lcd.setCursor(3,2);
        lcd.print("Last watering:");
        lcd.setCursor(6,3);
        lcd.print(day(now()) - day(plants[index].lastWaterting));
        lcd.print("d:");
        lcd.print(hour(now()) - hour(plants[index].lastWaterting));
        lcd.print("h:");
        lcd.print(minute(now()) - minute(plants[index].lastWaterting));
        lcd.print("m");
    }
}

void Plants::addPlant(struct_plante plant, int index){
    plants[index] = plant;
    delay(100);
}

void Plants::manualWatering(LiquidCrystal_I2C lcd, int index) {
    if(index == 6){
        lcd.clear();
        lcd.setCursor(7,1);
        lcd.print("Return");
        lcd.setCursor(4,2);
        lcd.print("to main menu");
    }
    else{
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Manual Watering!");
        lcd.setCursor(0,1);
        lcd.print("Push enter to water:");
        lcd.setCursor(((20-plants[index].name.length())/2),2);
        lcd.print(plants[index].name);
    }
}

void Plants::selectValueToModify(LiquidCrystal_I2C lcd, int index){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Value to modify!");
    if (index == 6){
        lcd.setCursor(0,2);
        lcd.print("Modify delay between");
        lcd.setCursor(1,3);
        lcd.print("humidity readings!");
    }
    else if(index == 7){
        lcd.setCursor(0,2);
        lcd.print("Push enter to return");
    }
    else {
        lcd.setCursor(0,2);
        lcd.print("Watering period of :");
        lcd.setCursor(((20-plants[index].name.length())/2),3);
        lcd.print(plants[index].name);
    }
}

void Plants::valueModification(LiquidCrystal_I2C lcd, int index){
    lcd.clear();
    if(index == 6){
        lcd.setCursor(0,0);
        lcd.print("Delay entre lectures");
        lcd.setCursor(5,1);
        lcd.print(readingInterval);
        lcd.print(" minutes");
        lcd.setCursor(0,3);
        lcd.print("Push enter to accept");
    }
    else {
        lcd.setCursor(0,0);
        lcd.print("Watering period for:");
        lcd.setCursor(((20-plants[index].name.length())/2),1);
        lcd.print(plants[index].name);
        lcd.setCursor(5,2);
        lcd.print(plants[index].wateringDuration);
        lcd.print(" seconds");
        lcd.setCursor(0,3);
        lcd.print("Push enter to accept");
    }
}

Plants::~Plants(){}