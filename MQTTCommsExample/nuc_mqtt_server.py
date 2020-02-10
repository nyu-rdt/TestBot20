import paho.mqtt.client as mqtt

# callback function gets executed when connection is made to server
def on_connect(client, userdata, flags, rc):
    print("connected")
    client.subscribe("demoTopicOut")

# callback function gets executed when message is received on any topic
def on_message(client, userdata, msg):
    print(msg.payload)

# create client, link callback functions to client, create MQTT server
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("localhost", 1883)

for i in range(0, 10):
    client.publish("demoTopicIn", " hello")

client.loop_forever()