#include "Grill.h"

byte in_on = A2;         //A
byte in_powerup = A0;    //B
byte in_powerdown = A3;  //C
byte in_sleep = A1;      //D
byte result_of_scankeys = 0; //результат сканирования кнопок
byte mode; //режим
byte object; //цель, до которой будет подтягиваться мощность
byte power = 0; // Мощность, которая выдаётся на насос
byte minimum_of_power = 20; // Минимальное значение мощности
byte maximum_of_power = 255; //Максимально значение мощности
byte step_power = 5; // Шаг, с которым уменьшается / увеличивается мощность
long previous_time_change = 0; //Переменная, которая хранит время, прошедшее с последнего подтягивания мощности
long delay_for_change = 100;//Время, которое должно пройти для следующего подтягивания мощности

void setup() 
{
  Serial.begin (9600);
}

void loop ()
{
  
}

byte ScanKeys () //сканируем кнопки
{
  byte result = 0;
  if (digitalRead (in_on) == HIGH) {
    result = 1;   
  } else if (digitalRead (in_powerup) == HIGH) {
    result = 2; 
  } else if ( digitalRead (in_powerdown) == HIGH) {
    result = 3;
  } else if ( digitalRead (in_sleep) == HIGH) {
    result = 4;
  } else {
    result = 0;
  }
  return result;
}

void PowerIncreaseDecrease (byte power_after_change)
{
  if ((millis () - previous_time_change) >= delay_for_change) {
    if (power > power_after_change) {
      power -= step_power;
      if (power < minimum_of_power)
        power = 0;
    } else if (power < power_after_change) {
      power += step_power;
      if (power > maximum_of_power)
        power = maximum_of_power;
    }
  previous_time_change = millis ();
  }
}

void LogicOfWork (byte result_of_scankeys) 
{
  if (result_of_scankeys == 1) {
    if (mode == 0)
      mode = 1;
    if (mode == 1)
      mode = 3;
    if (mode == 2)
      mode = 3;
  }

  if (result_of_scankeys == 2) {
    if (mode == 1)
      object++;
    if (mode == 2)
      object++;
    if (mode == 3)
      mode = 2;
  }

  if (result_of_scankeys == 3) {
    if (mode == 2)
      object--;
    if (mode == 3)
      mode = 2;
  }

  if (result_of_scankeys == 4) {
    if (mode == 1)
      mode = 0;
    if (mode == 2)
      mode = 0;
    if (mode == 3)
      mode = 1;
  }
  
}
