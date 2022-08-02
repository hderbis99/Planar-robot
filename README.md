# Construction
My robot is made of four servos: 
  - SG90
  - 3 x FS5103B
  
This mechanism are powered from a battery cage, six battery 1,5[V]. Power goes into voltage stabilizers. This gives us 5[V] at the output and powers the servos. The circuit still has a joystick IDUINO which is connected to microcontoller Arduino, two systems PCF8574 which communicate on the magistarl I2C, piezzo/buzzer, four buttons, four switchs and four diodes. 
# Operation
When we switch the switches to the ON position diodes will light up and we can controlled the robot. Robot is controlled by four buttons, and joystick IDUINO. When you move the joystick right or left it turns the servo SG90 while when you move the joystick up or down it turns the servos FS5103B. Buttons are responsible for the movement of the other servos. When two buttons are pressed, the robot moves to the base position (S1: 90, S2:60, S3:60, S4:60). The position of the robot is shown on the display LCD 16x2. When the switches are in the OFF position we cannot control the robot.
# Libraries
The libraries I have used are:
  - Wire.h
  - LiquidCrystal.h
  - PCF8574.h
  - Servo.h
  - EEPROM.h
 
 All libraries are included in my project. EEPROM.h is embedded in the arduino compiler.
