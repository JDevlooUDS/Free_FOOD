#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <SPI.h>

// Pin RESET Mega
// SPI pins (hardware SPI)
#define SPI_MISO 19
#define SPI_MOSI 18
#define SPI_SCK  5
#define SPI_CS   4  // Non utilisé, mais nécessaire pour SPI.beginTransaction

//ArduinoSPI monSPI();


WebServer server(80);

// Wi-Fi credentials
const char* ssid = "ESP32_AP";
const char* password = "12345678";

File uploadFile;

// Page HTML principale
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Uploader HEX vers Mega 2560</title>
<style>
body { font-family: Arial; text-align: center; margin-top: 50px; }
input[type=file] { margin: 20px; }
button { padding: 10px 20px; font-size: 16px; border-radius: 8px; }
</style>
</head>
<body>
<h2>Uploader un fichier HEX vers Mega 2560</h2>
<form method="POST" action="/upload" enctype="multipart/form-data">
<input type="file" name="file" accept=".hex" required><br>
<button type="submit">Envoyer</button>
</form>
</body>
</html>
)rawliteral";

// ---- Fonctions SPI / STK500 simplifiées ----

// Reset Mega pour entrer en bootloader
void resetMega() {
  digitalWrite(SPI_CS, LOW);
  delay(50);
  digitalWrite(SPI_CS, HIGH);
  delay(50);
}

// Transfert simple byte par byte via SPI
uint8_t spiTransfer(uint8_t data) {
  return SPI.transfer(data);
}

// Simuler STK500: envoyer un octet, recevoir ack (très simplifié)
// Pour un flash réel, il faut gérer page write, cmd sync, checksum
bool stk500WriteByte(uint8_t byte) {
  uint8_t resp = spiTransfer(0xAC); // Commande STK500 fictive pour ack
  (void)resp;
  spiTransfer(byte);                // Envoyer octet
  return true;
}

// Transfert du fichier HEX via SPI / STK500
bool flashMega(File &f) {
  if (!f) return false;
  Serial.println("[SPI] Début transfert HEX vers Mega...");

  while (f.available()) {
    uint8_t b = f.read();
    if (!stk500WriteByte(b)) {
      Serial.println("[SPI] Erreur lors du transfert !");
      return false;
    }
  }
  Serial.println("[SPI] Transfert terminé !");
  return true;
}

// ---- Gestion serveur web ----

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("[UPLOAD] Début : %s\n", upload.filename.c_str());
    if (!SPIFFS.begin(true)) {
      Serial.println("[ERREUR] SPIFFS non monté !");
      return;
    }
    uploadFile = SPIFFS.open("/" + upload.filename, FILE_WRITE);
  } 
  else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
  } 
  else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
      Serial.printf("[UPLOAD] Terminé : %s (%u octets)\n", upload.filename.c_str(), upload.totalSize);

      resetMega();
      File f = SPIFFS.open("/" + upload.filename, FILE_READ);
      flashMega(f);
      f.close();

      server.sendHeader("Location", "/");
      server.send(303);
    }
  }
}

// ---- Wi-Fi events ----
void WiFiEventHandler(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
      Serial.printf("[WIFI] Client connecté (%d total)\n", WiFi.softAPgetStationNum());
      break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
      Serial.printf("[WIFI] Client déconnecté (%d restant)\n", WiFi.softAPgetStationNum());
      break;
    default:
      break;
  }
}

// ---- Setup / loop ----
void setup() {
  Serial.begin(9600);
  pinMode(SPI_CS, OUTPUT);
  digitalWrite(SPI_CS, HIGH);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);

  WiFi.onEvent(WiFiEventHandler);
  WiFi.softAP(ssid, password);
  Serial.printf("[AP] Point d’accès : %s\n", ssid);
  Serial.printf("[AP] IP : %s\n", WiFi.softAPIP().toString().c_str());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/upload", HTTP_POST, []() {
    server.send(200, "text/plain", "OK");
  }, handleUpload);

  server.begin();
  Serial.println("[HTTP] Serveur démarré !");
}

void loop() {
  server.handleClient();
}
