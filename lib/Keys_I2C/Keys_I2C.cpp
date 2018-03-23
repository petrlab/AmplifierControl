#include <Arduino.h>
#include "Keys_I2C.h"

byte Keys_I2C::_key_Addr = 0;
byte Keys_I2C::_key_up_code = 0;
byte Keys_I2C::_key_down_code = 0;
byte Keys_I2C::_key_enter_code = 0;
byte Keys_I2C::_key_on_off_code = 0;
unsigned long Keys_I2C::_prevMillis = 0;
byte Keys_I2C::_mask = 0;
volatile KEY Keys_I2C::Command = KEY::KEY_OFF;
volatile bool Keys_I2C::isPressed = false;

void Keys_I2C::init(byte key_Addr, byte interruptPin, byte key_up_code, byte key_down_code, byte key_enter_code, byte key_on_off_code){
  _key_Addr = key_Addr;
	_key_up_code = key_up_code;
	_key_down_code = key_down_code;
	_key_enter_code = key_enter_code;
	_key_on_off_code = key_on_off_code;
	_mask = key_up_code + key_down_code + key_enter_code;

	Command = KEY::KEY_OFF;

	Wire.begin();
  Wire.beginTransmission(key_Addr);
  Wire.write(0xFF);
  Wire.endTransmission();

	pinMode(interruptPin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(interruptPin), Keys_I2C::isr_key, FALLING);
}

void Keys_I2C::isr_key(){
	static bool prev_key_on_off = false;
	byte bits = 0;
	bool key_on_off;

	isPressed = false;

	
	if (millis() > _prevMillis + 50){
		interrupts();
		Wire.requestFrom(_key_Addr, (byte)1);
		if (Wire.available()){
			bits = ~Wire.read();
			key_on_off = (bits & _key_on_off_code) ? true : false;
			if (key_on_off ^ prev_key_on_off){
				isPressed = true;
				Command = (key_on_off) ? KEY_ON : KEY_OFF;
				prev_key_on_off = key_on_off;
			}
			else if (bits & _mask) {
				isPressed = true;
				Command = (bits & _key_up_code) ? KEY_UP: Command;
				Command = (bits & _key_down_code) ? KEY_DOWN: Command;
				Command = (bits & _key_enter_code) ? KEY_ENTER: Command;
			}
		}
		_prevMillis = millis();
	}
}