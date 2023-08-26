// Temperature functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long blinkTime = 0;
String tempPosition;

byte Clock[8] =
  {
    0b10001,
    0b01110,
    0b10101,
    0b10101,
    0b10111,
    0b10001,
    0b01110,
    0b00000
  };
byte Thermometer[8] =  //icon for termometer
  {
    0b00100,
    0b01010,
    0b01010,
    0b01110,
    0b01110,
    0b11111,
    0b11111,
    0b01110
  };
byte degreeCentigrade[8] = {
  0b11000,
  0b11000,
  0b00110,
  0b01001,
  0b01000,
  0b01000,
  0b01001,
  0b00110
};

void setup() {
  Serial.begin(9600);

  lcd.init();       // initialize the lcd
  lcd.backlight();  //To Power ON the back light

  lcd.createChar(1, Clock);
  lcd.createChar(3, Thermometer);
  lcd.createChar(4, degreeCentigrade);

#ifndef ESP8266
  while (!Serial)
    ;  // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      ;
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time
  }
}

void loop() {
  DateTime now = rtc.now();

  lcd.setCursor(0, 1);
      lcd.write(1);
      // lcd.print(" ");

      if(now.hour() == 0)
      {
        lcd.print("12");
      }
      else if(now.hour() > 12)
      {
          int Hour=now.hour()-12;
          if(Hour < 10)
          {
            lcd.print("0");
            lcd.print(Hour, DEC);
          }
          else{
            lcd.print(Hour, DEC);          
          }
      }
      else
      {
        int Hour=now.hour();
        if(Hour < 10)
          {
            lcd.print("0");
            lcd.print(Hour, DEC);
          }
          else{
            lcd.print(Hour, DEC);          
          }
      }

      lcd.print(':');
      if(now.minute() < 10)
      {
        lcd.print("0");
        lcd.print(now.minute(), DEC);
      }
      else
      {
        lcd.print(now.minute(), DEC);
      }
      lcd.print(':');
      if(now.second() < 10)
      {
        lcd.print("0");
        lcd.print(now.second(), DEC);
      }
      else
      {
        lcd.print(now.second(), DEC);
      }
      //AM PM srart
      if(now.hour() >= 12)
      {
        // lcd.print(" ");
        lcd.print("PM");
      }
      else
      {
        // lcd.print(" ");
        lcd.print("AM");
      }
      //AM PM end
      // time end

      lcd.print(" ");
      //temperature start
      lcd.write(3);
      int Temperature=(rtc.getTemperature());
      lcd.print(Temperature);
      lcd.write(4);
      //temperature end



  lcd.setCursor(0, 0);
  if (millis() - blinkTime > 4500) {
    lcd.print("ROOM TEMPERATURE");
    blinkTime = millis();
  }
  if (millis() - blinkTime > 2500) {
    lcd.print(tempPosition);
  }
  ///////////////////////////////////////
  if (Temperature <= 25.00) {
    tempPosition = "     COLD       ";
  } else if (Temperature >= 26.00 && Temperature <= 30.00) {
    tempPosition = "  NORMAL COLD   ";
  } else if (Temperature >= 31.00 && Temperature <= 35.00) {
    tempPosition = "     NORMAL     ";
  } else if (Temperature >= 36.00 && Temperature <= 40.00) {
    tempPosition = "      HOT       ";
  } else if (Temperature >= 41.00 && Temperature <= 43.00) {
    tempPosition = "    VERY HOT    ";
  } else if (Temperature >= 44.00) {
    tempPosition = "  PRAIRIE FIRE  ";
  }
  //////////////////////////////////////
}