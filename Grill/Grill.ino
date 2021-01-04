#include   "Grill.h"
#include   "Visual.h"
#include   <SdFat.h>
#include   <SPI.h>
#include   <UTFT.h>
#include   <UTFT_SdRaw.h>
#define    SD_CHIP_SELECT SS
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

UTFT myGLCD(TFT01_24SP, 5, 7, 2, 3, 4);

byte in_on = A3;         //A
byte in_powerup = A1;    //B
byte in_powerdown = A4;  //C
byte in_sleep = A2;      //D
byte led_screen = 6;
byte result_of_scankeys = 0; //результат сканирования кнопок
byte mode = 0; //режим
byte object; //цель, до которой будет подтягиваться мощность
byte power = 0; // Мощность, которая выдаётся на насос
byte power_for_start = 30;//Мощность на которой происходит включение насоса
byte previous_power = power_for_start;//Мощность, к которой возвращается насос после включения;
byte minimum_of_power = 20; // Минимальное значение мощности
byte maximum_of_power = 255; //Максимально значение мощности
byte step_power = 3; // Шаг, с которым уменьшается / увеличивается мощность
long previous_time_change_power = 0; //Переменная, которая хранит время, прошедшее с последнего подтягивания мощности
long delay_for_change_power = 0; //Время, которое должно пройти для следующего подтягивания мощности
long timer_for_start = 0; //Переменная, которая хранит время прошедшее с запуска насоса
long delay_for_start = 1500; //Время, которое включается насос
long timer_for_change_object = 0; //Переменная, которая хранит время для задержек изменеия цели
byte flag_for_change_object; //флаг, используемый в функции ChangingTheObject
byte little_step = 3; //маленький шаг изменения цели
byte big_step = 10; //большой шаг изменения цели

void setup() 
{
    pinMode (led_screen, OUTPUT);
    myGLCD.InitLCD ();
    Serial.begin (9600);
    digitalWrite (led_screen, HIGH);
    myGLCD.setFont (SevenSegNumFont);
    myGLCD.setColor(VGA_LIME);
    myGLCD.clrScr();
}

void loop ()
{
    result_of_scankeys = ScanKeys ();
    LogicOfWork (result_of_scankeys);
    Work ();
    ChangingTheObject ();
    PowerIncreaseDecrease (object);
    Monitor ();
    //MonitorOnComputer ();
    Scale();
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
    if ((millis () - previous_time_change_power) >= delay_for_change_power) {
        if (abs (power - power_after_change) <= step_power) 
            power = power_after_change;
        else {
            if (power > power_after_change) {
                power -= step_power;
                if (power < minimum_of_power)
                    power = 0;
            } else if (power < power_after_change) {
                power += step_power;
                if (power > maximum_of_power)
                    power = maximum_of_power;
            }
        }
        previous_time_change_power = millis ();
    }
}

void LogicOfWork (byte result_of_scankeys) 
{
    switch (result_of_scankeys) {
        case 1:
            switch (mode) {
                case 0:
                    mode = 1;
                    timer_for_start = millis ();
                    break;
                case 1:
                    mode = 3;
                    break;
                case 2:
                    mode = 3;
                    break; 
            }
            break;
        case 2:
            switch (mode) {
                case 1:
                    mode = 2;
                    break;
                case 3:
                    mode = 2;
                    break; 
            }      
            break;
        case 3:
            switch (mode) {
                case 3:
                    mode = 2;
                    break; 
            }
            break;
        case 4:
            switch (mode) {
                case 1:
                    mode = 0;
                    break;
                case 2:
                    mode = 0;
                    break;
                case 3:
                    mode = 2;
                    break; 
            }      
            break;    
    }
}

void Work ()
{
    switch (mode) {
        case 0:
            object = 0;
            //dreams ()
            break;
        case 1:
            power = power_for_start;
            object = power_for_start;
            if ((millis () - timer_for_start) >= delay_for_start) {
                object = previous_power;
                mode = 2;
            }
            break;
    }
}

void ChangingTheObject ()
{
    byte coefficient = 1;
    if ((result_of_scankeys != 2) && (result_of_scankeys != 3)) {
        timer_for_change_object = 0;
        flag_for_change_object = 0;
    } else { 
        if (result_of_scankeys == 3)
            coefficient = -1;
        switch (flag_for_change_object) {
            case 0:
                timer_for_change_object = millis ();
                flag_for_change_object = 1;
                break;
            case 1:
                if (millis () - timer_for_change_object > 50) { //заменить на переменную
                    object += (little_step * coefficient);
                    flag_for_change_object = 2;
                }
                break;
            case 2:
                if (millis () - timer_for_change_object > 500)
                    flag_for_change_object = 3;
                break;
            case 3:
                if (millis () - timer_for_change_object > 100){
                    object += (big_step * coefficient);
                    timer_for_change_object = millis ();
                }
                break;
        }
    }
}



void MonitorOnComputer ()
{
    Serial.print ("MODE: ");
    Serial.println (mode);
    Serial.print ("OBJECT: ");
    Serial.println (object);
    Serial.print ("POWER: ");
    Serial.println (power);
    Serial.println ();
}
