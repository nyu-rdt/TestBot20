#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "Team_16"
#define WIFI_PASS "lunabots"
#define WIFI_CHANNEL 1

#define SERVER_ADDR "192.168.1.4"
#define SERVER_PORT 1883
#define TOPIC_NAME_OUT "demoTopicOut"
#define TOPIC_NAME_IN "demoTopicIn"
#define MQTT_RECONNECT_TIMEOUT 200
#define MQTT_READ_TIMEOUT 50

// Create MQTT client on the ESP
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, SERVER_ADDR, SERVER_PORT, "demoTopic", "");

Adafruit_MQTT_Publish outTopic = Adafruit_MQTT_Publish(&mqtt, TOPIC_NAME_OUT);
Adafruit_MQTT_Subscribe inTopic = Adafruit_MQTT_Subscribe(&mqtt, TOPIC_NAME_IN);


void checkConnection();
void publishTest();
void subscribeTest();

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
 
  WiFi.begin(WIFI_SSID, WIFI_PASS, WIFI_CHANNEL);

  //  Wait until ESP is connected to wifi to proceed
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");

  mqtt.subscribe(&inTopic);
}

void loop() {
  // Constantly check the connection and reconnect if it is dropped
  checkConnection();

  publishTest();
  subscribeTest();
}

 
void checkConnection(){
  if (mqtt.connected()) return; 
  
  int8_t connectionStatus;
  connectionStatus = mqtt.connect();
  while (connectionStatus != 0){
    mqtt.disconnect();
    delay(MQTT_RECONNECT_TIMEOUT);
    connectionStatus = mqtt.connect();
  }
}

void publishTest() {
  mqtt.publish("demoTopicOut", "hi");
}

void subscribeTest() {
  Adafruit_MQTT_Subscribe* subPtr;
  while ((subPtr = mqtt.readSubscription(MQTT_READ_TIMEOUT))){
    if (subPtr == &inTopic){
      Serial.println((char*) inTopic.lastread);
    }
  }
}
