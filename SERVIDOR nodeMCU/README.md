<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Documentação Técnica - Projeto ESP8266</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f9;
      color: #333;
      margin: 0;
      padding: 20px;
      line-height: 1.6;
    }

    .container {
      max-width: 800px;
      margin: auto;
      background: #ffffff;
      padding: 20px;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    }

    h1 {
      color: #0057a8;
      text-align: center;
    }
    
    h2 {
      color: #0057a8;
      border-bottom: 2px solid #ddd;
      padding-bottom: 5px;
      margin-top: 20px;
    }

    pre {
      background-color: #f4f4f9;
      border-left: 3px solid #0057a8;
      padding: 10px;
      overflow-x: auto;
    }

    table {
      width: 100%;
      border-collapse: collapse;
      margin: 20px 0;
    }

    table, th, td {
      border: 1px solid #ddd;
      padding: 8px;
    }

    th {
      background-color: #0057a8;
      color: white;
      text-align: left;
    }

    ul {
      margin: 10px 0;
      padding: 0 20px;
    }

    .code-btn {
      background-color: #0057a8;
      color: white;
      padding: 5px 10px;
      border-radius: 5px;
      cursor: pointer;
      text-align: center;
      display: inline-block;
      margin-top: 10px;
    }

    .code-btn:hover {
      background-color: #003f7f;
    }
  </style>
</head>
<body>

<div class="container">
  <h1>Documentação Técnica - Projeto ESP8266</h1>

  <h2>1. Introdução</h2>
  <p>
    Este projeto implementa um servidor web em uma placa ESP8266 (NodeMCU) para controle remoto de um LED.
    Além disso, o dispositivo está configurado para permitir atualizações OTA (Over-the-Air), facilitando o desenvolvimento
    e a manutenção do firmware.
  </p>

  <h2>2. Estrutura do Código</h2>
  <p>
    O código do projeto está organizado nas seguintes seções:
  </p>
  <ul>
    <li>Configuração de rede Wi-Fi.</li>
    <li>Configuração de atualização OTA.</li>
    <li>Servidor web para controle do LED.</li>
    <li>CSS embutido para estilizar a interface web.</li>
  </ul>

  <h2>3. Configuração e Desenvolvimento</h2>
  
  <h3>3.1 Configuração do Wi-Fi</h3>
  <p>
    No início do código, definimos o SSID e a senha para conectar o ESP8266 à rede Wi-Fi.
    Assim que a conexão é estabelecida, o IP é exibido no monitor serial.
  </p>
  <pre><code>
const char* ssid = "NomeDoWiFi";
const char* password = "SenhaDoWiFi";
  </code></pre>

  <h3>3.2 Configuração do OTA</h3>
  <p>
    Configuramos o <code>ArduinoOTA</code> para permitir atualizações OTA, incluindo mensagens de início, progresso e erro.
  </p>
  <pre><code class="cpp-code">
ArduinoOTA.onStart([]() { Serial.println("Iniciando atualização OTA..."); });
ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
});
ArduinoOTA.onError([](ota_error_t error) { Serial.printf("Erro OTA[%u]\n", error); });
ArduinoOTA.begin();
  </code></pre>

  <h3>3.3 Configuração do Servidor Web</h3>
  <p>
    O servidor web é configurado para escutar na porta 80. As rotas principais são:
  </p>
  <table>
    <tr>
      <th>Rota</th>
      <th>Função</th>
    </tr>
    <tr>
      <td><code>/</code></td>
      <td>Exibe a interface de controle do LED com botões para ligar/desligar.</td>
    </tr>
    <tr>
      <td><code>/led/on</code></td>
      <td>Ativa o LED.</td>
    </tr>
    <tr>
      <td><code>/led/off</code></td>
      <td>Desativa o LED.</td>
    </tr>
  </table>

  <h3>3.4 Interface HTML com CSS</h3>
  <p>
    A interface HTML exibe dois botões de controle do LED. O CSS embutido estiliza a página, dando uma aparência moderna.
  </p>

  <pre><code>
void handleRoot() {
  String html = "&lt;!DOCTYPE html&gt;&lt;html&gt;";
  html += "&lt;head&gt;&lt;meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"&gt;";
  html += "&lt;style&gt;";
  html += "body { font-family: Arial, sans-serif; background-color: #f4f4f9; color: #333; }";
  html += ".container { max-width: 400px; margin: auto; text-align: center; }";
  html += ".button { background-color: #0057a8; color: white; padding: 15px; border: none; cursor: pointer; }";
  html += "&lt;/style&gt;&lt;/head&gt;";
  html += "&lt;body&gt;&lt;div class='container'&gt;";
  html += "&lt;h1&gt;Controle do LED&lt;/h1&gt;";
  html += "&lt;button class='button' onclick=\"toggleLED('on')\"&gt;Ligar LED&lt;/button&gt;";
  html += "&lt;button class='button' onclick=\"toggleLED('off')\"&gt;Desligar LED&lt;/button&gt;";
  html += "&lt;/div&gt;&lt;/body&gt;&lt;/html&gt;";
  server.send(200, "text/html", html);
}
  </code></pre>

</div>

</body>
</html>
