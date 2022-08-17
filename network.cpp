#include "network.h"
#include "config.h"
#include "pins.h"
#include "ESP8266WiFi.h"
#include "sdControl.h"

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}

bool Network::start() {
  wifiConnected = false;
  wifiConnecting = true;
  
  // Set hostname first
  WiFi.hostname(HOSTNAME);
  // Reduce startup surge current
  WiFi.setAutoConnect(false);
  WiFi.mode(WIFI_STA);
  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  WiFi.begin(config.ssid(), config.password());

  // Wait for connection
  unsigned int timeout = 0;
  while(WiFi.status() != WL_CONNECTED) {
    //blink();
    Serial.print(".");
    timeout++;
    if(timeout++ > WIFI_CONNECT_TIMEOUT/100) {
      Serial.println("");
      wifiConnecting = false;
      return false;
    }
    else
      delay(100);
  }

  
  Serial.print("Connected to ");
  Serial.println(config.ssid());
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.print("RSSI: "); Serial.println(WiFi.RSSI());
  Serial.print("Mode: "); Serial.println(WiFi.getPhyMode());
//  Serial.print("Asscess to SD at the Run prompt : \\\\"); Serial.print(WiFi.localIP());Serial.println("\\DavWWWRoot");

  wifiConnected = true;

  config.save();
  String sIp = IpAddress2String(WiFi.localIP());

  //Serial.println("Going to start DAV server");
  //if(startDAVServer() < 0) return false;
  wifiConnecting = false;

  return true;
}

//int Network::startDAVServer() {
//  if(!sdcontrol.canWeTakeBus()) {
//    return -1;
//  }
//  sdcontrol.takeBusControl();
//  
//  // start the SD DAV server
//  if(!dav.init(SD_CS, SPI_FULL_SPEED, SERVER_PORT))   {
//    DBG_PRINT("ERROR: "); DBG_PRINTLN("Failed to initialize SD Card");
//    // indicate error on LED
//    //errorBlink();
//    initFailed = true;
//  }
//  else {
//    //blink();
//  }
//  
  //sdcontrol.relinquishBusControl();
//  DBG_PRINTLN("FYSETC WebDAV server started");
//  return 0;
//}

bool Network::isConnected() {
  return wifiConnected;
}

bool Network::isConnecting() {
  return wifiConnecting;
}

// a client is waiting and FS is ready and other SPI master is not using the bus
bool Network::ready() {
  if(!isConnected()) return false;
  
//  // do it only if there is a need to read FS
//	if(!dav.isClientWaiting())	return false;
//	
//	if(initFailed) {
//	  dav.rejectClient("Failed to initialize SD Card");
//	  return false;
//	}
//	
//	// has other master been using the bus in last few seconds
//	if(!sdcontrol.canWeTakeBus()) {
//		dav.rejectClient("Marlin is reading from SD card");
//		return false;
//	}

	return true;
}

void Network::handle() {
//  if(network.ready()) {
//	  sdcontrol.takeBusControl();
//	  dav.handleClient();
//	  sdcontrol.relinquishBusControl();
//	}
}

Network network;
