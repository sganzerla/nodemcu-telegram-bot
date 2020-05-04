// biblioteca para conexao WiFi ESP8286
#include <ESP8266WiFi.h>
// biblioteca para utilizar client SSL
#include <WiFiClientSecure.h>
// biblioteca para manipular BOT Telegram
// lamentavelmente a versao exige que o ArduinoJson
// esteja na versao 5.13.5
#include <TelegramBot.h>

#include <Servo.h>

// nome da Rede WIFI
const char *ssid = "";
// senha da Rede WIFI
const char *password = "";

// token BOT Telegram
const char BotToken[] = "";

WiFiClientSecure net_ssl;

TelegramBot bot(BotToken, net_ssl);

// instância do servo
Servo myservo;

#define SERVO_PIN D3

// limites dos angulos do servo
// indicam a abertura da tampa em ordem decrescente
const uint8_t ANGLE_SERVO[] = {80, 60, 40, 20, 0};

// indica se a tampa está em movimento
bool isMove = false;

void setup()
{
    Serial.begin(115200);

    // associa o pino do nodemcu com o servo
    myservo.attach(SERVO_PIN);

    // define como fechada a tampa da caixa
    myservo.write(ANGLE_SERVO[0]);

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

    Serial.print("Angulo Servo: ");
    Serial.println(angleServo());
}

void loop()
{

    delay(200);
    // checa se há conexão
    if (WiFi.status() == WL_CONNECTED)
    {
        // verifica mensagens no chat
        message m = bot.getUpdates();

        // verifica se há conteúdo na mensagem
        if (m.chat_id != 0)
        {
            // se a mensagem for OPEN
            if (m.text.equals("OPEN"))
            {
                if (isMove)
                {
                    bot.sendMessage(m.chat_id, "Lamento, a tampa já estava em movimento, tente novamente em instantes.");
                    return;
                }
                if (angleServo() == ANGLE_SERVO[4])
                {
                    bot.sendMessage(m.chat_id, "Tampa já estava aberta.");
                    return;
                }
                abrir();
                // ... devolve mensagem no chat
                bot.sendMessage(m.chat_id, "Tudo certo, já abri a tampa.");
            }
            // e se a mensagem for CLOSE ...
            else if (m.text.equals("CLOSE"))
            {
                if (isMove)
                {
                    bot.sendMessage(m.chat_id, "Lamento, a tampa já estava em movimento, tente novamente em instantes.");
                    return;
                }
                if (angleServo() == ANGLE_SERVO[0])
                {
                    bot.sendMessage(m.chat_id, "Tampa já estava fechada.");
                    return;
                }
                fechar();
                // ... devolve mensagem no chat
                bot.sendMessage(m.chat_id, "Pronto, fechei a tampa.");
            }
            else
            {
                bot.sendMessage(m.chat_id, situacaoTampa());
            }
        }
    }
}

void abrir()
{
    for (int pos = 0; pos <= 4; pos += 1)
    {
        isMove = true;
        myservo.write(ANGLE_SERVO[pos]);
        Serial.println(myservo.read());
        delay(350);
    }
    isMove = false;
}

void fechar()
{

    for (int pos = 4; pos >= 0; pos -= 1)
    {
        isMove = true;
        myservo.write(ANGLE_SERVO[pos]);
        delay(350);
    }
    isMove = false;
}

char *situacaoTampa()
{
    if (isMove)
        return "Em movimento.";

    int angle = angleServo();

    if (angle == ANGLE_SERVO[0])
    {
        return "Tampa fechada.";
    }
    else if (angle == ANGLE_SERVO[4])
    {
        return "Tampa totalmente aberta.";
    }
    else
    {
        return "Não tenho certeza, mas provavelmente a tampa está parcialmente aberta.";
    }
}

uint16 angleServo()
{
    return myservo.read();
}