#include <Servo.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

Servo myservo;  // Servo object to control a servo

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

  // Convert payload to string
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Assuming message is a speed value between 0 and 75
  int speed = message.toInt();
  controlServo(speed);
}

void controlServo(int speed) {
  // Map speed (0 to 75) to PWM signal range for continuous rotation
  // Assuming 90 is stop; 0 is full speed one direction; 180 is full speed the opposite direction
  // This mapping is approximate and may need adjustment for your specific servo
  int pulseWidth = map(speed, 0, 75, 90, 180); // Adjust this range based on your servo's behavior
  myservo.write(pulseWidth);
}

void setup() {
  Serial.begin(9600);
  myservo.attach(11); // Attach the servo on pin 11 to the servo object
  
  // Initialize WiFi and MQTT
  setupWiFiAndMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setupWiFiAndMQTT() {
  // WiFi connection
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

  // MQTT connection
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect(); // Connect or reconnect to MQTT
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Subscribe to topics
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
