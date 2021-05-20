
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

 
#define WIFISSID "*******" // Put your WifiSSID here
#define PASSWORD "*********" // Put your wifi password here
#define TOKEN "BBFF-***********************" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "ESP32_bldc_readings" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string;
 
/****************************************
  Define Constants
****************************************/
#define VARIABLE_LABEL1 "Current Readings" // Assing the variable label 

#define DEVICE_LABEL "ESP32_bldc"
 
char mqttBroker[]  = "industrial.api.ubidots.com";//broker of messages
char payload[1000]; // telling the size of payoad
char topic1[150];// allocating the topic size

 
// Space to store values to send
char str_BLDC[10];

 
/****************************************
  Auxiliar Functions
****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);
 
  

void callback(char* topic, byte* payload, unsigned int length)
{
 
 /*
related to pubsubclient


Subscription Callback
If the client is used to subscribe to topics, a callback function must be provided in the constructor. 
This function is called when new messages arrive at the client.

The callback function has the following signature:

void callback(const char[] topic, byte* payload, unsigned int length)
Parameters
topic const char[] - the topic the message arrived on
payload byte[] - the message payload
length unsigned int - the length of the message payload
Internally, the client uses the same buffer for both inbound and outbound messages. After the callback function returns, 
or if a call to either publish or subscribe is made from within the callback function, the topic and payload values passed to 
the function will be overwritten. The application should create its own copy of the values if they are required after the callback 
returns.

*/
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


  //-----Wifi connection-----
  WiFi.begin(WIFISSID, PASSWORD);
  Serial.println();
  Serial.print("Waiting for WiFi Connection ..............");
 
 //check if wifi connected if not keep on trying 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
 //-------------------------
 
 //after wifi is connected
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //printing local wifi ip address
  client.setServer(mqttBroker, 1883);//this port is fixed acc. to the udibots docs 
  client.setCallback(callback); //calling the function callback for starting the publishing task
 //----------------
}
 
void loop()
{
 
 // checking for the mqtt connection if not then keep on trying 
  if (!client.connected())
  {
    reconnect();
  }
 //---------------------------
 
 //------sending dummy data in for loop------
  for (int i=0;i<15;i++){
  float current = i;
  dtostrf(current, 4, 2, str_BLDC);
  sprintf(topic1, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL1);
  sprintf(payload, "%s {\"value\": %s}}", payload, str_BLDC);
  Serial.println("Publishing BLDC Data");
  client.publish(topic1, payload);
  Serial.println();
  client.loop();
  delay(2000); //you change this 2 sec to 1sec(minimum can not cros beyond this) safeside 2sec
  }
 //------------------------------------------

  
}
 
