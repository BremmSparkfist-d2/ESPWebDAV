#include "config.h"
#include "network.h"
#include "sdControl.h"
#include <SPI.h>
#include <SdFat.h>
#include "pins.h"
#include <ArduinoHA.h>
#include <ESP8266WiFi.h>

// LED is connected to GPIO2 on this board
#define INIT_LED			{pinMode(2, OUTPUT);}
#define LED_ON				{digitalWrite(2, LOW);}
#define LED_OFF				{digitalWrite(2, HIGH);}

const char* mqtt_server = "ha.nebulae.nl";
const char* mqtt_user = "mqttuser";
const char* mqtt_password = "mqttpassword";
const byte mac[] = {0x01, 0x11, 0xFA, 0x6E, 0x38, 0x4A};
const int mqtt_Port = 1883;

WiFiClient espClient;
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(espClient, device);

HASensor alpha2_data("alpha2_data");
HASensor alpha2_state("alpha2_state");

// ------------------------
void setup() {
  Serial.begin(115200);
  INIT_LED;
  blink();

  sdcontrol.setup();

  // ----- WIFI -------
  if (config.load() == 1) { // Connected before
    if (!network.start()) {
      Serial.println("Connect fail, please check your INI file or set the wifi config and connect again");
    }
  }

  mqtt.begin(mqtt_server, mqtt_user, mqtt_password);

  alpha2_data.setName("Alpha 2 Data");
  alpha2_data.setUnitOfMeasurement("txt");
  alpha2_state.setName("Alpha 2 State");
  alpha2_state.setUnitOfMeasurement("txt");

}

// ------------------------
void loop() {
  statusBlink();

  readFile();

  mqtt.loop();

  delay(5000);
}

void readFile() {

  SdFat sdfat;

  if (!sdcontrol.canWeTakeBus()) {
    Serial.println("Not able to take bus control");
    alpha2_state.setValue("Not able to take bus control");
    return;
  }

  sdcontrol.takeBusControl();

  if (!sdfat.begin(SD_CS, SPI_FULL_SPEED)) {
    Serial.println("Initial SD failed");
    alpha2_state.setValue("Initial SD failed");
    sdcontrol.relinquishBusControl();
    return;
  }

  File file = sdfat.open("Logs/log.txt", FILE_READ);
  if (!file) {
    Serial.println("Open INI file failed");
    alpha2_state.setValue("Open INI file failed");
    
    sdcontrol.relinquishBusControl();
    return;
  }

  String buffer;
  while (file.available()) { // check for EOF
    buffer = file.readStringUntil('\r');
    if (buffer.length() == 0) continue; // Empty line
    
    Serial.println(buffer);
    alpha2_data.setValue(buffer.c_str());
  }

  file.close();
  sdcontrol.relinquishBusControl();

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
  for (int i = 0; i < 100; i++)	{
    LED_ON;
    delay(50);
    LED_OFF;
    delay(50);
  }
}

void statusBlink() {
  static unsigned long time = 0;
  if (millis() > time + 1000 ) {
    if (network.isConnecting()) {
      LED_OFF;
    }
    else if (network.isConnected()) {
      LED_ON;
      delay(50);
      LED_OFF;
    }
    else {
      LED_ON;
    }
    time = millis();
  }
}
