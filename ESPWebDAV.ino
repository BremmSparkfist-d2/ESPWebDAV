// Using the WebDAV server with Rigidbot 3D printer.
// Printer controller is a variation of Rambo running Marlin firmware

#include "config.h"
#include "network.h"
#include "sdControl.h"

// LED is connected to GPIO2 on this board
#define INIT_LED			{pinMode(2, OUTPUT);}
#define LED_ON				{digitalWrite(2, LOW);}
#define LED_OFF				{digitalWrite(2, HIGH);}

// ------------------------
void setup() {
	Serial.begin(115200);
	INIT_LED;
	blink();
	
	sdcontrol.setup();

	// ----- WIFI -------
  if(config.load() == 1) { // Connected before
    if(!network.start()) {
      Serial.println("Connect fail, please check your INI file or set the wifi config and connect again");
    }
  }
}

// ------------------------
void loop() {
  // handle the request
	network.handle();

  // blink
  statusBlink();
}

// ------------------------
void blink()	{
// ------------------------
	LED_ON; 
	delay(100); 
	LED_OFF; 
	delay(400);
}

// ------------------------
void errorBlink()	{
// ------------------------
	for(int i = 0; i < 100; i++)	{
		LED_ON; 
		delay(50); 
		LED_OFF; 
		delay(50);
	}
}

void statusBlink() {
  static unsigned long time = 0;
  if(millis() > time + 1000 ) {
    if(network.isConnecting()) {
      LED_OFF;
    }
    else if(network.isConnected()) {
      LED_ON; 
  		delay(50); 
  		LED_OFF; 
    }
    else {
      LED_ON;
    }
    time = millis();
  }

  // SPI bus not ready
	//if(millis() < spiBlockoutTime)
	//	blink();
}
