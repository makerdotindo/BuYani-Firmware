#include "header/outputHandler.h"


outputHandler::outputHandler(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void outputHandler::init(int pin)
{
    this->off();
    this->isOn = false;
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

outputHandler::outputHandler()
{
}

void outputHandler::on()
{
    digitalWrite(pin, HIGH);
    isOn = true;
}

void outputHandler::off()
{
    digitalWrite(pin, LOW);
    isOn = false;
}

void outputHandler::toggle()
{
    if (isOn)
    {
        off();
    }
    else
    {
        on();
    }
}