#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <HardwareSerial.h>
#include <LittleFS.h>

const char RESET_PIN = 12;

const char *ssid = "esp8266--test";
const char *password = "wasd1234";

ESP8266WebServer server(80);
File fsUploadFile;

enum program_state
{
  folder,
  encryption,
  send_bin
};

program_state state = folder;

// ---------- HTML PAGE ----------
const char uploadPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP8266 Folder Upload</title>
    <style>
      body { font-family: Arial; text-align: center; margin-top: 40px; }
      input[type=file] { margin: 10px; }
      .container { border: 1px solid #ccc; padding: 20px; border-radius: 8px; display: inline-block; }
      button { padding: 10px 20px; font-size: 16px; }
    </style>
  </head>
  <body>
    <div class="container">
      <h2>Upload Folder to ESP8266</h2>
      <form id="uploadForm" method="POST" action="/upload" enctype="multipart/form-data">
        <!-- Allow selecting multiple files -->
        <input type="file" id="fileInput" name="upload" webkitdirectory directory multiple>
        <br>
        <button type="submit">Upload Folder</button>
      </form>
      <p id="status"></p>

      <script>
        const form = document.getElementById('uploadForm');
        const status = document.getElementById('status');

        form.addEventListener('submit', async (e) => {
          e.preventDefault();
          const files = document.getElementById('fileInput').files;
          if (!files.length) {
            alert("Please select a folder or files first.");
            return;
          }

          status.textContent = "Uploading " + files.length + " files...";

          for (let i = 0; i < files.length; i++) {
            const file = files[i];
            const formData = new FormData();
            formData.append('upload', file, file.webkitRelativePath || file.name);

            await fetch('/upload', { method: 'POST', body: formData });
            status.textContent = `Uploaded ${i + 1} of ${files.length}: ${file.name}`;
          }

          status.textContent = "✅ All files uploaded successfully!";
        });
      </script>
    </div>
  </body>
</html>
)rawliteral";

// ---------- File Upload Handler ----------
void handleFileUpload() {
  HTTPUpload &upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) 
  {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;

    // Ensure directories exist in path (optional)
    int lastSlash = filename.lastIndexOf('/');
    if (lastSlash > 0) 
    {
      String dirPath = filename.substring(0, lastSlash);
      LittleFS.mkdir(dirPath);
    }

    fsUploadFile = LittleFS.open(filename, "w");
  } 
  else if (upload.status == UPLOAD_FILE_WRITE) 
  {
    if (fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
  } 
  else if (upload.status == UPLOAD_FILE_END) 
  {
    if (fsUploadFile) 
    {
      fsUploadFile.close();
      state = encryption;
    }
  }
}

// ---------- Binary transfer to atmega ----------
void enter_bootloader()
{
  digitalWrite(RESET_PIN, HIGH);
  delay(100);
  digitalWrite(RESET_PIN, LOW);
  delay(50);
} 

void send_txrx()
{
  string firmware_dir = "/code/firmware.bin";
  File firmware = LittleFS.open(firmware_dir, "r");

  size_t size = firmware.size();

  uint8_t *buffer = new uint8_t[size];
  firmware.read(buffer, size);
  firmware.close();

  for(int i = 0; i < size; i++)
  {
    Serial.write(buffer[i]);
  }

  delete[] buffer;
}


// ---------- Setup and Loop ----------
void setup() {
  Serial.begin(115200);

  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);

  LittleFS.begin();

  WiFi.softAP(ssid, password);

  // Routes
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", uploadPage);
  });

  server.on(
    "/upload", HTTP_POST,
    []() { server.send(200, "text/plain", "File uploaded successfully"); },
    handleFileUpload
  );

  server.on("/list", HTTP_GET, []() {
    String output = "Files on ESP8266:\n";
    Dir dir = LittleFS.openDir("/");
    while (dir.next()) {
      output += dir.fileName() + " (" + String(dir.fileSize()) + " bytes)\n";
    }
    server.send(200, "text/plain", output);
  });

  server.begin();
}

void loop() {
  switch (state) 
  {
    case folder :
      server.handleClient();
      break;

    case encryption :

      break;
    
    case send_bin :
      send_txrx();
      break;
  }
}
esp8266Server.ino
5 Ko