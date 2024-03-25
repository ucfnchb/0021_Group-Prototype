#include <Servo.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    13  // Replace with the data input pin of your LED strip
#define NUMPIXELS  60  // Replace with the number of pixels in your LED strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

Servo myservo;  // Servo object to control a servo

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

void setup() {
  Serial.begin(9600);
  myservo.attach(11); // Attach the servo on pin 11 to the servo object
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
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

  if (strcmp(topic, "student/ucfnhy0/tide") == 0) {
    float tideData = message.toFloat();
    if (tideData < 0) {
      setColor(255, 255, 0); // Yellow for negative tide data
    } else {
      setColor(0, 0, 255); // Blue for positive tide data
    }
  }
}

void setColor(int red, int green, int blue) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}
