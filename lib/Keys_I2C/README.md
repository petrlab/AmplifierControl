# Keys_I2C

How to use.
'Keys' is static class.
```
#include <Arduino.h>
#include "Keys_I2C.h"

const byte keysAddressI2C = 0x21;

void setup()
{
	Serial.begin(9600);
  
  	Keys.init(keysAddressI2C, 2, 1, 4, 8, 2);
		 |          |  |  |  |  |
		 |          |  |  |  |  +------- ON/OFF Power key code 2^1 where 1 is first bit
		 |          |  |  |  +---------- ENTER (bit 4) 
		 |          |  |  +------------- DOWN (bit 2)
		 |          |  +---------------- UP (bit 0)
		 |          +------------------- Interrupt port Arduino UNO
		 +------------------------------ I2C address

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

		Keys.isPressed = false;  <--- Do not forget !!!!!
	}
}
```
