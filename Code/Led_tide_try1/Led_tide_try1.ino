
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

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

#define LED_PIN    6  // Adjust to the pin connected to your LED strip
#define NUMPIXELS 60  // Adjust to the number of pixels in your LED strip
Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

  // Initialize WiFi and MQTT
  setupWiFiAndMQTT();

  // Initialize the LED strip
  strip.begin();
  strip.show();  // Turn all LEDs off ASAP
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
      // Subscribe to the tide data topic
      client.subscribe("student/ucfnhy0/tide");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Assuming tide data is a simple positive/negative value
  float tideData = message.toFloat();

  // Set LED strip color based on tide data
  if (tideData > 0) {
    setColor(0, 0, 255);  // Blue color for positive tide data
  } else {
    setColor(255, 255, 0);  // Yellow color for negative tide data
  }
}

// Function to set the color of the entire LED strip
void setColor(int red, int green, int blue) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}