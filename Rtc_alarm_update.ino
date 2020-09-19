
#include "Wire.h"

#include<EEPROM.h>
#include <LiquidCrystal.h> 
#define DS3231_I2C_ADDRESS 0x68

const int waitTime =4000;
boolean blnk=true,change = false, change2=false,change3 = false, rel = false , rel2 = false, rel3 = false;
byte Ihour, Iminute, Isecond, digit=0, disp = 0;
byte Rhour = EEPROM.read(0), Rminute = EEPROM.read(1), Rsecond = EEPROM.read(2);
const int Bt1 =8, Bt2=9, Bt3=10, Buzzer=11;
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void setup(){
 
  pinMode(Bt1, INPUT);
  pinMode(Bt2, INPUT);
  pinMode(Bt3, INPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  
  Wire.begin();
  lcd.begin(16, 2); 
  lcd.print("Start Alarm");
  lcd.setCursor(0, 1);
  if (Rhour<10){
    lcd.print("0");
  }
  lcd.print(Rhour, DEC);
  // convert the byte variable to a decimal number when displayed
  lcd.print(":");
  if (Rminute<10){
    lcd.print("0");
  }
  lcd.print(Rminute, DEC);
  lcd.print(":");
  if (Rsecond<10){
    lcd.print("0");
  }
  lcd.print(Rsecond, DEC);
  delay(2000); 
  lcd.clear();
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  Ihour = hour;
  Iminute = minute;
  Isecond = second;

  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year

}
boolean checkAlarm(byte hours, byte minutes,byte seconds){

  if(
   EEPROM.read(0) == hours
   &&
   EEPROM.read(1) == minutes
   &&
   EEPROM.read(2) == seconds
   ){
      return true;
   }else{
      return false;
     
   }
  
}//checkAlarm

void beepalarm(){
   digitalWrite(Buzzer, HIGH);//
   lcd.clear();
   lcd.setCursor(1,1);
   lcd.print("Alarm Triggered");
    for(int i = 0; i<waitTime; i++){
          if(digitalRead(Bt1) == LOW && digitalRead(Bt2) == LOW && digitalRead(Bt3) == LOW)
            delay(1);
          else
            break;}
   }

void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  lcd.clear();
  // send it to the serial monitor
  lcd.setCursor(4, 1);
  if (hour<10){
    lcd.print("0");
  }
  lcd.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  lcd.print(":");
  if (minute<10){
    lcd.print("0");
  }
  lcd.print(minute, DEC);
  lcd.print(":");
  if (second<10){
    lcd.print("0");
  }
  lcd.print(second, DEC);
  if(checkAlarm(hour,minute,second))
    beepalarm();
  else 
    digitalWrite(Buzzer, LOW);
  
  for(int i = 0; i<1000 ; i++){
  if(digitalRead(Bt1) == LOW)
    delay(1);
  else
    break;}
  
}

void DisplayUpdateTime(){
  lcd.setCursor(0,0);
  lcd.print("****Set Time****");
  if(change2== true){
    Serial.print("Bt2\n");
    if(digit < 3)
    {digit += 1;
    Serial.print("digit :");
    Serial.print(digit);
    Serial.print("\n");}
    else
    digit = 0;}
   switch(digit){
    case 1:
      if (change3 == true){
        if(Isecond < 59)
        Isecond += 1;
        else 
        Isecond = 0;}
        lcd.setCursor(10, 1);
        for(int i = 0; i<700; i++){
          if(digitalRead(Bt1) == LOW && digitalRead(Bt2) == LOW && digitalRead(Bt3) == LOW)
            delay(1);
          else
            break;}
        if(blnk == true){
          blnk = false;}
        else 
          blnk = true;
        if(blnk == true)
          lcd.print("__");
        else 
          { if (Isecond<10)
                lcd.print("0");
            lcd.print(Isecond, DEC);}
        setDS3231time(Isecond, Iminute, Ihour,2,9,3,20);
        break;
    case 2:
      if (change3 == true){
        if(Iminute < 59)
        Iminute += 1;
        else 
        Iminute = 0;}
        lcd.setCursor(10, 1);
        if (Isecond<10)
          lcd.print("0");
        lcd.print(Isecond, DEC);
        lcd.setCursor(7,1);
        for(int i = 0; i<700; i++){
          if(digitalRead(Bt1) == LOW && digitalRead(Bt2) == LOW && digitalRead(Bt3) == LOW)
            delay(1);
          else
            break;}
        if(blnk == true){
          blnk = false;}
        else 
          blnk = true;
        if(blnk == true)
          lcd.print("__");
        else 
          {if (Iminute<10)
              lcd.print("0");
           lcd.print(Iminute, DEC);}
        setDS3231time(Isecond, Iminute, Ihour,2,9,3,20);
        break;
    case 3:
      if (change3 == true){
        if(Ihour < 23)
        Ihour += 1;
        else 
        Ihour = 0;}
        lcd.setCursor(10, 1);
        if (Isecond<10)
            lcd.print("0");
        lcd.print(Isecond, DEC);
        lcd.setCursor(7,1);
        if (Iminute<10)
          lcd.print("0");
        lcd.print(Iminute, DEC);
        lcd.setCursor(4,1);
        for(int i = 0; i<700; i++){
          if(digitalRead(Bt1) == LOW && digitalRead(Bt2) == LOW && digitalRead(Bt3) == LOW)
            delay(1);
          else
            break;}
        if(blnk == true){
          blnk = false;}
        else 
          blnk = true;
        if(blnk == true)
          lcd.print("__");
        else 
          {  if (Ihour<10)
                lcd.print("0");
            lcd.print(Ihour);
          }
        setDS3231time(Isecond, Iminute, Ihour,2,9,3,20);
        break;
    }
  if(digit == 0){
  
  lcd.setCursor(4, 1);
  if (Ihour<10){
    lcd.print("0");
  }
  lcd.print(Ihour, DEC);
  // convert the byte variable to a decimal number when displayed
  lcd.print(":");
  if (Iminute<10){
    lcd.print("0");
  }
  lcd.print(Iminute, DEC);
  lcd.print(":");
  if (Isecond<10){
    lcd.print("0");
  }
  lcd.print(Isecond, DEC);}
}

void DisplayAlarmTime(){
  lcd.setCursor(0,0);
  lcd.print("***Set  Alarm***");
  if(change2== true){
    Serial.print("Bt2\n");
    if(digit < 3)
    {digit += 1;
    Serial.print("digit :");
    Serial.print(digit);
    Serial.print("\n");}
    else
    digit = 0;}
   switch(digit){
    case 1:
      if (change3 == true){
        if(Rsecond < 59)
        Rsecond += 1;
        else 
        Rsecond = 0;}
        lcd.setCursor(10, 1);
        for(int i = 0; i<700; i++){
          if(digitalRead(Bt1) == LOW && digitalRead(Bt2) == LOW && digitalRead(Bt3) == LOW)
            delay(1);
          else
            break;}
        if(blnk == true){
          blnk = false;}
        else 
          blnk = true;
        if(blnk == true)
          lcd.print("__");
        else 
          { if (Rsecond<10)
                lcd.print("0");
            lcd.print(Rsecond, DEC);}
        break;
    case 2:
      if (change3 == true){
        if(Rminute < 59)
        Rminute += 1;
        else 
        Rminute = 0;}
        lcd.setCursor(10, 1);
        if (Rsecond<10)
          lcd.print("0");
        lcd.print(Rsecond, DEC);
        lcd.setCursor(7,1);
        for(int i = 0; i<700; i++){
          if(digitalRead(Bt1) == LOW && digitalRead(Bt2) == LOW && digitalRead(Bt3) == LOW)
            delay(1);
          else
            break;}
        if(blnk == true){
          blnk = false;}
        else 
          blnk = true;
        if(blnk == true)
          lcd.print("__");
        else 
          {if (Rminute<10)
              lcd.print("0");
           lcd.print(Rminute, DEC);}
        break;
    case 3:
      if (change3 == true){
        if(Rhour < 23)
        Rhour += 1;
        else 
        Rhour = 0;}
        lcd.setCursor(10, 1);
        if (Rsecond<10)
            lcd.print("0");
        lcd.print(Rsecond, DEC);
        lcd.setCursor(7,1);
        if (Rminute<10)
          lcd.print("0");
        lcd.print(Rminute, DEC);
        lcd.setCursor(4,1);
        for(int i = 0; i<700; i++){
          if(digitalRead(Bt1) == LOW && digitalRead(Bt2) == LOW && digitalRead(Bt3) == LOW)
            delay(1);
          else
            break;}
        if(blnk == true){
          blnk = false;}
        else 
          blnk = true;
        if(blnk == true)
          lcd.print("__");
        else 
          {  if (Rhour<10)
                lcd.print("0");
            lcd.print(Rhour);
          }
        break;
    }
  if(digit == 0){
   
  lcd.setCursor(4, 1);
  if (Rhour<10){
    lcd.print("0");
  }
  lcd.print(Rhour, DEC);
  // convert the byte variable to a decimal number when displayed
  lcd.print(":");
  if (Rminute<10){
    lcd.print("0");
  }
  lcd.print(Rminute, DEC);
  lcd.print(":");
  if (Rsecond<10){
    lcd.print("0");
  }
  lcd.print(Rsecond, DEC);}
  }


void loop(){
  if (digitalRead(Bt1) == HIGH)
  { 
    if(rel == true)
    {change = true;
     Serial.print("Bt1\n");
     Serial.print(change);
     }
    else 
     change= false;
     
     rel = false;
  }
  else
    {rel = true;
    change = false;}
  if (digitalRead(Bt2) == HIGH)
  { 
    if(rel2 == true)
    {
     change2 = true;
     Serial.print("Bt2\n");
     Serial.print(change2);
     }
    else 
     change2 = false;

     rel2 = false;
  }
  else
    {rel2 = true;
    change2 = false;}

  if (digitalRead(Bt3) == HIGH)
  { 
    if(rel3 == true)
    {
     change3 = true;
     Serial.print("Bt3\n");
     Serial.print(change3);
     }
    else 
     change3 = false;

     rel3 = false;
  }
  else
    {rel3 = true;
    change3 = false;}
  if(change == true)
    {disp = disp+1;
    digit = 0;}
  if(disp > 2)
    disp = 0;
  if(disp == 1)
  {
  DisplayUpdateTime();
  delay(10);}
  if(disp == 2){
  DisplayAlarmTime();
  delay(10);}  
  if(disp == 0)
    {
      EEPROM.write(0, Rhour);
      EEPROM.write(1, Rminute);
      EEPROM.write(2, Rsecond);
      displayTime();
      } 
}         
