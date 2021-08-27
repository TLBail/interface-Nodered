#include <Arduino.h>
#include <stdint.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>  

//mqtt server
char mqtt_server[] = "192.20.2.108";  //adresse IP serveur 
#define MQTT_USER ""
#define MQTT_PASS ""

WiFiClient espClient;
PubSubClient MQTTclient(espClient);

#define SUCCES 1


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("starting ..");
  screenSetup();
  drawstyles("Demarage...");
  wifiSetup();
  mqttSetup();

  drawstyles("Connecte au mqtt \n passe votre carte \n pour vous identifie");

  rfidSetup();
  
  //buzzer
  pinMode(D4, OUTPUT);

}

void loop() {
  // connect serveur MQTT
  if (!MQTTclient.connected()) {
    MQTTconnect();
  }
  MQTTclient.loop();

  if(checkRfid() == SUCCES){
    tone(2, 1000);
    delay(200);
    noTone(2);
    delay(200);
    tone(2, 1000);
    delay(200);
    noTone(2);
    delay(200);
    MQTTclient.loop();
    delay(3000);
  }else{
    drawstyles("Connecte au mqtt \n passe votre carte \n pour vous identifie");  
    drawbitmap();
  }

}