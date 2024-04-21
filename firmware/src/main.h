//
// Created by Stefan on 31/03/2024.
//

#ifndef INC_911_CAN_TACHO_MAIN_H
#define INC_911_CAN_TACHO_MAIN_H

#include "mcp2515.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <blinker.h>
#include <tacho.h>

// CAN IDS
#define CAN_ID_RPM          0x201
#define CAN_ID_TEMP         0x203

// Wifi setup
#define SSID                "pvstsdkjdk"
#define PWD                 "KKCA6TBV8TA7"
#define HOST_NAME           "911_TACHO_OTA"

// Pin for SPI chip select
#define SPI_CS_PIN          D8

// CAN message structure
struct can_frame canMsg;
MCP2515 CAN(SPI_CS_PIN);

struct CanData {
    uint16_t rpm;
    int temp;
};

CanData canData;
Tacho tacho;

#endif //INC_911_CAN_TACHO_MAIN_H
