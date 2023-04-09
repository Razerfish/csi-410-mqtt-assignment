#include "utils.h"

void connectToWiFi(const char* SSID, const char* PASSWORD, WiFiClient& wioClient, unsigned int retry, int& status)
{
      // This code adapted from here: https://docs.arduino.cc/tutorials/communication/wifi-nina-examples
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Couldn't connect to WiFi module!");
        while (true)
          ;
    }

    if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION)
    {
        Serial.println("Please upgrade WiFi module firmware!");
    }

    // Try to connect to WiFi
    Serial.print("Attempting to connect to WiFi, SSID: ");
    Serial.println(SSID);

    status = WiFi.begin(SSID, PASSWORD);
    while (status != WL_CONNECTED)
    {
        Serial.print("Unable to connect to WiFi, trying again in ");
        Serial.print(retry / 1000);
        Serial.println(" seconds.");
        delay(retry);

        Serial.print("Attempting to connect to WiFi, SSID: ");
        Serial.println(SSID);
        status = WiFi.begin(SSID, PASSWORD);
    }

    Serial.println("Connected!\n");
}

void initCarrier(MKRIoTCarrier& carrier)
{
    Serial.println("Attempting to initialize carrier board...");

    CARRIER_CASE = false;
    if (!carrier.begin())
    {
        Serial.println("Unable to initialize carrier!");
        while (true)
          ;
    }
    
    Serial.println("Success!\n");
}

void createMQTTClient(PubSubClient& client, const string& broker, const uint16_t brokerPort, void (*callback)(char* topic, uint8_t* payload, unsigned int length))
{
    client.setServer(broker.c_str(), brokerPort);
    client.setCallback(callback);
}

void reconnectMQTTClient(PubSubClient& client, string clientName, string subscribeTopic, unsigned int retry)
{
    while (!client.connected())
    {
        Serial.println("Attempting MQTT connection...");

        if (client.connect(clientName.c_str()))
        {
            Serial.println("Connected!\n");
            client.subscribe(subscribeTopic.c_str());
        }
        else
        {
            Serial.print("Unable to connect to MQTT broker, error: ");
            Serial.println(client.state());

            delay(retry);
        }
    }
}

void initRTC(RTCZero& rtc, unsigned int retry)
{
    // The code for this adapted from here: http://man.hubwiz.com/docset/Arduino.docset/Contents/Resources/Documents/www.arduino.cc/en/Tutorial/WiFiRTC.html
    unsigned long epoch = 0;

    while (epoch == 0)
    {
        Serial.println("Fetching current time from server...");
        epoch = WiFi.getTime();

        if (epoch == 0)
        {
            Serial.println("Unable to fetch current time");
            Serial.print("Trying again in ");
            Serial.print(retry / 1000);
            Serial.println(" seconds.\n");
            delay(retry);
        }
    }

    Serial.println("Current time received!\n");

    rtc.begin();
    rtc.setEpoch(epoch);
}
