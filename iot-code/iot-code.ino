// biblioteca para conexao WiFi ESP8286
#include <ESP8266WiFi.h>
// biblioteca para utilizar client SSL
#include <WiFiClientSecure.h>
// biblioteca para manipular BOT Telegram
// lamentavelmente a versao exige que o ArduinoJson 
// esteja na versao 5.13.5
#include <TelegramBot.h>

// nome da Rede WIFI
const char *ssid = " ";
// senha da Rede WIFI
const char *password = " ";

// token BOT Telegram
const char BotToken[] = " ";

WiFiClientSecure net_ssl;

TelegramBot bot(BotToken, net_ssl);

void setup()
{
  // declara uso do LED original da placa do NodeMCU
  pinMode(LED_BUILTIN, OUTPUT);

  // de
  Serial.begin(115200);

  // inicia conexao WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  bot.begin();
  // desabilita conexao segura com SSL
  net_ssl.setInsecure();
}

void loop()
{

  delay(200);
  // checa se há conexão
  if (WiFi.status() == WL_CONNECTED)
  {
    // verifica mensagens no chat
    message m = bot.getUpdates();

    // verifica se há conteúco na mensagem
    if (m.chat_id != 0)
    {
      // se a mensagem for ON ...
      if (m.text.equals("ON"))
      {
        // ... liga LED ...
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        // ... devolve mensagem no chat
        bot.sendMessage(m.chat_id, "Ligado");
      }
      // e se a mensagem for OFF ...
      else if (m.text.equals("OFF"))
      {
        // ... desliga o LED ...
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        // ... devolve mensagem no chat
        bot.sendMessage(m.chat_id, "Desligado");
      }
    }
  }
}
