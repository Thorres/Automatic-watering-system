#ifndef PlantsUtils.h
#define PlantsUtils.h

#define MAX_NB_PLANTS 20
#define MAIN_MENU 1
#define OPTIONS_MENU 2
#define PLANTS_MENU 3
#define WATERING_MENU 4

#include "Arduino.h"
#include <TimeLib.h>
#include <Vector.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


struct struct_plante {
  //ces valeurs n'ont pas besoin d'etre modifiees manuellement
  time_t lastWaterting; //time code de la derniere fois que cette plante a ete arrosee
  int humidityLevel;     //derniere lecture du capteur d'humidite
  //ces valeurs doivent etre modifiees manuellement dans la partie setup()
  String name;
  String room;
  int humidityLimit;     //treshold au dessus duquel la plante sera arrosee (0-1023, sujet a ajustement)
  int analogCaptor;      //pin analog du capteur de cette plante
  int pump;               //pin digital de la base du transistor de cette plante
};

class Plants {
    public:
      size_t index;
      struct_plante storage_array[MAX_NB_PLANTS];
      int menuState;
      Vector<struct_plante> plants;

      Plants();
      void mainMenu(LiquidCrystal_I2C lcd);
      void showWatering(LiquidCrystal_I2C lcd, int index);
      void addPlant(struct_plante plant);
      void showPlants(LiquidCrystal_I2C lcd);

      ~ Plants();
    private:
};
#endif