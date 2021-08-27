#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D8
#define RST_PIN D0
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
// Init array that will store new NUID
byte nuidPICC[4];




// RFID FONCTION

void rfidSetup(){
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


int checkRfid(){

    int valueToReturn = 0;
   // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
   if ( ! rfid.PICC_IsNewCardPresent())
   return 0;
 // Verify if the NUID has been readed
 if ( ! rfid.PICC_ReadCardSerial())
   return 0;
 Serial.print(F("PICC type: "));
 MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
 Serial.println(rfid.PICC_GetTypeName(piccType));
 // Check is the PICC of Classic MIFARE type
 if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
     piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
     piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
   Serial.println(F("Your tag is not of type MIFARE Classic."));
   return 0;
 }
  Serial.println(F("A new card has been detected."));
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  Serial.println(F("The NUID tag is:"));
  MQTTclient.publish("esp32/humidity", printHex(rfid.uid.uidByte, rfid.uid.size));
  Serial.println();
  Serial.print(F("In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();


  drawstyles("bonjour " + (String)printHex(rfid.uid.uidByte, rfid.uid.size) );
  valueToReturn = 1;
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  return valueToReturn;

}

char* printHex(byte *buffer, byte bufferSize) {
 char* out = new char[100];
 for (byte i = 0; i < bufferSize; i++) {
   //out[i] = buffer[i] < 0x10 ? '0' : ' ';
   out[i] = buffer[i];
 }
 return out;
}
/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
 for (byte i = 0; i < bufferSize; i++) {
   Serial.print(buffer[i] < 0x10 ? " 0" : " ");
   Serial.print(buffer[i], DEC);
 }
}