import json
from datetime import datetime

import paho.mqtt.client as mqtt

# Config variables
ID = "FVW"

BROKER = "broker.hivemq.com"
BROKER_PORT = 1883
CLIENT_NAME = ID + "temp_logger"
KEEPALIVE = 60

PUBLISH_TOPIC = ID + "/interval"
SUBSCRIBE_TOPIC = ID + "/temperature"

def on_connect(client, userdata, flags, rc):
    print("Connected with code: " + str(rc))
    client.subscribe(SUBSCRIBE_TOPIC)

def on_message(client, userdata, msg):
    #print(msg.topic + " " + str(msg.payload))

    data = json.loads(msg.payload.decode("utf-8"))

    with open("temp.txt", "a") as file:
        output = str(data["temperature"]) + ", " + str(datetime.fromtimestamp(data["time"]))
        print(output)
        file.write(output + "\n")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, BROKER_PORT, KEEPALIVE)

client.loop_forever()
