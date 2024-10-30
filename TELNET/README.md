### 1. Importação de Bibliotecas

``` c++
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
```

### 2. Configuração de rede e Pino de saída
``` c++
const char* ssid = "ssid";
const char* password = "password";
const int LED_PIN = 5; // D1
```

- ssid: Nome da rede WiFi.
- password: Senha da rede WiFi.
- LED_PIN: Define o pino D1 (GPIO5) para o controle do LED.

### 3. Configuração do servidor TelNet
``` c++
WiFiServer server(23); // Servidor Telnet na porta 23
WiFiClient client;
```

- server(23): Cria um servidor Telnet na porta 23, padrão para comunicações Telnet.
- client: Instância de cliente WiFi que será usada para conexões remotas via Telnet.

### 4. Configuração da Rede Wifi
```c++
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Conexão falhou, tentando novamente...");
    WiFi.begin(ssid, password);
    delay(5000);
  }
```

- pinMode(LED_PIN, OUTPUT): Define o pino LED_PIN como saída.
- Serial.begin(9600): Inicia a comunicação serial para exibir mensagens no console.
- WiFi.mode(WIFI_STA): Define o ESP8266 para o modo estação (STA).
- WiFi.begin(ssid, password): Tenta conectar à rede WiFi.
- while (WiFi.waitForConnectResult() != WL_CONNECTED): Loop que verifica a conexão WiFi até que seja bem-sucedida.

### 5. Configuração do processo OTA
``` c++
  ArduinoOTA.onStart([]() {
    Serial.println("Iniciando atualização OTA...");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nAtualização concluída.");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erro[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Falha na autenticação");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Falha no início");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Falha na conexão");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Falha no recebimento");
    else if (error == OTA_END_ERROR) Serial.println("Falha na finalização");
  });
  ArduinoOTA.begin();
  Serial.println("Pronto para atualizações OTA.");
```

- Funções de Callbacks OTA:
  - onStart(): Exibe mensagem quando a atualização OTA inicia.
  - onEnd(): Exibe mensagem quando a atualização OTA termina.
  - onProgress(): Mostra o progresso da atualização OTA em porcentagem.
  - onError(): Exibe o erro específico que ocorre durante a atualização OTA.
- ArduinoOTA.begin(): Inicializa o serviço OTA, permitindo futuras atualizações OTA.

### 6. Inicialização do Servidor Telnet
``` c++
  server.begin();
  server.setNoDelay(true);
  Serial.println("Servidor Telnet iniciado...");
}
```
- server.begin(): Inicia o servidor Telnet.
- server.setNoDelay(true): Configura para que as mensagens Telnet sejam enviadas imediatamente, sem buffer.

### 7. Loop Principal e Conexão do Cliente Telnet
``` c++
void loop() {
  ArduinoOTA.handle();
  
  if (server.hasClient()) {
    if (!client || !client.connected()) {
      if (client) client.stop();
      client = server.available();
      Serial.println("Cliente conectado via Telnet.");
      client.println("Digite '1' para ligar o LED e '0' para desligar o LED.");
    } else {
      server.available().stop();
    }
  }
```

- ArduinoOTA.handle(): Verifica continuamente por novas atualizações OTA.
- if (server.hasClient()): Verifica se há um novo cliente tentando se conectar.
  - Se o cliente atual não estiver conectado, interrompe a conexão (client.stop()).
  - Conecta-se ao novo cliente (client = server.available()).
  - Envia uma mensagem ao cliente informando os comandos para controle do LED.
- else: Fecha novas conexões adicionais, mantendo apenas uma conexão Telnet ativa.

### 8. Leitura e Execução de Comandos
``` c++
  if (client && client.connected() && client.available()) {
    char command = client.read();
    if (command == '1') {
      digitalWrite(LED_PIN, LOW); 
      client.println("LED ligado.");
      Serial.println("LED ligado.");
    } else if (command == '0') {
      digitalWrite(LED_PIN, HIGH); 
      client.println("LED desligado.");
      Serial.println("LED desligado.");
    } else {
      client.println("Comando inválido. Digite '1' para ligar o LED e '0' para desligar o LED.");
    }
  }
}
```

- if (client && client.connected() && client.available()): Verifica se o cliente está conectado e enviou dados.
- char command = client.read(): Lê o comando do cliente Telnet.
  - Comandos:
    - '1': Liga o LED (digitalWrite(LED_PIN, LOW);).
    - '0': Desliga o LED (digitalWrite(LED_PIN, HIGH);).
    - Comando inválido: Se o comando não for '1' ou '0', responde com uma mensagem de erro ao cliente.