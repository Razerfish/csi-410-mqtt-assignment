// This code has been adapted from the examples showed in class.
#pragma once

#include <string>

using namespace std;

const int RETRY_INTERVAL = 5000;
const long INITIAL_INTERVAL = 1000;

// WiFi credentials
const char *SSID = "ChamplainPSK";
const char *PASSWORD = "letusdare";

// MQTT Settings
const string ID = "FVW";

const string BROKER = "broker.hivemq.com";
const uint16_t BROKER_PORT = 1883;
const string CLIENT_NAME = ID + "temperature_sensor";

const string PUBLISH_TOPIC = ID + "/temperature";
const string SUBSCRIBE_TOPIC = ID + "/interval";
