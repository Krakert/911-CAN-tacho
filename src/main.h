//
// Created by Stefan on 31/03/2024.
//

#ifndef INC_911_CAN_TACHO_MAIN_H
#define INC_911_CAN_TACHO_MAIN_H

#include <mcp2515.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// RPM limits
#define MAX_RPM             7000
#define MIN_RPM             1000
#define WIFI_RPM            9000

// Wifi setup
#define SSID                "SSID"
#define PWD                 "PWS"
#define HOST_NAME           "911_TACHO_OTA"

// Pin for tachometer
#define TACHO_OUTPUT_PIN    D1
#define SPI_CS_PIN          D8
#define MAX_PWM             255

// CAN message structure
struct can_frame canMsg;
MCP2515 CAN(SPI_CS_PIN);

// RPM and corresponding multiplier values
long rpms[] = {MIN_RPM, 2000, 3000, 4000, 5000, 6000, MAX_RPM};
double multipliers[] = {0.65, 0.855, 0.92, 0.9, 0.92, 0.93, 1.0};
size_t arraySize = sizeof(rpms) / sizeof(rpms[0]);


#endif //INC_911_CAN_TACHO_MAIN_H
