// Get ESP8266 going with Arduino IDE
// - https://github.com/esp8266/Arduino#installing-with-boards-manager
// Required libraries (sketch -> include library -> manage libraries)
// - PubSubClient by Nick ‘O Leary
// - DHT sensor library by Adafruit

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "Extremely Kappa(Only for gamers)"
#define wifi_password "SoloQueue<3"

#define mqtt_server "192.168.1.100"
#define mqtt_user "your_username"
#define mqtt_password "your_password"

#define waterSensorStatus_topic "Elementz/welcome"


WiFiClient espClient;

PubSubClient client(espClient);
//DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(12, INPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    if (client.connect("ESP8266Client")) {
    // if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


long lastMsg = 0;
int waterSensorStatus = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  waterSensorStatus = digitalRead(12); 
  Serial.print("WaterSensor Value: ");
  Serial.println(waterSensorStatus);
  client.publish(waterSensorStatus_topic, String(waterSensorStatus).c_str(), true);
 

  delay(1000)
}
