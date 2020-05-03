#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TelegramBot.h>

const char *ssid = " ";
const char *password = " ";
const char BotToken[] = " ";

WiFiClientSecure net_ssl;
TelegramBot bot(BotToken, net_ssl);

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
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
  net_ssl.setInsecure();
}

void loop()
{

  if (WiFi.status() == WL_CONNECTED)
  { 
    message m = bot.getUpdates(); 
    if (m.chat_id != 0)
    {
      if (m.text.equals("ON"))
      {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        bot.sendMessage(m.chat_id, "Ligado");
      }
      else if (m.text.equals("OFF"))
      {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        bot.sendMessage(m.chat_id, "Desligado");
      }
    }
  }
}
