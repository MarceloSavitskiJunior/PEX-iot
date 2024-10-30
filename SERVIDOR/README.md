## Explicação do código do arquivo main.cpp
---

### 1. Importação de Bibliotecas

``` c++
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
```

###### ***ESP8266WiFi.h***: Biblioteca para conexão do ESP8266 a uma rede WiFi.
###### ***ESP8266WebServer.h***: Biblioteca para configurar um servidor web no ESP8266.
 

### 2. Configuração de rede WiFi

``` c++
const char* ssid     = "trombadepato";
const char* password = "jojobizarro";
```
Simplesmente atribui valores de id da rede e senha da rede. Nesse caso, a rede usada foi roteada de um celular.


### 3. Instância do Servidor
``` c++
ESP8266WebServer server(80);
```

### 4. Definição de pino de saída
``` c++
const int output5 = 5;  // D1
```
Pino de saída para conectar o jumper no ***nodemcu***

### 5. Configuração do IP estático
``` c++
IPAddress local_IP(192,168,110,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
```

***local_IP***: IP fixo que o ESP8266 usará na rede.

***gateway***: IP do gateway de rede (geralmente o roteador).

***subnet***: Máscara de sub-rede da rede local.

### 6. Declaração das funções de Manipulação de Rotas
``` c++
void handleRoot();
void handleOn();
void handleOff();
```

- ***handleRoot()***: Serve a página HTML principal.
- ***handleOn()***: Lida com a requisição para ligar a lâmpada.
- ***handleOff()***: Lida com a requisição para desligar a lâmpada.

### 7. Função para servir a página HTML

``` c++
void handleRoot() {
  String html = R"html(
    <!DOCTYPE html>
    <html>
      <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="icon" href="data:,">
        <style>
          html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }
          .button { background-color: #195B6A; border: none; color: white; padding: 16px 40px; 
                    text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; }
          .button2 { background-color: #77878A; }
        </style>
      </head>
      <body>
        <h1>ESP8266 Web Server</h1>
        <h2>LIGADOR DE LÂMPADA</h2>
        <button onclick="toggleLED('on')" class="button">ON</button>
        <button onclick="toggleLED('off')" class="button button2">OFF</button>
        <script>
          function toggleLED(state) {
            var xhr = new XMLHttpRequest();
            xhr.open('GET', '/toggle/' + state, true);
            xhr.send();
          }
        </script>
      </body>
    </html>
  )html";
  
  server.send(200, "text/html", html);
}
```

- Esta função cria uma string HTML completa e a envia ao cliente.
- HTML inclui:
  - Estilos: Botões estilizados para o controle de ON e OFF.
  - JavaScript: Função toggleLED() envia uma requisição AJAX ao servidor para ligar ou desligar o LED.
  - server.send(200, "text/html", html);: Envia a página HTML com o código de status 200 (OK).


### 8. Função para ligar o LED
``` c++
void handleOn() {
  digitalWrite(output5, HIGH);
  server.send(200, "text/plain", "LED Ligado!");
}
```
- Define o pino output5 para HIGH (lâmpada ligada).
- Retorna uma resposta de texto "LED Ligado!" para o cliente.

### 9. Função para desligar o LED
``` c++
void handleOff() {
  digitalWrite(output5, LOW);
  server.send(200, "text/plain", "LED Desligado!");
}
```
- Define o pino output5 para LOW (lâmpada desligada).
- Retorna uma resposta de texto "LED Desligado!" para o cliente.

### 10. Função para Configuração Inicial
``` c++

void setup() {
  Serial.begin(9600);
  pinMode(output5, OUTPUT);
  digitalWrite(output5, LOW);
 
  
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  
  server.on("/", HTTP_GET, handleRoot);

  server.on("/5/on", HTTP_GET, handleOn);

  server.on("/5/off", HTTP_GET, handleOff);

  server.begin();
}
```

- Configurações Iniciais:
   - Inicia a comunicação serial.
   - Define o pino output5 como saída e inicia em LOW (desligado).
- Configuração do WiFi:
  - Configura o IP estático e conecta à rede WiFi com o ssid e password definidos.
  - Imprime o IP ao conectar.
- Definição das Rotas:
  - /: Rota principal, chama handleRoot() para exibir a página de controle.
  - /5/on: Chama handleOn() para ligar o LED.
  - /5/off: Chama handleOff() para desligar o LED.

### 11. Função de loop
``` c++
void loop(){
  server.handleClient();
}
```
- ***server.handleClient()***: Verifica continuamente se há novas requisições de clientes e as processa conforme necessário.