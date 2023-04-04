/*
This has been adapted from the personal weather station
template on create.arduino.cc and from the code showcased
in class
*/

#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <Arduino_MKRIoTCarrier.h>

#include "config.h"

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
MKRIoTCarrier carrier;

unsigned long interval = INITIAL_INTERVAL;
int status = WL_IDLE_STATUS;

float temp = 0.0;

void setup()
{
  Serial.begin(9600);

  // Wait for Serial to be ready before printing anything but don't block if we're not connected.
  delay(1000);

  connectToWiFi();
  connectToMqttBroker();

  initCarrier();
}

void initCarrier()
{
  Serial.println("Attempting to initialize carrier board...");

  if (!carrier.begin())
  {
    Serial.println("Unable to initialize carrier!");
    while (true)
      ;
  }
}

void connectToMqttBroker()
{
  // This code adapted from here: https://docs.arduino.cc/tutorials/uno-wifi-rev2/uno-wifi-r2-mqtt-device-to-device
  Serial.print("Attempting to connect to MQTT broker: ");
  Serial.println(BROKER);

  if (!mqttClient.connect(BROKER, BROKER_PORT))
  {
    Serial.print("Unable to connect to broker! Error code: ");
    Serial.println(mqttClient.connectError());

    while (true)
      ;
  }

  Serial.println("Connected to MQTT broker!");
}

void connectToWiFi()
{
  // This code adapted from here: https://docs.arduino.cc/tutorials/communication/wifi-nina-examples
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Couldn't connect to WiFi module");
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade WiFi module firmware!");
  }

  // Try to connect to WiFi
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connected to WiFi SSID: ");
    Serial.println(SSID);

    status = WiFi.begin(SSID, PASSWORD);
    delay(WIFI_RETRY_INTERVAL);
  }

  Serial.println("Connected!");
}

void loop()
{
  mqttClient.poll();

  temp = carrier.Env.readTemperature();
}