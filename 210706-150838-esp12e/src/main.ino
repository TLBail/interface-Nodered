#include <Arduino.h>
#include <stdint.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>  
#include <ArduinoJson.h>


//WiFi Connection configuration
char ssid[] = "Usine";     //  le nom du reseau WIFI
char password[] = "nainporte";  // le mot de passe WIFI
//mqtt server
char mqtt_server[] = "192.20.2.107";  //adresse IP serveur 
#define MQTT_USER ""
#define MQTT_PASS ""

WiFiClient espClient;
PubSubClient MQTTclient(espClient);


//RFID CODE
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D8
#define RST_PIN D0
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
// Init array that will store new NUID
byte nuidPICC[4];
//FIN RFID CODE



int stateIndex = 0;

int buzzerPin = 10;


int leftboutonPin = 0;
int rightboutonPin = 2;


#define NBMACHINE 5


enum Machine { CNC = 10,
    LASER = 11,
    TROISD = 12,
    BRODEUSE = 13,
    VYNIL = 14
};


Machine machines[] = { CNC,
    LASER,
    TROISD,
    BRODEUSE,
    VYNIL
};

String machineText[] = { 
    "la CNC",
    "l'imprimante laser",
    "l'imprimante 3D",
    "La brodeuse numérique ",
    "La découpe Vynil"
};


Machine selectedMachine[10];

struct MachineInfo
{
    Machine machineType;
    int value;
};


MachineInfo machinesInfo[NBMACHINE];



void setup() {
  Serial.begin(115200);
  screenSetup();

  drawstyles("Demarage");

  
  //setup button
  pinMode(leftboutonPin, INPUT);
  pinMode(rightboutonPin, INPUT);
  
  
  // Conexion WIFI
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  MQTTclient.setServer(mqtt_server, 1883);

  drawstyles("Connecte au mqtt \n passe votre carte pour vous identifie");

  //RFID SETUP
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  Serial.println();
  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();
  for (byte i = 0; i < 6; i++) {
   key.keyByte[i] = 0xFF;
  }
  Serial.println();
  Serial.println(F("This code scan the MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  
}

void loop() {
  static uint32_t  lastTimeMqtt= 0;
  // connect serveur MQTT
  if (!MQTTclient.connected()) {
    MQTTconnect();
  }

  
  //etape 1 on lit le rfid
  if(stateIndex == 0){
    drawstyles("Connecte au mqtt \n passe votre carte pour vous identifie");
    if(checkRfid()) stateIndex = 1;
  } else
  //etape 2 on demande de confirmé le code rfid appui court je confirme appui long je ne confirme pas
  if(stateIndex == 1){
    checkButton();  
  } else
  //etape 3 je montre les machine
  if(stateIndex == 2){  
    displayScreen();
    checkButton();
  }else

  //etape 4 je montre la machine confirmé je demande si je veux rajouter d'autre machine ou si je continu
  if(stateIndex == 3){
    checkButton();


  } else


  //etape 5 je demande l'autonomie du membre
  if(stateIndex == 4){
    checkButton();
    enterAutonomie();

  } else
  
  //etape 6 je demande pour chaque machine l'utilisation appui court j'incrémente appui long je passe a la machine suivante quand il y a plus de machine je passe à l'état suivant
  if(stateIndex == 5){
    enterMachineInfo();
    checkButton();
  } else
  
  //etape 7 je remercie l'utilisateur et le renvoie au début si il le souhaite
  if(stateIndex == 6){
    checkButton();
    drawstyles("merci !! appuis sur un bouton pour revenir au debut");
    

  } 



}


//les event


void onButtonPressDown(){

}


void onLeftButtonPressUp(){

  if(stateIndex == 1){
    stateIndex = 2;
  }else
  if(stateIndex == 2){
    selectMachine();
    stateIndex = 3;

    String sumOfSelectedMachine = "";
    
    int i = 0;
    while(selectedMachine[i] != 0){
      sumOfSelectedMachine += " " + machineToString(selectedMachine[i]);
      i++;
    }
    
    drawstyles(sumOfSelectedMachine + "voulez vous rajoutez une autre machine(bouton gauche)\nou termine la saisi(bouton droit) ");
  }else
  if(stateIndex == 3){
    stateIndex = 2;
  }else
  if(stateIndex == 4){
    incValueAutonomie();
  }else
  if(stateIndex == 5){
    incValueMachineInfo();
  }else
  if(stateIndex == 6){
    stateIndex = 0;
  }

}

void onRightButtonPressUp(){

  if(stateIndex == 1){
    stateIndex = 0;
    drawstyles("Connecte au mqtt \n passe votre carte pour vous identifie");

  }else
  if(stateIndex == 2){
    //decMachineScreenIndex();
    incMachineScreenIndex();

  }else
  if(stateIndex == 3){
    stateIndex = 4;
  }else
  if(stateIndex == 4){
    stateIndex++;
  }else
  if(stateIndex == 5){
    incMachineIndex();
  }else
  if(stateIndex == 6){
    stateIndex = 0;
  }

}




