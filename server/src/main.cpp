#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HardwareSerial.h>
#include <FS.h>
#include <SPIFFS.h>

HardwareSerial MegaSerial(2); // UART2 (RX2=16, TX2=17)
WebServer server(80);

// Identifiants du point d’accès
const char* ssid = "ESP32_AP";
const char* password = "12345678";

// Page HTML simple
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>Uploader HEX vers Arduino</title>
    <style>
      body { font-family: Arial; text-align: center; margin-top: 50px; }
      input[type=file] { margin: 20px; }
      button { padding: 10px 20px; font-size: 16px; }
    </style>
  </head>
  <body>
    <h2>Envoyer un fichier HEX vers Arduino</h2>
    <form method="POST" action="/upload" enctype="multipart/form-data">
      <input type="file" name="file" accept=".hex" required><br>
      <button type="submit">Envoyer</button>
    </form>
  </body>
</html>
)rawliteral";

// Gestion de la page principale
void handleRoot() {
  server.send(200, "text/html", index_html);
}

// Gestion du fichier uploadé
File uploadFile;

void handleFileUpload() {
  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("Upload start: %s\n", upload.filename.c_str());
    if (!SPIFFS.begin(true)) {
      Serial.println("Erreur SPIFFS");
      return;
    }
    uploadFile = SPIFFS.open("/" + upload.filename, FILE_WRITE);
  } 
  else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile)
      uploadFile.write(upload.buf, upload.currentSize);
  } 
  else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
      Serial.printf("Upload terminé : %s (%u octets)\n", upload.filename.c_str(), upload.totalSize);

      // Lecture et envoi UART
      File f = SPIFFS.open("/" + upload.filename, FILE_READ);
      if (f) {
        Serial.println("Envoi du fichier sur UART...");
        while (f.available()) {
          uint8_t b = f.read();
          MegaSerial.write(b);
        }
        f.close();
        Serial.println("Transfert terminé !");
      }
    }
    server.sendHeader("Location", "/");
    server.send(303);
  }
}

void setup() {
  Serial.begin(115200);
  MegaSerial.begin(115200, SERIAL_8N1, 16, 17); // UART vers Arduino Mega

  // Lancement du point d’accès
  WiFi.softAP(ssid, password);
  Serial.print("Point d’accès : ");
  Serial.println(ssid);
  Serial.print("IP : ");
  Serial.println(WiFi.softAPIP());

  // Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/upload", HTTP_POST, []() {
    server.send(200, "text/plain", "OK");
  }, handleFileUpload);

  server.begin();
  Serial.println("Serveur web démarré");
}

void loop() {
  server.handleClient();
}
