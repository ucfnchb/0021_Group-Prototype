#include <Servo.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

Servo myservo;  // Servo object to control a servo
Servo servo2;   // Second servo object
Servo servo3;   // Third servo object
Servo servo4;   // Fourth servo object, labeled as servo4

// WiFi credentials
const char* ssid = "CE-Hub-Student";
const char* password = "casa-ce-gagarin-public-service";

// MQTT Broker settings
const char* mqtt_server = "mqtt.cetools.org";
int mqtt_port = 1884;
const char* mqtt_user = "student";
const char* mqtt_password = "ce2021-mqtt-forget-whale";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  int speed = message.toInt();
  controlServo(speed);
}

void controlServo(int speed) {
  int pulseWidth = map(speed, 0, 75, 90, 180);
  myservo.write(pulseWidth);  // Control the first servo
  servo2.write(pulseWidth);   // Control the second servo similarly
  servo3.write(pulseWidth);   // Control the third servo similarly
  servo4.write(pulseWidth);   // Control the fourth servo similarly
}

void setup() {
  Serial.begin(9600);
  myservo.attach(11); // Attach the first servo on pin 11
  servo2.attach(10);  // Attach the second servo on pin 10
  servo3.attach(9);   // Attach the third servo on pin 9
  servo4.attach(8);   // Attach the fourth servo on pin 8
  
  setupWiFiAndMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setupWiFiAndMQTT() {
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

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("student/ucfnhy0/wind"); // Ensure this topic is correct for your data
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
