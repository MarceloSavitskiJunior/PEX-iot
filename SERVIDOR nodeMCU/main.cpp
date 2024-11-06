#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
const int ledPin = LED_BUILTIN;

const char* ssid = "Oi";
const char* password = "oi242777";

const char* ota_password = "socorro";

void setupWiFi();
void setupWebServer();
void setupOTA();
void handleRoot();
void turnLedOn();
void turnLedOff();

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  setupWiFi();
  setupWebServer();
  setupOTA();

  Serial.println("Configuração concluída. Aguardando comandos...");
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void setupWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/led/on", HTTP_GET, turnLedOn);
  server.on("/led/off", HTTP_GET, turnLedOff);

  server.begin();
  Serial.println("Servidor web iniciado.");
}

void setupOTA() {
  ArduinoOTA.setPassword(ota_password);

  ArduinoOTA.onStart([]() {
    Serial.println("Iniciando atualização OTA...");
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nAtualização OTA concluída.");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progresso da OTA: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erro[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Erro de autenticação");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Erro no início da atualização");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Erro de conexão");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Erro de recebimento");
    else if (error == OTA_END_ERROR) Serial.println("Erro ao finalizar");
  });

  ArduinoOTA.begin();
  Serial.println("Pronto para atualizações OTA.");
}

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
  html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 {background-color: #77878A;}</style></head>";
  html += "<body><h1>ESP8266 Web Server</h1>";
  html += "<h2>Controle de LED</h2>";
  html += "<button onclick=\"toggleLED('off')\" class=\"button\">Ligar</button>";
  html += "<button onclick=\"toggleLED('on')\" class=\"button button2\">Desligar</button>";
  html += "<script>";
  html += "function toggleLED(state) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/led/' + state, true);";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void turnLedOn() {
  Serial.println("Ligando o LED...");
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "LED Ligado!");
}

void turnLedOff() {
  Serial.println("Desligando o LED...");
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "LED Desligado!");
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
}
