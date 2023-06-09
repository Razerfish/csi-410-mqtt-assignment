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
#include "utils.h"

WiFiClient wioClient;
PubSubClient mqttClient(wioClient);

MKRIoTCarrier carrier;

RTCZero rtc;

unsigned long lastWebhook = 0; // The epoch time of when the last webhook was sent.
unsigned long interval = INITIAL_INTERVAL;
int status = WL_IDLE_STATUS;

void clientCallback(char *topic, uint8_t *payload, unsigned int length)
{
  Serial.println("Client callback");

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

  reconnectMQTTClient(mqttClient, CLIENT_NAME, SUBSCRIBE_TOPIC, RETRY_INTERVAL);
}

void publishTemp(float temp)
{
  reconnectMQTTClient(mqttClient, CLIENT_NAME, SUBSCRIBE_TOPIC, RETRY_INTERVAL);
  mqttClient.loop();

  // delay was here in case you encounter errors

  DynamicJsonDocument doc(1024);
  doc["temperature"] = temp;
  doc["time"] = rtc.getEpoch();

  string telemetry;
  serializeJson(doc, telemetry);

  Serial.print("Sending telemetry: ");
  Serial.print(telemetry.c_str());
  Serial.print("...");

  mqttClient.publish(PUBLISH_TOPIC.c_str(), telemetry.c_str());

  Serial.println("Sent!");
}

void setup()
{
  Serial.begin(9600);

  while (!Serial)
    ;

  delay(1000);
  Serial.println("Ready");

  initCarrier(carrier);
  connectToWiFi(SSID, PASSWORD, wioClient, RETRY_INTERVAL, status);
  initRTC(rtc, RETRY_INTERVAL);
  createMQTTClient(mqttClient, BROKER, BROKER_PORT, clientCallback);  
  reconnectMQTTClient(mqttClient, CLIENT_NAME, SUBSCRIBE_TOPIC, RETRY_INTERVAL);
}

static uint32_t last = 0;

void loop()
{
  if (millis() - last > interval)
  {
    float temp = readTemp(carrier);
    publishTemp(temp);
    last = millis();

    if (temp >= UPPER_LIMIT || temp <= LOWER_LIMIT)
    {
      if (rtc.getEpoch() >= lastWebhook + REPEAT_TIMEOUT * 60)
      {
        callWebhook(wioClient, temp, IFTTT_URL, IFTTT_EVENT, IFTTT_KEY, IFTTT_PORT);
        lastWebhook = rtc.getEpoch();
      }
    }
  }
}