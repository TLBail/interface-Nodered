void mqttSetup(){
  MQTTclient.setServer(mqtt_server, 1883);
  MQTTclient.setCallback(callback);
}

void MQTTconnect() {

  while (!MQTTclient.connected()) {
      Serial.print("Attente  MQTT connection...");
      String clientId = "TestClient-";
      clientId += String(random(0xffff), HEX);

    // test connexion
    if (MQTTclient.connect(clientId.c_str(),"","")) {
      Serial.println("connected");
      MQTTclient.subscribe("esp32/callback");


    } else {  // si echec affichage erreur
      Serial.print("ECHEC, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" nouvelle tentative dans 5 secondes");
      drawstyles("ECHEC nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.println("Message arrived in topic: ");
  Serial.print(topic);
 
  String msg = "";

  Serial.println("Message:");
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  
  }
 
  Serial.print(msg);
  
  if(msg.equals("CONNUE")){
    playBuzzer();
  }else{
    playDoom();
  }


}
 