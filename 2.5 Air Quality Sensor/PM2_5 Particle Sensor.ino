#include "PMS.h"
#include "SoftwareSerial.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
 
SoftwareSerial Serial1(2, 3); // RX, TX
 
PMS pms(Serial1);
PMS::DATA data;
 
void setup()
{
  Serial1.begin(9600);
  lcd.begin(20,4);
  lcd.setCursor(0, 0);
  lcd.print("Warming up");
  delay(4000);
  lcd.clear();
}
 
void loop()
{
  if (pms.read(data))
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dust Concentration");
    lcd.setCursor(0, 1);
    lcd.print("PM1.0 :" + String(data.PM_AE_UG_1_0) + "(ug/m3)");
    lcd.setCursor(0, 2);
    lcd.print("PM2.5 :" + String(data.PM_AE_UG_2_5) + "(ug/m3)");
    lcd.setCursor(0, 3);
    lcd.print("PM10  :" + String(data.PM_AE_UG_10_0) + "(ug/m3)");
    
    delay(1000);
  }
}