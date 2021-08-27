

char ssid[] = "Usine";     //  le nom du reseau WIFI
char password[] = "nainporte";  // le mot de passe WIFI


void wifiSetup(){
    // Conexion WIFI
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  
}