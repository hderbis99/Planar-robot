#include <Servo.h> // Biblioteka, odpowiedzialna za sterowanie serwomechanizmami 
#include <Wire.h> // Biblioteka służąca do komunikacji z magistralą I2C
#include <PCF8574.h> // Biblioteka obsługująca układy PCF8574
#include <LiquidCrystal.h> //Biblioteka obsługująca wyświetlacz LCD 16x2
#include <EEPROM.h>       // Biblioteka odwołująca się do pamięci EEPROM

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Informacja o podłączeniu nowego wyświetlacza

PCF8574 expander1; // Obiekty odpowiadające za pierwszy i drugi układ PCF
PCF8574 expander2;

Servo servo1;     // Inicjacja serwomechanizmów
Servo servo2;
Servo servo3;
Servo servo4;

byte Expander_1 [] = {0,1,2,3,4,5,6,7};
byte Expander_2 [] = {0,1,2,3,4,5,6,7};

uint8_t adres [] = {0, 2, 8, 16}; // Przypisanie poszczególnych adrsów pamięci EEPROM do przechowywania pozycji serwomechanizmów

#define right_joystick_y A1
#define right_joystick_x A0


class Diode                         
{
  public:
  void diode1_ON()                   
  {
   expander1.digitalWrite(7, 1); 
  }
    void diode1_OFF()
  {
    expander1.digitalWrite(7, 0); 
  }
    void diode2_ON()
  {
    expander1.digitalWrite(6, 1); 
  }
    void diode2_OFF()
  {
    expander1.digitalWrite(6, 0); 
  }
    void diode3_ON()
  {
    expander1.digitalWrite(5, 1); 
  }
    void diode3_OFF()
  {
    expander1.digitalWrite(5, 0); 
  }
    void diode4_ON()
  {
    expander1.digitalWrite(4, 1); 
  }
    void diode4_OFF()
  {
    expander1.digitalWrite(4, 0); 
  } 
};

class Control
{
  private:
  int current = 0; //Zmienna przechowująca aktualny czas
  int lastmillis = 0; // Zmienna przechowująca ostatni zmierzony czas

  int lastmillis2 = 0; 
  int lastmillis3 = 0; 
  int lastmillis4 = 0; 
  int lastmillis5 = 0; 

  bool parse1 = true;
  bool parse2 = true;
  
  public:

  int S1_axis_degree; // Zmienne przechowujące pozycję serw
  int S2_axis_degree;
  int S3_axis_degree;
  int S4_axis_degree;
  
  void diodeControl() 
  {
  Diode diode;                               
  
  if(expander1.digitalRead(0) == 1)
  {
    diode.diode1_ON();
  }
  if(expander1.digitalRead(0) == 0)
  {
    diode.diode1_OFF();
  }
  if(expander1.digitalRead(1) == 1)
  {
    diode.diode2_ON();
  }
  if(expander1.digitalRead(1) == 0)
  {
    diode.diode2_OFF();
  }
  if(expander1.digitalRead(2) == 1)
  {
    diode.diode3_ON();
  }
  if(expander1.digitalRead(2) == 0)
  {
    diode.diode3_OFF();
  }
  if(expander1.digitalRead(3) == 1)
  {
    diode.diode4_ON();
  }
  if(expander1.digitalRead(3) == 0)
  {
    diode.diode4_OFF();
  }
}

  void servoControl()                                
  {
    if(expander1.digitalRead(0) == 0 && expander1.digitalRead(1) == 0 && expander1.digitalRead(2) == 0 && expander1.digitalRead(3) == 0)
    {
      current = millis();
      if(current - lastmillis >=40)
      {
        int right_joystick_x_value = analogRead(right_joystick_x); // Zmienne przechowujące zczytaną pozycję po wychyleniu joystick'a
        int right_joystick_y_value = analogRead(right_joystick_y); 

        if(right_joystick_x_value < 340) S1_axis_degree -=5;      
        else if(right_joystick_x_value > 680) S1_axis_degree +=5; 

        if(right_joystick_y_value < 340) S4_axis_degree -=5;
        else if(right_joystick_y_value > 680) S4_axis_degree +=5;

        if(expander2.digitalRead(0) == LOW) 
        {
          S2_axis_degree+=5;
        }
        if(expander2.digitalRead(1) == LOW)
        {
          S2_axis_degree-=5;
        }
        if(expander2.digitalRead(2) == LOW)
        {
          S3_axis_degree+=5;
        }
        if(expander2.digitalRead(3) == LOW)
        {
          S3_axis_degree-=5;
        }
        basePosition_1(S1_axis_degree);      
        basePosition_2(S2_axis_degree); 
        basePosition_2(S3_axis_degree); 
        basePosition_2(S4_axis_degree);                                                       
    
        S1_axis_degree = min(180, max(0, S1_axis_degree));  // Ustawienie w jakim zakresie kątowym poruszają się serwomechanizmy
        S2_axis_degree = min(120, max(0, S2_axis_degree));
        S3_axis_degree = min(120, max(0, S3_axis_degree));
        S4_axis_degree = min(120, max(0, S4_axis_degree)); 

        lastmillis = millis(); 

         if(S1_axis_degree >=100)
         {
           lcd.setCursor(4, 0); 
           lcd.print(S1_axis_degree);
         }
         else if(S1_axis_degree < 100)
         {
           lcd.setCursor(4, 0);  
           lcd.print("0");
           lcd.setCursor(5, 0);  
           lcd.print(S1_axis_degree);
         }

         if(S2_axis_degree >=100)
         {
           lcd.setCursor(13, 0);  
           lcd.print(S2_axis_degree);
         }
         else if(S2_axis_degree < 100)
         {
           lcd.setCursor(13, 0);  
           lcd.print("0");
           lcd.setCursor(14, 0);  
           lcd.print(S2_axis_degree);
         }

         if(S3_axis_degree >=100)
         {
           lcd.setCursor(4, 1);  
           lcd.print(S3_axis_degree);
         } 
         else if(S3_axis_degree < 100)
         {
           lcd.setCursor(4, 1);  
           lcd.print("0");
           lcd.setCursor(5, 1);  
           lcd.print(S3_axis_degree);
         }

        if(S4_axis_degree >=100)
        {
          lcd.setCursor(13, 1);  
          lcd.print(S4_axis_degree);
        }
        else if(S4_axis_degree < 100)
        {
          lcd.setCursor(13, 1);  
          lcd.print("0");
          lcd.setCursor(14, 1);  
          lcd.print(S4_axis_degree);
        } 
      }
        servo1.write(S1_axis_degree); // Serwomechanizmy wykonują ruch do danej pozycji
        servo2.write(S2_axis_degree);
        servo3.write(S3_axis_degree);
        servo4.write(S4_axis_degree);

        EEPROM.put(adres[0], S1_axis_degree); // Ostatnia pozycja serwomechanizmów jest przypisywana do danego adresu w pamięci EEPROM
        EEPROM.put(adres[1], S2_axis_degree);
        EEPROM.put(adres[2], S3_axis_degree);
        EEPROM.put(adres[3], S4_axis_degree);

        if(current - lastmillis2 >= 100 && parse1 == true && parse2 == true)
        {
        String servo1_print = "one:" + String(S1_axis_degree);
        Serial.println(servo1_print); // Wyświetlenie poszczególnych pozycji w porcie szeregowym
        parse1 = false;
        lastmillis2 = millis();
        }
        if(current - lastmillis3 >= 200 && parse1 == false && parse2 == true)
        {
        String servo2_print = "two:" + String(S2_axis_degree);
        Serial.println(servo2_print);
        parse2 = false;
        lastmillis3 = millis();
        }
        if(current - lastmillis4 >= 300 && parse1 == false && parse2 == false)
        {
        String servo3_print = "three:" + String(S3_axis_degree);
        Serial.println(servo3_print);
        parse1 = true;
        lastmillis4 = millis();
        }
        if(current - lastmillis5 >= 400 && parse1 == true && parse2 == false)
        {
        String servo4_print = "four:" + String(S4_axis_degree);
        Serial.println(servo4_print);
        parse2 = true;
        lastmillis5 = millis();
        }
    }
    else
    {
      EEPROM.get(adres[0], S1_axis_degree); // Ostatnia pozycja serwomechanizmów jest odczytywana z pamięci EEPROM
      EEPROM.get(adres[1], S2_axis_degree);
      EEPROM.get(adres[2], S3_axis_degree);
      EEPROM.get(adres[3], S4_axis_degree);

      servo1.write(S1_axis_degree); 
      servo2.write(S2_axis_degree);
      servo3.write(S3_axis_degree);
      servo4.write(S4_axis_degree);
    }
  }
  void readEEPROM() // Metoda odpowiadająca za odczytanie ostatniej pozycji w jakiej znajdowały się serwomechanizmy
  {
    EEPROM.get(adres[0], S1_axis_degree);
    EEPROM.get(adres[1], S2_axis_degree);
    EEPROM.get(adres[2], S3_axis_degree);
    EEPROM.get(adres[3], S4_axis_degree);
  }

  void basePosition_1(int s_axis_degree)  // Metoda odpowiadająca za ustawienie pierwszego serwa do pozycji bazowej
  {
     if(expander2.digitalRead(0) == LOW && expander2.digitalRead(1) == LOW) 
     {
      // Serwo 1
       if(s_axis_degree < 90)
       {
         s_axis_degree +=5;
       }
       if(s_axis_degree > 90)
       {
         s_axis_degree-=5;
       }
       if(s_axis_degree == 90)
       {
         s_axis_degree +=0;
       }
     }
  }

    void basePosition_2(int s_axis_degree)  // Metoda odpowiadająca za ustawienie pozostałych serw do pozycji bazowej
  {
     if(expander2.digitalRead(0) == LOW && expander2.digitalRead(1) == LOW) 
     {
      // Serwo 1
       if(s_axis_degree < 60)
       {
         s_axis_degree +=5;
       }
       if(s_axis_degree > 60)
       {
         s_axis_degree-=5;
       }
       if(s_axis_degree == 60)
       {
         s_axis_degree +=0;
       }
     }
  }
  
  void piezzo()
  {
    if(expander1.digitalRead(0) == 0 && expander1.digitalRead(1) == 0 && expander1.digitalRead(2) == 0 && expander1.digitalRead(3) == 0)
    {
      if(digitalRead(A2) == 0)
      {
        expander2.digitalWrite(7, HIGH);
      }
      else
      {
        expander2.digitalWrite(7, LOW);
      }
    }
  }
};

class Display                            
{ 
  public:
  
  void showDisplay()                         
  {
      lcd.begin(16, 2); //Deklaracja typu
    
      lcd.setCursor(0, 0);  
      lcd.print("S1:");
      lcd.setCursor(9, 0);  
      lcd.print("S2:");

      lcd.setCursor(0, 1);  
      lcd.print("S3:");
      lcd.setCursor(9, 1);  
      lcd.print("S4:");
  }
};

void onInterrupt()
{
  expander1.checkForInterrupt();
  expander2.checkForInterrupt();
}

void expander_Output() // Procedura odpowiadająca za poprawne wysterowanie wyjść układów PCF8574
{
   for(int i = 0; i < 4; i++)
  {
    expander1.pinMode(Expander_1[i], INPUT_PULLUP);
  }
  for(int j = 4; j < 8; j++)
  {
    expander1.pinMode(Expander_1[j], OUTPUT);
  }
  for(int k = 0; k < 4; k++)
  {
    expander2.pinMode(Expander_2[k], INPUT_PULLUP);
  }
  for(int l = 4; l < 8; l++)
  {
    expander2.pinMode(Expander_2[l], OUTPUT);
  }
  pinMode(A2, INPUT_PULLUP);
}

Control robot1; 
Display digital; 

void setup() {
  Serial.begin(9600);// Port szeregowy do komunikacji z komputerem 
  
  expander1.begin(0x20);
  expander2.begin(0x21);

  servo1.attach(8); 
  servo2.attach(10);
  servo3.attach(11);
  servo4.attach(12);
  
  expander1.enableInterrupt(2, onInterrupt);
  
  expander_Output(); 

  robot1.readEEPROM();

  digital.showDisplay();
}

void loop() {
  robot1.servoControl();
  robot1.diodeControl();
  robot1.piezzo();
}
