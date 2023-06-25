#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "secret.h"

#define POWER_PIN  17
#define SIGNAL_PIN 36
#define sensor_pin 39

#define MQTT_SERVER "1.tcp.eu.ngrok.io"
#define MQTT_PORT 21589

int _moisture, sensor_analog, value;
int pump, level;

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Zielarz", MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.print("connected as ");
      Serial.println(MQTT_USERNAME);
    } else {
      Serial.print("failed (");
      Serial.print(client.state());
      Serial.println(") trying again in 3 seconds ...");
      delay(3000);
    }
  }
}

void requestMoistureChange()
{
  // publish state
  StaticJsonDocument<244> doc;
  char buffer[244];
  doc["idx"] = 12;
  doc["nvalue"] = 0; 
  doc["svalue"] = String(_moisture);
  serializeJson(doc, buffer);
  client.publish("domoticz/in", buffer);
  Serial.print("send: ");
  Serial.println(buffer);
}

void requestWaterAlert()
{
    // publish state
  StaticJsonDocument<244> doc;
  doc["nvalue"] = level;
  doc["idx"] = 13;
  char buffer[244];
  serializeJson(doc, buffer);
  client.publish("domoticz/in", buffer);
  Serial.print("published: ");
  Serial.println(buffer);
}

void requestPumpStatusChange()
{
    // publish state
  StaticJsonDocument<244> doc;
  doc["nvalue"] = pump;
  doc["idx"] = 11;
  char buffer[244];
  serializeJson(doc, buffer);
  client.publish("domoticz/in", buffer);
  Serial.print("published: ");
  Serial.println(buffer);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(POWER_PIN, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW);
  delay(1000);
  Serial.print("Connecting to ");
  Serial.print(SSID);
  Serial.print(" ..");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected to ");
  Serial.print(SSID);
  Serial.print(" as ");
  Serial.println(WiFi.localIP());

  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  // put your main code here, to run repeatedly:
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  if(value<1000)
  {
    level = 10;
  } 
  else if (value<1500)
  {
    level = 2;
  }
  else
  {
    level = 1;
  }
  Serial.print("The water sensor value: ");
  Serial.println(value);
  sensor_analog = analogRead(sensor_pin);
  _moisture = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
  Serial.print("Moisture = ");
  Serial.print(_moisture);  /* Print Temperature on the serial window */
  Serial.println("%");

  requestMoistureChange();
  requestWaterAlert();

  if(_moisture<40 && value>1000)
  {
    pump = 1;
    requestPumpStatusChange();
    Serial.println("Podlewanie");
    delay(10000);
    pump = 0;
    requestPumpStatusChange();
  } 
  else if (_moisture>40)
  {
    Serial.println("Nie trzeba podlewać");
  }
  else if (value<1000)
  {
    Serial.println("Za mało wody");
  }

  delay(1000);
}
