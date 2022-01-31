#ifndef _FUNCTIONS_
#define _FUNCTIONS_

/* ############### INCLUDES ############### */
#include <platform_types.h>
#include "EEPROM.h"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#include <ESP8266WiFi.h>

/* Used for OTA */
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


/* ############## PROTOTYPES ############### */
void Function_Init();
void MainAppStateMachine();

extern struct_button BtnArray[];
extern struct_relay  RelayArray[];
extern struct_btnCodes btnCodesIR;
extern struct_btnCodes btnCodesIR_buffer;

extern void Btn_Cyclic();
extern void Relay_Cyclic();
#endif /* _FUNCTIONS_ */