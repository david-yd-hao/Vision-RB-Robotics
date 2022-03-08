#/usr/local/sbin/mosquitto -c /usr/local/etc/mosquitto/mosquitto.conf
from time import sleep
import paho.mqtt.client as mqtt
def connect(client_name = "laptop",host_name = "127.0.0.1"):

    client =mqtt.Client(client_name)

    client.connect(host_name)

    return client
def send(client, data, topic):
    client.publish(topic, data)
def receive(client, topic):
    def on_message(client, userdata, msg):
        return msg.payload.decode(), msg.topic
    client.subscribe(topic)
    client.on_message = on_message
if __name__ == "__main__":
    client = connect(host_name="127.0.0.1")
    send(client,"Hello","arduino/")
    receive(client, "arduino/")
    