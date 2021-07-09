

// RFID FONCTION

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
 if (rfid.uid.uidByte[0] != nuidPICC[0] ||
     rfid.uid.uidByte[1] != nuidPICC[1] ||
     rfid.uid.uidByte[2] != nuidPICC[2] ||
     rfid.uid.uidByte[3] != nuidPICC[3] ) {
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


  drawstyles("bonjour " + (String)printHex(rfid.uid.uidByte, rfid.uid.size) + " \n confirmé votre identité(bouton gauche) ou infirmé(bouton droit)");
  valueToReturn = 1;
 }    
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