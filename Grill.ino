/*
Данная версия программы "Grill" 2.2
для версии платы 2.2 с дисплеем 1602
русифицированы надписи на экране
исправлен тип данных для аналогрид с булина на интеджер
на кнопке д реализован режим раздува
реализовано включение подсветки экрана без увеличения мощности
*/
/*
Сигналы пульта
А - A2 
B - A0
C - A3
D - A1
*/

#include <LiquidCrystal.h>
#include "chars.h"

LiquidCrystal lcd (7, 6, 5, 4, 3, 2);
int InPowerOnOff = A2;
int InInflate = A1; //int InInflate = A3; // Из-за битого пина а5 используем а3
int InPowerUp = A0;
int InPowerDown = A3;
int Out = 9;  //Шим
int light = 13;
int PowerOnOff = 0;
int PowerInflate = 0;
int PowerUp = 0;
int PowerDown = 0;
int analoglevel = 500;
const int steppower = 5;
const int limit = 30;
int power = 0;
const int minm = 20;
const int maxm = 255;
int mode = 0;
int previousmode;
int object = 0;
bool flag = false;
int blue = 11;
int red = 10;
int previouspower = minm;
char s [4];
long previoustime;
long previoustime_work = 0;
long previoustime_light = 0;
long interval = 10000; //время подсветки
int scan;
long del = 100;//задержка подтягивания
int i = 0;
int step_light = 10;
const int maxm_light = 255;
const int minm_light = 0;
long time_light = 0; // счетчик времени для мигания синим
long time_light1 = 0; // счетчик времени для мигания красным при привешении лимитов
long time_light2 = 0; // счетчик времени для красного при раздуве
long time_light3 = 0; // счетчик времени для красного при раздуве при двойном мигании
long time_start;
int light_mode = 1;//режимы определяющий стадию мигания, когда мигаем синим в режиме ожидания
long start_delay_light = 100;
long mode0_delay_light = 5;//задержка между изменениями яркости светодиода, когда мигаем синим в режиме ожидания
long border_delay_light = 100;
//int minm_light = 0;
//int maxm_light = 200;
int steps_light = 5;

void chars_setup () {
  lcd.createChar(1, rus_1);
  lcd.createChar(2, rus_2);
  lcd.createChar(3, rus_3);
  lcd.createChar(4, rus_4);
  lcd.createChar(5, rus_5);
  lcd.createChar(6, rus_6);
  lcd.createChar(7, rus_7);
  lcd.createChar(0, rus_0);
}

void setup() 
{
  //Serial.begin(9600);
  pinMode(InPowerOnOff, INPUT);
  pinMode(InInflate, INPUT);
  pinMode(InPowerUp, INPUT);
  pinMode(InPowerDown, INPUT);
  pinMode(Out, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(light, OUTPUT);
  lcd.begin(16, 2); //размерность экрана
  chars_setup ();
  digitalWrite(light, HIGH);
  analogReference(DEFAULT);//опорное напряжение равно напряжению питания контроллера 
  led(5);
}

void work (int workpower) // Уменьшаем или увеличиваем мощность
{
  if ((millis () - previoustime_work) >= del) {
    if (power > workpower) {
      power = power - steppower;
      if (power < minm) {
        power = 0;
      }
    } else if (power < workpower) {
      power = power + steppower;
      if (power < minm) {
        power = minm;
      }
    }
    previoustime_work = millis ();    
  }
}

int scankeys () //сканируем кнопки
{
  int res = 0;
  PowerOnOff = analogRead (InPowerOnOff);
  //Serial.print("PowerOnOff"); Serial.print(PowerOnOff); Serial.print("\n");
  PowerInflate = analogRead (InInflate);
  PowerUp = analogRead (InPowerUp);
  PowerDown = analogRead (InPowerDown);
  if (PowerOnOff > analoglevel) {
    res = 1;   
  } else if (PowerUp > analoglevel) {
    res = 2; 
  } else if (PowerDown > analoglevel) {
    res = 3;
  } else if (PowerInflate > analoglevel) {
    res = 4;
  } else {
    res = 0;
  }
  return res;
}

void backlight (int resscankeys) //включаем и выключаем подсветку
{
  if (resscankeys != 0) {
    if (digitalRead (light) == LOW) {
      digitalWrite (light, HIGH);
      delay (300);
    }
    digitalWrite (light, HIGH);
    previoustime = millis ();
  }
  if (millis () - previoustime >= interval) {
    digitalWrite (light, LOW);
  }
}

void printscale () 
{
  String scale;
  int i = 0;
  long num = maxm / 10;
  for (i; i < maxm; i += num) {
    if (i < power) {
      scale += '*';  
    } else if (i < object) {
      scale += '-';
    } else if (i <= maxm) {
      scale += ' ';
    }
  }
  lcd.setCursor (0,1);
  lcd.print (scale);
}

void led (int arg) //пишем что то на дисплее
{
  if (arg != 2) {
    space(0);
  }
  switch (arg) {
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("3A\5\4CK");//"ЗАПУСК"
      break;
    case 2:
      printscale ();
      lcd.setCursor(12, 1);
      sprintf(s, "%3d", power * 100 / maxm);
      lcd.print(s);
      lcd.print('%');
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("OCTAHOBKA");//"ОСТАНОВКА"
      break;
    case 4:
      lcd.setCursor (0, 0);
      lcd.print("PA3");
      lcd.print(char(0));
      lcd.print("\4B");
      //"РАЗДУВ"
      break;
    case 5:
      lcd.setCursor (0, 0);
      lcd.print("O\7\3");
      lcd.print(char(0));
      lcd.print("AH\3E");
      //"ОЖИДАНИЕ"
      break;
  }
}

void space (int arg) {
  if (arg == 0) {
    lcd.setCursor(0, 0);
  }
  if (arg == 1) {
    lcd.setCursor(0, 1);
  }
  lcd.print("                 ");
}

void _light (int arg) //помаргушки
{
  switch (arg) {
    case 0: // моргаем синим в режиме ожидания
      if (mode == 0) {
        if (light_mode == 1) {
          if (millis () - time_light > mode0_delay_light) {
            if (i < maxm_light) {
              i += steps_light;
              analogWrite (blue, i);
              time_light = millis ();
            } else if (i >= maxm_light) {
              light_mode = 2;
            }
          }
        }
        if (light_mode == 2) {
          if (millis () - time_light > mode0_delay_light) {
            if (i > minm_light) {
              i -= steps_light;
              analogWrite (blue, i);
              time_light = millis ();
            } else if (i <= minm_light) {
              light_mode = 0;
              time_light = millis ();
            }
          }
        }
        if (light_mode == 0) {
          if (millis () - time_light > 5000) {//задержка между циклами
            light_mode = 1;
          }
        }
      } else if (mode == 1) {
        if (scankeys () == 2 || scankeys () == 3) {
          //Serial.print("Key press  ");
          if ((power >= maxm) || (power <= minm)) {
            //Serial.print("Limit power   ");
            if (millis () - time_light1 > 50) {
              //Serial.print("timelight   ");
              if (digitalRead (red) == LOW) {
                //Serial.print("ON\n");
                digitalWrite (red, HIGH);
                time_light1 = millis ();
              } else if (digitalRead (red) == HIGH) {
                //Serial.print("OFF\n");
                digitalWrite (red, LOW);
                time_light1 = millis ();
              }
            }
          }
        } else {
          digitalWrite (red, LOW);
        }
      } else if (mode == 2) {
        if (millis () - time_light2 > 1000) {
          digitalWrite (red, HIGH);
          delay(20);
          digitalWrite (red, LOW);
          time_light2 = millis ();
        }
      }
      break;

    case 1:  //часто моргаем синим при старте !Используется вместо паузы при старте, что бы мотор начал разгоняться! 
      time_start = millis ();
      time_light = millis ();
      while (millis () - time_start < 3000) {
        if (millis () - time_light > start_delay_light) { 
          if (digitalRead (blue) > 0) {
            analogWrite (blue, 0);
            digitalWrite (light, HIGH);
            //Serial.print("0\n");
            
          } else if (digitalRead (blue) == 0) {
            analogWrite (blue, 255);
            digitalWrite (light, LOW);
            //Serial.print("1\n"); 
          }
          time_light = millis ();
        }
        //Serial.print("\n\n");
        Serial.print(digitalRead (blue));
        //Serial.print("\n\n");
      }
      digitalWrite (blue, LOW);
      break;
  }
}

void start () 
{
  //Serial.print("Start\n"); 
  led (1); //второй аргумент не важен, так как при вызове функции с первым аргументом = 1, второй агрумент не влияет на дейсвтие функции
  power = limit;
  analogWrite (Out, power);
  _light (1);
  //delay(3000);
  object = previouspower;
  space (0);
}

void finish () 
{
  previouspower = power;
  //power = 0;
  object = 0;
  led (3);
  delay (1000);
  space (0);
}

void body (int resscankeys) 
{
  if (resscankeys == 1) {
    if (mode == 0) {
      start ();
      mode = 1;
    } else if ((mode == 1) && (power != maxm) && (power == object)) {
        led (4);
        previouspower = power;
        object = maxm;
        previousmode = mode;
        mode = 2;
    } else if ((mode == 2) && (power == maxm)) {
        space (0);
        mode = previousmode;
        if (previousmode == 0) {
          finish ();
          led (5);
        } else if (previousmode == 1) {
          object = previouspower;
      }
    }
  } else if ((resscankeys == 2) && (mode == 1) && (object == power) && (power < maxm) && ((power + steppower) <= maxm)) {
    if (digitalRead (light) == HIGH){
      object = power + steppower;
    }
  } else if ((resscankeys == 3) && (mode == 1) && (power > minm) && ((power - steppower) >= minm)) {
    if (digitalRead (light) == HIGH) {
      if ((power < object) ) {
        object = power;
      } else if (object == power) {
        object = power - steppower;
      }
    }
  } else if (resscankeys == 4) {
    if (mode != 0) {
      finish ();
      mode = 0;
      led (5);
    }
  }    
}

void loop()
{
  scan = scankeys ();
  //Serial.print("knopka "); Serial.print(scan); Serial.print("\n");
  /*Serial.print("PowerOnOff"); */
  body (scan);
  backlight (scan);
  work (object);
  analogWrite (Out, power);
  //writing ();
  led (2);
  _light (0);
}
