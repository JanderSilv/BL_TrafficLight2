#include "mbed.h"

InterruptIn button(p5);

DigitalOut redLight(p6);
DigitalOut yellowLight(p7);
DigitalOut greenLight(p8); // Blue Led

Ticker ticker;
Timeout timeout;

enum States
{
    OFF,
    RED,
    YELLOW,
    GREEN,
    ALLERT,
};

States state = RED;
States previous_state = RED;

float buttonTimePressed = 0;
bool pressFlag = false;

void startButtonPress();
void finishButtonPress();
void turnOff();
void goRed();
void goYellow();
void goGreen();
void goAllert();
void goPreviousState();
void reset();

int main()
{
    goRed();
    button.rise(&startButtonPress);
    button.fall(&finishButtonPress);
    while (1)
    {
        if (pressFlag)
            buttonTimePressed += 0.1;
        wait(0.1);
    }
}

void startButtonPress()
{
    buttonTimePressed = 0;
    pressFlag = true;
}

void finishButtonPress()
{
    printf("Botão pressionado por %.1fs\n----\n", buttonTimePressed);
    if (buttonTimePressed <= 3 && state == GREEN)
        goYellow();
    else if (buttonTimePressed <= 10)
    {
        state == ALLERT ? goPreviousState() : goAllert();
    }
    else
    {
        state == OFF ? goRed() : turnOff();
    }
    pressFlag = false;
}

void turnOff()
{
    reset();
    printf("DESLIGADO\n----\n");
    state = OFF;
}

void goRed()
{
    reset();
    printf("VERMELHO\n----\n");
    redLight = 1;
    state = RED;
    timeout.attach(&goGreen, 10);
}

void goYellow()
{
    reset();
    printf("AMARELO\n----\n");
    yellowLight = 1;
    state = YELLOW;
    timeout.attach(&goRed, 4);
}

void goGreen()
{
    reset();
    printf("VERDE\n----\n");
    greenLight = 1;
    state = GREEN;
    timeout.attach(&goYellow, 20);
}

void blinkYellow()
{
    yellowLight = !yellowLight;
}

void goAllert()
{
    reset();
    printf("ALERTA\n----\n");
    previous_state = state;
    state = ALLERT;
    ticker.attach(&blinkYellow, 0.5);
}

void goPreviousState()
{
    reset();
    switch (previous_state)
    {
    case OFF:
        turnOff();
        break;
    case RED:
        goRed();
        break;
    case YELLOW:
        goYellow();
        break;
    case GREEN:
        goGreen();
        break;
    default:
        break;
    }
}

void reset()
{
    redLight = 0;
    yellowLight = 0;
    greenLight = 0;
    ticker.detach();
    timeout.detach();
}