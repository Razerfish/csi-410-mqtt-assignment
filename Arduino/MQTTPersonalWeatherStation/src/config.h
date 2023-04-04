// This code has been adapted from the examples showed in class.
#pragma once

#include <String>

using namespace std;

const int WIFI_RETRY_INTERVAL = 5000;
const long INITIAL_INTERVAL = 1000;

// WiFi credentials
const char *SSID = "ChamplainPSK";
const char *PASSWORD = "letusdare";

// MQTT Settings
const String ID = "FVW";

const char BROKER[] = "broker.hivemq.com";
const uint16_t BROKER_PORT = 1883;
const String CLIENT_NAME = ID + "temperature_sensor";

const String PUBLISH_TOPIC = ID + "/temperature";
const String SUBSCRIBE_TOPIC = ID + "/interval";
