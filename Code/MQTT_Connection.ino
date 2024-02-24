#include <WiFiNINA.h>
#include <PubSubClient.h>

//connection to wifi
const char* ssid = "TP-Link_1180";
const char* password = "03331170";

// MQTT Broker setting 
const char* mqtt_server = "mqtt.cetools.org"; 
int mqtt_port = 1884;
const char* mqtt_user = "student";          
const char* mqtt_password = "ce2021-mqtt-forget-whale";    

WiFiClient wifiClient;
PubSubClient client(wifiClient);


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

// connection to internet 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

 // connect to MQTT
  if (!client.connect("arduinoClient", mqtt_user, mqtt_password)) {
    Serial.println("Initial connection to MQTT Broker failed, entering reconnect...");
    reconnect(); 
  } else {
    Serial.println("Connected to MQTT Broker!");
    // subscribe tide and wind 
    client.subscribe("student/ucfnhy0/tide");
    client.subscribe("student/ucfnhy0/wind");
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
 // connect 
    if (client.connect("arduinoClient", mqtt_user, mqtt_password)) {
      Serial.println("connected");
     // subscribe 
      client.subscribe("student/ucfnhy0/tide");
      client.subscribe("student/ucfnhy0/wind");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      delay(5000);
    }
  }
}
