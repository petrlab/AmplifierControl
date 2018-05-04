#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <menuIO/PCF8574Out.h>//arduino I2C LCD
#include <menu.h>
#include <menuIO/serialOut.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialIn.h>
#include <menuIO/PCF8574KeyIn.h>

using namespace Menu;

#define LEDPIN LED_BUILTIN
#define MAX_DEPTH 2

LiquidCrystal_PCF8574 lcd(0x27);

#define K_UP     0
#define K_DOWN   4
#define K_ESC    2
#define K_ENTER  3

keyMap myBtn_map[]={
                      {K_UP,options->getCmdChar(upCmd)},
                      {K_DOWN,options->getCmdChar(downCmd)},
                      {K_ESC,options->getCmdChar(escCmd)},
                      {K_ENTER,options->getCmdChar(enterCmd)}
                    };
PCF8574KeyIn<5,0x21> myButton(myBtn_map);


int lufterTimeDelayOn=30;
int heizungTimeDelayOn=180;
int nvTimeDelayOn = 10;
int lufterTimeDelayOff=60;
int heizungTimeDelayOff=30;

int ledCtrl=LOW;

result myLedOn() {
  ledCtrl=HIGH;
  return proceed;
}
result myLedOff() {
  ledCtrl=LOW;
  return proceed;
}

MENU(subMenuOn, "TIME DELAY ON", Menu::doNothing, Menu::noEvent, Menu::wrapStyle
  ,FIELD(lufterTimeDelayOn,"Lufter ","s",30,300,10,1, myLedOn, exitEvent, Menu::noStyle)
  ,FIELD(heizungTimeDelayOn,"Heizung","s",180,300,10,1,myLedOff, exitEvent, Menu::noStyle)
  ,FIELD(nvTimeDelayOn,"HV     ","s",5,300,10,1,Menu::doNothing, Menu::noEvent, Menu::noStyle)
	,EXIT("Back")
);

MENU(subMenuOff, "TIME DELAY OFF", Menu::doNothing, Menu::noEvent, Menu::wrapStyle
  ,FIELD(lufterTimeDelayOff,"Lufter ","s",60,300,10,1, Menu::doNothing, Menu::noEvent, Menu::noStyle)
  ,FIELD(heizungTimeDelayOff,"Heizung","s",30,300,10,1,Menu::doNothing, Menu::noEvent, Menu::noStyle)
	,EXIT("Back")
);

MENU(mainMenu, "    DELAY SETUP", Menu::doNothing, Menu::noEvent, Menu::wrapStyle
	,SUBMENU(subMenuOn)
	,SUBMENU(subMenuOff)
  // ,FIELD(lufterTimeDelay,"Lufter ","s",30,60,10,1, Menu::doNothing, Menu::noEvent, Menu::noStyle)
  // ,FIELD(heizungTimeDelay,"Heizung","s",180,300,10,1,Menu::doNothing, Menu::noEvent, Menu::noStyle)
  // ,FIELD(nvTimeDelay,"HV     ","s",5,300,10,1,Menu::doNothing, Menu::noEvent, Menu::noStyle)
  //,OP("LED On",myLedOn,enterEvent)
  //,OP("LED Off",myLedOff,enterEvent)
  ,EXIT("Exit")
);

//serialIn serial(Serial);
//MENU_INPUTS(in,&serial);
MENU_INPUTS(in,&myButton);

MENU_OUTPUTS(out,MAX_DEPTH
  ,LCD_OUT(lcd,{0,0,20,4})
  ,NONE
);
// MENU_OUTPUTS(out,MAX_DEPTH
//   ,SERIAL_OUT(Serial)
//   ,NONE//must have 2 items at least
// );

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);

void setup() {
  pinMode(LEDPIN, OUTPUT);

	lcd.begin(20,4);
  lcd.setBacklight(255);
	nav.showTitle = true;
	nav.doNav(navCmd(downCmd));

  Serial.begin(115200);
  while(!Serial);
  Serial.println("Menu 4.x");
  Serial.println("Use keys + - * /");
  Serial.println("to control the menu navigation");
}

void loop() {
  nav.poll();
  digitalWrite(LEDPIN, ledCtrl);
}