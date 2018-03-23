#ifndef Keys_I2C_h
#define Keys_I2C_h

#include <Wire.h>

// #ifdef __cplusplus
// extern "C" {
// #endif

#define BIT(n,i) (n>>i&1)

enum KEY{
	KEY_UP,
	KEY_DOWN,
	KEY_ENTER,
	KEY_ON,
	KEY_OFF
};

class Keys_I2C {
	public:
		static volatile bool isPressed;
		static volatile KEY Command;

		static void init(byte key_Addr, byte interruptPin, byte key_up_code, byte key_down_code, byte key_enter_code, byte key_on_off_code);

	private:
		static byte _key_Addr;
		static byte _key_up_code;
		static byte _key_down_code;
		static byte _key_enter_code;
		static byte _key_on_off_code;
		static unsigned long _prevMillis;
		static byte _mask;
		//void decodeKey();
		static void isr_key();
};

extern Keys_I2C Keys;

// #ifdef __cplusplus
// }
// #endif

#endif