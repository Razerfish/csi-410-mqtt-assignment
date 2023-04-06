#pragma once

#include <Arduino.h>
#include <string>
#include <PubSubClient.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include <Arduino_MKRIoTCarrier.h>
#include <RTCZero.h>

using namespace std;

// Wifi functions
void connectToWiFi(const char* SSID, const char* PASSWORD, WiFiClient& wioClient, unsigned int retry, int& status);

// Carrier board functions
void initCarrier(MKRIoTCarrier& carrier);

// MQTT Functions
void createMQTTClient(PubSubClient& client, const string& broker, const uint16_t brokerPort, void (*func)(char* topic, uint8_t* payload, unsigned int length));
void reconnectMQTTClient(PubSubClient& client, string clientName, string subscribeTopic, unsigned int retry);

// Time functions
void initRTC(RTCZero& rtc, unsigned int retry);
