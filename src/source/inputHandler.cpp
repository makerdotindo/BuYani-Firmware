#include "header/inputHandler.h"
#include <ConfiguratorMKR.h>

extern ConfiguratorMKR configurator;

inputHandler::inputHandler() : dht1()
{
}

inputHandler::~inputHandler()
{
}

void inputHandler::initSensor()
{
    dht1.setup(configurator.getVariable("dht1_pin").toInt(), DHTesp::DHT22);
    // this->dht1.begin();
    // this->dht2.begin();
}

void inputHandler::updateSensor()
{

    this->dht1Temp = this->dht1.getTemperature();
    this->dht1Hum = this->dht1.getHumidity();
    // this->dht2Temp = this->dht2.readTemperature();
    // this->dht2Hum = this->dht2.readHumidity();

    Serial.println("Suhu: " + (String)this->dht1Temp + " Kelembaban: " + (String)this->dht1Hum );

    if (isnan(this->dht1Temp))
    {
        this->dht1Temp = 0;
    }

    if (isnan(this->dht1Hum))
    {
        this->dht1Hum = 0;
    }

    if (isnan(this->dht2Temp))
    {
        this->dht2Temp = 0;
    }

    if (isnan(this->dht2Hum))
    {
        this->dht2Hum = 0;
    }
}

float inputHandler::getDHT1Temp()
{
    return this->dht1Temp;
}

float inputHandler::getDHT1Hum()
{
    return this->dht1Hum;
}

float inputHandler::getDHT2Temp()
{
    return this->dht2Temp;
}

float inputHandler::getDHT2Hum()
{
    return this->dht2Hum;
}
