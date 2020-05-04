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
const char *ssid = " ";
// senha da Rede WIFI
const char *password = " ";

// token BOT Telegram
const char BotToken[] = " ";

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
                if (checkOpen(m.chat_id))
                    return;

                openBox();
                // ... devolve mensagem no chat
                bot.sendMessage(m.chat_id, "Tudo certo, já abri a tampa.");
            }
            // e se a mensagem for CLOSE ...
            else if (m.text.equals("CLOSE"))
            {
                if (checkClose(m.chat_id))
                    return;

                closeBox();
                // ... devolve mensagem no chat
                bot.sendMessage(m.chat_id, "Pronto, fechei a tampa.");
            }
            else if (m.text.equals("STATUS"))
            {
                bot.sendMessage(m.chat_id, statusBox());
            }
            else
            {
                bot.sendMessage(m.chat_id,
                                "Não reconheci nenhum comando válido na sua mensagem, se deseja saber o status da tampa da caixa envie o texto "
                                "STATUS"
                                ", se deseja fechá-la envie "
                                "CLOSE"
                                " e se desejar abrí-la envie "
                                "OPEN"
                                ".");
            }
        }
    }
}

void openBox()
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

void closeBox()
{

    for (int pos = 4; pos >= 0; pos -= 1)
    {
        isMove = true;
        myservo.write(ANGLE_SERVO[pos]);
        delay(350);
    }
    isMove = false;
}

bool checkOpen(String chat_id)
{
    if (isMove)
    {
        bot.sendMessage(chat_id, "Oi, não posso executar seu comando no momento, porque já há uma operação em andamento. Verifique a situação da caixa enviando STATUS daqui a pouco.");
        return true;
    }
    if (angleServo() == ANGLE_SERVO[4])
    {
        bot.sendMessage(chat_id, "Olá, ela já estava aberta então eu não precisei fazer nada.");
        return true;
    }
    return false;
}

bool checkClose(String chat_id)
{
    if (isMove)
    {
        bot.sendMessage(chat_id, "Oi, não posso executar seu comando no momento, porque já há uma operação em andamento. Verifique a situação da caixa enviando STATUS daqui a pouco.");
        return true;
    }
    if (angleServo() == ANGLE_SERVO[0])
    {
        bot.sendMessage(chat_id, "Olá, ela já estava fechada então eu não precisei fazer nada.");
        return true;
    }
    return false;
}

char *statusBox()
{
    if (isMove)
        return "Em movimento.";

    int angle = angleServo();

    if (angle == ANGLE_SERVO[0])
    {
        return "A caixa está fechada no momento.";
    }
    else if (angle == ANGLE_SERVO[4])
    {
        return "A caixa está totalmente aberta no momento.";
    }
    else
    {
        return "Não tenho certeza, mas provavelmente a caixa está parcialmente aberta.";
    }
}

uint16 angleServo()
{
    return myservo.read();
}