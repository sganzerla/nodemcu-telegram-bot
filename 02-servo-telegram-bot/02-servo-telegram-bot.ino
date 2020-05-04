#include <Servo.h>

// instância do servo
Servo myservo;

#define SERVO_PIN D3

// limites dos angulos do servo
// indicam a abertura da tampa em ordem crescente
const uint8_t ANGLE_SERVO[] = {0, 20, 40, 60, 80};

// indica se a tampa está em movimento
bool isMove = false;

void setup()
{
    Serial.begin(115200);

    // associa o pino do nodemcu com o servo
    myservo.attach(SERVO_PIN);

    // define como fechada a tampa da caixa
    myservo.write(ANGLE_SERVO[0]);
}

void loop()
{
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

    int angle = myservo.read();

    if (angle == ANGLE_SERVO[0])
    {
        return "Tampa fechada.";
    }
    else if (angle == ANGLE_SERVO[80])
    {
        return "Tampa totalmente aberta.";
    }
    else
    {
        return "Não tenho certeza, mas provavelmente a tampa está parcialmente aberta.";
    }
}