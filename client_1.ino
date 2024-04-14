#define led_1 5
#define led_2 13
#include <WiFiClient.h>
#include <PubSubClient.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
WiFiClient client;
PubSubClient mqtt_client(client);
const char *ssid = "HP Ultrasonic";
const char *password= "wwwwwwww";
const char *server_mqtt="192.168.0.132";  
const int port_mqtt=1883;
const char *mqtt_id="ttp2";
const char *topic_subscribe="to_esp8266";
const char *topic_publish="from_esp8266";
char rx_data[20]="";
String message_send="";
String p= "";
int j=0;


void callback(char *topic,byte *payload, unsigned int length)
{
  Serial.print("receive from: ");
  Serial.println(topic);
  Serial.print("messeage: ");
  for (size_t i=0; i<length; i++)
  {
     rx_data[i]=((char)payload[i]);
    // Serial.print((char)payload[i]);
  }
  p=String(rx_data);
  Serial.println(p);
  Serial.print("-------------------------------");
  Serial.println();
  memset(rx_data,0,sizeof(rx_data));
  // String message="";
  // Serial.print("receive form: ");
  // Serial.println(topic);
  // Serial.print("message: ");
  // for (size_t i=0; i< length; i++)
  // {
  //   message += (char)payload[i];
  // }
  // Serial.print(message);
  // Serial.println(".");
}
void setup() 
{
  Serial.begin(9600);
  pinMode(led_1,OUTPUT);
  pinMode(led_2,OUTPUT);
  Serial.println("connecting to wifi");
  WiFi.begin(ssid,password);
  // ket noi wifi
  while (WiFi.status()!=WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  } 
  Serial.println("connected to wifi");
  Serial.println("connecting to MQTT");
  mqtt_client.setServer(server_mqtt,port_mqtt);
  mqtt_client.setCallback(callback);
  // ket noi mqtt
  while (!mqtt_client.connect(mqtt_id))
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println("connected to MQTT");
  mqtt_client.publish(topic_publish,"hello Server");
  mqtt_client.subscribe(topic_subscribe);
}
void receive()
{
  if (Serial.available())
  {
    char c=(char)Serial.read();
    message_send +=c;
    if (c=='\n')
    {
      if (mqtt_client.connected())
      {
        mqtt_client.publish(topic_publish, message_send.c_str());
        message_send="";
        Serial.println(message_send);
      }
    }
  }
}
void loop() 
{
  mqtt_client.loop();
  receive();
  //if(strcmp((char*)rx_data,"on")==0)
  if (p=="on 1")
  {
    digitalWrite(led_1,HIGH);
    //digitalWrite(led_2,LOW);

  }
  if (p=="on 2")
  {
    digitalWrite(led_2,HIGH);
    //digitalWrite(led_1,LOW);

  }
  if (p=="off")
  {
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,LOW);
  }
  //Serial.println(p);
}
