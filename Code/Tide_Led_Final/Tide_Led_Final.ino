#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// WiFi credentials
const char* ssid = "xxxxxx";
const char* password = "xxxxxx";

// MQTT Broker settings
const char* mqtt_server = "mqtt.cetools.org";
int mqtt_port = 1884;
const char* mqtt_user = "student";
const char* mqtt_password = "xxxxxx";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#define LED_PIN    7 // Adjust to the pin connected to your LED strip
#define NUMPIXELS 60  // Adjust to the number of pixels in your LED strip
Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

float lastTideData = -99999; // Use a clearly out-of-range initial value to ensure first comparison triggers change

void setup() {
  Serial.begin(9600);
  setupWiFiAndMQTT();
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

  float tideData = message.toFloat();

  // Only trigger the gradual color change if the tide data has changed
  if (tideData != lastTideData) {
    lastTideData = tideData;
    if (tideData > 0) {
      gradualChange(255, 220, 0, 0, 0, 255); // From yellow to blue
    } else {
      gradualChange(0, 0, 255, 255, 220, 0); // From blue to yellow
    }
  }
}

void setColor(int red, int green, int blue) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

void gradualChange(int startRed, int startGreen, int startBlue, int endRed, int endGreen, int endBlue) {
  int steps = 50; // Number of steps in the transition
  int duration = 5000; // Total duration of the change in milliseconds
  int delayTime = duration / steps; // Time to wait between updates

  for (int i = 0; i <= steps; i++) {
    float ratio = (float)i / (float)steps;
    int red = startRed + ratio * (endRed - startRed);
    int green = startGreen + ratio * (endGreen - startGreen);
    int blue = startBlue + ratio * (endBlue - startBlue);
    setColor(red, green, blue);
    delay(delayTime);
  }
}
