#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
//Matthias Hertel driver https://github.com/mathertel/LiquidCrystal_PCF8574
#include <LiquidCrystal_PCF8574.h>
//Rui Azevedo ArduinoMenu 4. https://github.com/neu-rah/ArduinoMenu
#include <menuIO/PCF8574Out.h>//arduino I2C LCD
#include <menu.h>
#include <menuIO/serialOut.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialIn.h>
#include <menuIO/PCF8574KeyIn.h>

using namespace Menu;

#define LEDPIN LED_BUILTIN
#define MAX_DEPTH 2

#define K_UP     0
#define K_DOWN   4
#define K_ESC    2
#define K_ENTER  3

enum State{
	LUFTER_ON = 0,
	HEIZUNG_ON = 1,
	HV_ON = 2,
	LUFTER_OFF = 3,
	HEIZUNG_OFF = 4
};

keyMap myBtn_map[]={
                      {K_UP,options->getCmdChar(upCmd)},
                      {K_DOWN,options->getCmdChar(downCmd)},
                      {K_ESC,options->getCmdChar(escCmd)},
                      {K_ENTER,options->getCmdChar(enterCmd)}
                    };
PCF8574KeyIn<5,0x21> myButton(myBtn_map);

LiquidCrystal_PCF8574 lcd(0x27);

const char *devName[] = {"Lufter ", "Heizung"};
int timeDelay[5];
// int lufterTimeDelayOn;
// int heizungTimeDelayOn;
// int nvTimeDelayOn;
// int lufterTimeDelayOff;
// int heizungTimeDelayOff;

int ledCtrl=LOW;

volatile bool isMenuEnable;
volatile bool isMenuVisiable;

result LufterOn() {
  EEPROM.update(0, highByte(timeDelay[LUFTER_ON]));
  EEPROM.update(1, lowByte(timeDelay[LUFTER_ON]));
  return proceed;
}

result HeizungOn() {
  EEPROM.update(2, highByte(timeDelay[HEIZUNG_ON]));
  EEPROM.update(3, lowByte(timeDelay[HEIZUNG_ON]));
  return proceed;
}

result HVOn() {
  EEPROM.update(4, highByte(timeDelay[HV_ON]));
  EEPROM.update(5, lowByte(timeDelay[HV_ON]));
  return proceed;
}

result LufterOff() {
  EEPROM.update(6, highByte(timeDelay[LUFTER_OFF]));
  EEPROM.update(7, lowByte(timeDelay[LUFTER_OFF]));
  return proceed;
}

result HeizungOff() {
  EEPROM.update(8, highByte(timeDelay[HEIZUNG_OFF]));
  EEPROM.update(9, lowByte(timeDelay[HEIZUNG_OFF]));
  return proceed;
}

result setTimeDelayToDefault() {
	timeDelay[LUFTER_ON]=30;
	timeDelay[HEIZUNG_ON]=180;
	timeDelay[HV_ON] = 10;
	timeDelay[LUFTER_OFF]=60;
	timeDelay[HEIZUNG_OFF]=30;

	LufterOn();
	HeizungOn();
	HVOn();
	LufterOff();
	HeizungOff();

	return proceed;
}

MENU(subMenuOn, "TIME DELAY ON", Menu::doNothing, Menu::noEvent, Menu::wrapStyle
  ,FIELD(timeDelay[LUFTER_ON],"Lufter ","s",30,300,10,1, LufterOn, updateEvent, Menu::noStyle)
  ,FIELD(timeDelay[HEIZUNG_ON],"Heizung","s",180,300,10,1,HeizungOn, updateEvent, Menu::noStyle)
  ,FIELD(timeDelay[HV_ON],"HV     ","s",5,300,10,1,HVOn, updateEvent, Menu::noStyle)
	,EXIT("Back")
);

MENU(subMenuOff, "TIME DELAY OFF", Menu::doNothing, Menu::noEvent, Menu::wrapStyle
  ,FIELD(timeDelay[LUFTER_OFF],"Lufter ","s",60,300,10,1, LufterOff, updateEvent, Menu::noStyle)
  ,FIELD(timeDelay[HEIZUNG_OFF],"Heizung","s",30,300,10,1,HeizungOff, updateEvent, Menu::noStyle)
	,EXIT("Back")
);

MENU(mainMenu, "    DELAY SETUP", Menu::doNothing, Menu::noEvent, Menu::wrapStyle
	,SUBMENU(subMenuOn)
	,SUBMENU(subMenuOff)
	,OP("DEFAULT", setTimeDelayToDefault, Menu::enterEvent)
  ,EXIT("Exit")
);

MENU_INPUTS(in,&myButton);

MENU_OUTPUTS(out,MAX_DEPTH
  ,LCD_OUT(lcd,{0,0,20,4})
  ,NONE
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);

void delaySeconds(unsigned long timeSeconds){
	for(unsigned long i = 0; i < timeSeconds * 1000; i++)
	{
		delayMicroseconds(1000);
	};
}

void setup() {
	isMenuEnable = true;

  pinMode(LEDPIN, OUTPUT);

	timeDelay[LUFTER_ON]=EEPROM.read(0)<<8 | EEPROM.read(1);
	timeDelay[HEIZUNG_ON]=EEPROM.read(2)<<8 | EEPROM.read(3);
	timeDelay[HV_ON] = EEPROM.read(4)<<8 | EEPROM.read(5);
	timeDelay[LUFTER_OFF]=EEPROM.read(6)<<8 | EEPROM.read(7);
	timeDelay[HEIZUNG_OFF]=EEPROM.read(8)<<8 | EEPROM.read(9);

	lcd.begin(20,4);
  lcd.setBacklight(255);
	nav.showTitle = true;
	nav.useUpdateEvent = true;
	nav.idleOn();

	//nav.doNav(navCmd(downCmd));

   Serial.begin(115200);
   while(!Serial);
	// Serial.println(devName[0]);
  // Serial.println(devName[1]);
  // Serial.println("Menu 4.x");
  // Serial.println("Use keys + - * /");
  // Serial.println("to control the menu navigation");
}

int count = 0;

void loop() {
	if (isMenuEnable)
	{
		nav.poll();
		if (nav.sleepTask)
		{
			isMenuVisiable = false;
			lcd.setCursor(2, 1);
			lcd.print("Ready. Let's go.");
		}
		else{
			isMenuVisiable = true;
		};
	};
	digitalWrite(LEDPIN, ledCtrl);
}