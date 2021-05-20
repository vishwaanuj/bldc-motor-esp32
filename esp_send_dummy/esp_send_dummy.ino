

#include <WiFi.h>
#include <PubSubClient.h>
#include "EmonLib.h"             // Include Emon Library

#define VOLT_CAL 148.7  
#define CURRENT_CAL 62.6


EnergyMonitor emon1;    

 
#define WIFISSID "*********" // Put your WifiSSID here
#define PASSWORD "*********" // Put your wifi password here
#define TOKEN "BBFF-************** // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "ESP32_bldc_readings" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string;
 
/****************************************
  Define Constants
****************************************/
#define VARIABLE_LABEL1 "Current Readings" // Acesssing the variable label
#define VARIABLE_LABEL2 "Current Gauge" // Ascessing the variable label
#define VARIABLE_LABEL_SUBSCRIBE "Switch" // Ascessing the variable label
#define DEVICE_LABEL "ESP32"
 
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[1000];
char topic1[150];
char topic2[150];
char topicSubscribe[100];
 
// Space to store values to send
char str_BLDC[10];
char str_Gauge[10];
 
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

  if (message == "0.0") {
  Serial.println("switch off");
}
   else {
    Serial.println("switch on");
}
  
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
       client.subscribe(topicSubscribe);
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

 
  // Set up oversampling and filter initialization

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
  sprintf(topicSubscribe, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE);
  client.subscribe(topicSubscribe);
}
 
void loop()
{
  if (!client.connected())
  {
    client.subscribe(topicSubscribe);   
    reconnect();
  }
  for (int i=0;i<10;i++){
  float current = i;

  //All the data calculation needs to be done here before publishing
  dtostrf(current, 4, 2, str_BLDC);
  dtostrf(current*10, 4, 2, str_Gauge);
  //--------------------------------------
  
  sprintf(topic1, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL1);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_BLDC);

  client.publish(topic1, payload);

  sprintf(topic2, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL2);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_Gauge);

  client.publish(topic2, payload);

  client.loop();
  delay(2000);
  }

  
}
 
