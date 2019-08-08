import paho.mqtt.client as mqtt
import time

def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))

	client.subscribe("light/led1")

def on_message(client, userdata, msg):
	print(msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

broker_ip = "10.16.230.178"
client.username_pw_set("iotstudent", "coen")
client.connect(broker_ip, 1883, 60)
while True:


	client.reconnect()
	# Publish the message here! Use the following function
	client.publish("light/led1", payload="LIGHT ON")
