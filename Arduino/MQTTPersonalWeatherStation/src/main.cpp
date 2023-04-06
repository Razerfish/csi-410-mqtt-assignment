/*
This has been adapted from the personal weather station
template on create.arduino.cc and from the code showcased
in class
*/

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Arduino_MKRIoTCarrier.h>

#include "config.h"

WiFiClient wioClient;
PubSubClient mqttClient(wioClient);

MKRIoTCarrier carrier;

unsigned long interval = INITIAL_INTERVAL;
int status = WL_IDLE_STATUS;

void initCarrier()
{
  Serial.println("Attempting to initialize carrier board...");

  CARRIER_CASE = false;
  if (!carrier.begin())
  {
    Serial.println("Unable to initialize carrier!");
    while (true)
      ;
  }
}

void reconnectMQTTClient()
{
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect(CLIENT_NAME.c_str()))
    {
      Serial.println("connected");
      mqttClient.subscribe(SUBSCRIBE_TOPIC.c_str());
    }
    else
    {
      Serial.print("Unable to connect to MQTT broker error code: ");
      Serial.println(mqttClient.state());

      delay(RETRY_INTERVAL);
    }
  }
}

void clientCallback(char *topic, uint8_t *payload, unsigned int length)
{
  char buff[length + 1];
  for (unsigned int i = 0; i < length; i++)
  {
    buff[i] = (char)payload[i];
  }
  buff[length] = '\0';

  Serial.print("Message received: ");
  Serial.println(buff);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, buff);
  JsonObject obj = doc.as<JsonObject>();

  float temperature = obj["temperature"];

  Serial.print("temperature: ");
  Serial.println(temperature);

  reconnectMQTTClient();
}

void createMQTTClient()
{
  mqttClient.setServer(BROKER.c_str(), BROKER_PORT);
  mqttClient.setCallback(clientCallback);
  reconnectMQTTClient();
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
    Serial.print("Attempting to connect to WiFi, SSID: ");
    Serial.println(SSID);

    status = WiFi.begin(SSID, PASSWORD);
    delay(RETRY_INTERVAL);
  }

  Serial.println("Connected!");
}

void publishTemp()
{
  reconnectMQTTClient();
  mqttClient.loop();

  // delay was here in case you encounter errors

  float temp = carrier.Env.readTemperature();

  DynamicJsonDocument doc(1024);
  doc["temperature"] = temp;

  string telemetry;
  serializeJson(doc, telemetry);

  Serial.print("Sending telemetry: ");
  Serial.println(telemetry.c_str());

  mqttClient.publish(PUBLISH_TOPIC.c_str(), telemetry.c_str());

  Serial.println("Telemetry sent!");
}

void setup()
{
  Serial.begin(9600);

  while (!Serial)
    ;

  delay(1000);
  Serial.println("Ready");

  connectToWiFi();
  createMQTTClient();
  initCarrier();
}

void loop()
{
  publishTemp();
  delay(interval);
}