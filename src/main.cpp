#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "Keys_I2C.h"

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

const byte keysAddressI2C = 0x21;
const byte lcdAddressI2C = 0x27;

LiquidCrystal_I2C lcd(lcdAddressI2C,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

void setup()
{
	Serial.begin(9600);

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.home();
  
  Keys.init(keysAddressI2C, 2, 1, 4, 8, 2);

	Serial.println("Setup complite.");
}

void loop()
{
	String s;

	if (Keys.isPressed){
		Serial.print("Key pressed ");
		switch (Keys.Command)
		{
		case KEY_UP:
			s = "UP";		
			break;
		case KEY_DOWN:
			s = "DOWN";
			break;
		case KEY_ENTER:
			s = "ENTER";
			break;
		case KEY_ON:
			s = "POWER ON";
			break;
		case KEY_OFF:
			s = "POWER OFF";
			break;
		default:
			break;
		};

		Serial.println(s);



		Keys.isPressed = false;
	}
}
