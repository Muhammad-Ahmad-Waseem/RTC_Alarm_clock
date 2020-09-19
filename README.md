# RTC_Alarm_clock
Alarm clock with clock update and alarm time update

Hardware required:
Arduino UNO
RTC module
external push buttons
LCD module

Use i2c pins of arduino to connect with RTC. For LCD, find pins defined above lcd initilizer.
Connect buttons at pins bt1, bt2, bt3 (use external pull down, see configurations from internet)\

bt1 is used to change screen from normal, update alarm time, update clock time
bt2 is used to move cursor between hour, minute and second
bt3 is used to increment current selected value.
