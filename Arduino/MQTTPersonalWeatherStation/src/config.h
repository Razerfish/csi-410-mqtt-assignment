// This code has been adapted from the examples showed in class.
#pragma once

#include <string>

using namespace std;

const unsigned int RETRY_INTERVAL = 1000;
const unsigned int INITIAL_INTERVAL = 1000;

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

// Webhook trigger config
float UPPER_LIMIT = 37.78;
float LOWER_LIMIT = 0.0;
int REPEAT_TIMEOUT = 5; // In minutes

// IFTTT Config
const string IFTTT_URL = "maker.ifttt.com";
const int IFTTT_PORT = 80;
const string WEBHOOK_ENDPOINT = "/trigger/temp_outside_bounds/json/with/key/sMih99Vq-ytSzpjDifoNg";
