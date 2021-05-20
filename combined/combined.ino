

#include <WiFi.h>
#include <PubSubClient.h>
#include "EmonLib.h"             // Include Emon Library

#define VOLT_CAL 148.7  
#define CURRENT_CAL 62.6


EnergyMonitor emon1;    

 
#define WIFISSID "wifi name" // Put your WifiSSID here
#define PASSWORD "wifi password" // Put your wifi password here
#define TOKEN "BBFF-*****************" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "ESP32_bldc_readings" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string;
 
/****************************************
  Define Constants
****************************************/
#define VARIABLE_LABEL1 "Current Readings" // Assing the variable label

#define DEVICE_LABEL "ESP32"
 
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[1000];
char topic1[150];

 
// Space to store values to send
char str_BLDC[10];

 
/****************************************
  Auxiliar Functions
****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);
 
 
void callback(char* topic, byte* payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.write(payload, length);
  Serial.println(topic);
}
 
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, ""))
    {
      Serial.println("Connected");
    } else
    {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}
 
/****************************************
  Main Functions
****************************************/
 
void setup()
{
  Serial.begin(115200);
  while (!Serial);
  
 


  
  WiFi.begin(WIFISSID, PASSWORD);
  Serial.println();
  Serial.print("Waiting for WiFi Connection ..............");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  WiFi.setHostname("esp32-current-monitor");
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);


  emon1.voltage(1, VOLT_CAL, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(0, CURRENT_CAL);       // Current: input pin, calibration.

}
 
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }

  // Set up oversampling and filter initialization
  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out

  float current= emon1.Irms;             //extract Irms into Variable

  dtostrf(current, 4, 2, str_BLDC);
  sprintf(topic1, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL1);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_BLDC);
  Serial.println("Publishing BLDC Data");
  client.publish(topic1, payload);
  Serial.println(current);
  Serial.println();
  client.loop();
  delay(2000);
  


  
}
 
